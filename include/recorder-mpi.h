

#define __D_MPI_REQUEST MPIO_Request

#include <stdio.h>
#ifdef HAVE_MNTENT_H
#include <mntent.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <zlib.h>
#include <assert.h>
#include <search.h>

#include "mpi.h"
#include "recorder.h"

#if MPI_VERSION >= 3
#define CONST const
#else
#define CONST
#endif

typedef struct MPICommHash_t {
    void *key;      // MPI_Comm as key
    char* id;
    UT_hash_handle hh;
} MPICommHash;

typedef struct MPIFileHash_t {
    void *key;
    char* id;
    UT_hash_handle hh;
} MPIFileHash;

static MPICommHash *mpi_comm_table = NULL;
static MPIFileHash *mpi_file_table = NULL;
static int mpi_file_id = 0;
static int mpi_comm_id = 0;

void add_mpi_file(MPI_Comm comm, MPI_File *file) {
    if(file == NULL)
        return;

    int rank, world_rank;
    PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    PMPI_Comm_rank(comm, &rank);

    MPIFileHash *entry = malloc(sizeof(MPIFileHash));
    entry->key = malloc(sizeof(MPI_File));
    memcpy(entry->key, file, sizeof(MPI_File));

    char* id = calloc(32, sizeof(char));
    if(rank == 0)
        sprintf(id, "%d-%d", world_rank, mpi_file_id++);
    PMPI_Bcast(id, 32, MPI_BYTE, 0, comm);
    entry->id = id;

    HASH_ADD_KEYPTR(hh, mpi_file_table, entry->key, sizeof(MPI_File), entry);
}

char* file2id(MPI_File *file) {
    if(file == NULL)
        return strdup("MPI_FILE_NULL");
    else {
        MPIFileHash *entry = NULL;
        HASH_FIND(hh, mpi_file_table, file, sizeof(MPI_File), entry);
        if(entry)
            return strdup(entry->id);
        else
            return strdup("MPI_FILE_UNKNOWN");
    }
}

void add_mpi_comm(MPI_Comm *newcomm) {
    if(newcomm == NULL || *newcomm == MPI_COMM_NULL)
        return;
    int new_rank, world_rank;
    PMPI_Comm_rank(*newcomm, &new_rank);
    PMPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPICommHash *entry = malloc(sizeof(MPICommHash));
    entry->key = malloc(sizeof(MPI_Comm));
    memcpy(entry->key, newcomm, sizeof(MPI_Comm));

    // Rank 0 of the new communicator decides an unique id
    // and broadcast it to all others, then everyone stores it
    // in the hash table.
    char *id = calloc(32, sizeof(char));
    if(new_rank == 0)
        sprintf(id, "%d-%d", world_rank, mpi_comm_id++);
    PMPI_Bcast(id, 32, MPI_BYTE, 0, *newcomm);
    entry->id = id;

    HASH_ADD_KEYPTR(hh, mpi_comm_table, entry->key, sizeof(MPI_Comm), entry);
}

char* comm2name(MPI_Comm *comm) {
    if(comm == NULL || *comm == MPI_COMM_NULL)
        return strdup("MPI_COMM_NULL");
    else if(*comm == MPI_COMM_WORLD) {
        return strdup("MPI_COMM_WORLD");
    } else if(*comm == MPI_COMM_SELF) {
        return strdup("MPI_COMM_SELF");
    } else {
        MPICommHash *entry = NULL;
        HASH_FIND(hh, mpi_comm_table, comm, sizeof(MPI_Comm), entry);
        if(entry)
            return strdup(entry->id);
        else
            return strdup("MPI_COMM_UNKNOWN");
    }
}

static inline char *type2name(MPI_Datatype type) {
    char *tmp = malloc(128);
    int len;
    PMPI_Type_get_name(type, tmp, &len);
    tmp[len] = 0;
    if(len == 0) strcpy(tmp, "MPI_TYPE_UNKNOWN");
    return tmp;
}

static inline char* status2str(MPI_Status *status) {
    char *tmp = calloc(128, sizeof(char));
    if(status == MPI_STATUS_IGNORE)
        strcpy(tmp, "MPI_STATUS_IGNORE");
    else
        sprintf(tmp, "[%d %d %d]", status->MPI_SOURCE, status->MPI_TAG, status->MPI_ERROR);
    return tmp;
}

static inline char* whence2name(int whence) {
    if(whence == MPI_SEEK_SET)
        return strdup("MPI_SEEK_SET");
    if(whence == MPI_SEEK_CUR)
        return strdup("MPI_SEEK_CUR");
    if(whence == MPI_SEEK_END)
        return strdup("MPI_SEEK_END");
}


/**
 * Intercept the following functions
 */
int RECORDER_MPI_DECL(MPI_Comm_size)(MPI_Comm comm, int *size);
int RECORDER_MPI_DECL(MPI_Comm_rank)(MPI_Comm comm, int *rank);
int RECORDER_MPI_DECL(MPI_Get_processor_name)(char *name, int *resultlen);
int RECORDER_MPI_DECL(MPI_Comm_set_errhandler)(MPI_Comm comm, MPI_Errhandler errhandler);
int RECORDER_MPI_DECL(MPI_Barrier)(MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Bcast)(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Ibcast)(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request);
int RECORDER_MPI_DECL(MPI_Gather)(CONST void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
        int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Scatter)(CONST void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
        int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Gatherv)(CONST void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
        CONST int *rcount, CONST int *displs, MPI_Datatype rtype, int root, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Scatterv)(CONST void *sbuf, CONST int *scount, CONST int *displa,
        MPI_Datatype stype, void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);

/*
   int MPI_Allgather(CONST void* sbuf, int scount, MPI_Datatype stype, void* rbuf,
   int rcount, MPI_Datatype rtype, MPI_Comm comm ) {
   }
   */

int RECORDER_MPI_DECL(MPI_Allgatherv)(CONST void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
        CONST int *rcount, CONST int *displs, MPI_Datatype rtype, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Alltoall)(CONST void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
        int rcount, MPI_Datatype rtype, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Reduce)(CONST void *sbuf, void *rbuf, int count, MPI_Datatype stype, MPI_Op op, int root, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Allreduce)(CONST void *sbuf, void *rbuf, int count, MPI_Datatype stype, MPI_Op op, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Reduce_scatter)(CONST void *sbuf, void *rbuf, CONST int *rcounts,
        MPI_Datatype stype, MPI_Op op, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Scan)(CONST void *sbuf, void *rbuf, int count, MPI_Datatype stype, MPI_Op op, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Type_create_darray)(int size, int rank, int ndims, CONST int array_of_gsizes[], CONST int array_of_distribs[],
        CONST int array_of_dargs[], CONST int array_of_psizes[], int order, MPI_Datatype oldtype, MPI_Datatype *newtype);
int RECORDER_MPI_DECL(MPI_Type_commit)(MPI_Datatype *datatype);
int RECORDER_MPI_DECL(MPI_File_open)(MPI_Comm comm, CONST char *filename, int amode, MPI_Info info, MPI_File *fh);
int RECORDER_MPI_DECL(MPI_File_close)(MPI_File *fh);
int RECORDER_MPI_DECL(MPI_File_sync)(MPI_File fh);
int RECORDER_MPI_DECL(MPI_File_set_size)(MPI_File fh, MPI_Offset size);
int RECORDER_MPI_DECL(MPI_File_set_view)(MPI_File fh, MPI_Offset disp, MPI_Datatype etype,
        MPI_Datatype filetype, CONST char *datarep, MPI_Info info);
int RECORDER_MPI_DECL(MPI_File_read)(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_read_at)(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_read_at_all)(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_read_all)(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_read_shared)(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_read_ordered)(MPI_File fh, void *buf, int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_read_at_all_begin)(MPI_File fh, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype);
int RECORDER_MPI_DECL(MPI_File_read_all_begin)(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
int RECORDER_MPI_DECL(MPI_File_read_ordered_begin)(MPI_File fh, void *buf, int count, MPI_Datatype datatype);
int RECORDER_MPI_DECL(MPI_File_iread_at)(MPI_File fh, MPI_Offset offset, void *buf, int count,
        MPI_Datatype datatype, __D_MPI_REQUEST *request);
int RECORDER_MPI_DECL(MPI_File_iread)(MPI_File fh, void *buf, int count,
        MPI_Datatype datatype, __D_MPI_REQUEST *request);
int RECORDER_MPI_DECL(MPI_File_iread_shared)(MPI_File fh, void *buf, int count,
        MPI_Datatype datatype, __D_MPI_REQUEST *request);
int RECORDER_MPI_DECL(MPI_File_write)(MPI_File fh, CONST void *buf, int count,
        MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_write_at)(MPI_File fh, MPI_Offset offset, CONST void *buf,
        int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_write_at_all)(MPI_File fh, MPI_Offset offset, CONST void *buf,
        int count, MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_write_all)(MPI_File fh, CONST void *buf, int count,
        MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_write_shared)(MPI_File fh, CONST void *buf, int count,
        MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_write_ordered)(MPI_File fh, CONST void *buf, int count,
        MPI_Datatype datatype, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_File_write_at_all_begin)(MPI_File fh, MPI_Offset offset, CONST void *buf,
        int count, MPI_Datatype datatype);
int RECORDER_MPI_DECL(MPI_File_write_all_begin)(MPI_File fh, CONST void *buf, int count, MPI_Datatype datatype);
int RECORDER_MPI_DECL(MPI_File_write_ordered_begin)(MPI_File fh, CONST void *buf, int count, MPI_Datatype datatype);
int RECORDER_MPI_DECL(MPI_File_iwrite_at)(MPI_File fh, MPI_Offset offset, CONST void *buf,
        int count, MPI_Datatype datatype, __D_MPI_REQUEST *request);
int RECORDER_MPI_DECL(MPI_File_iwrite)(MPI_File fh, CONST void *buf, int count,
        MPI_Datatype datatype, __D_MPI_REQUEST *request);
int RECORDER_MPI_DECL(MPI_File_iwrite_shared)(MPI_File fh, CONST void *buf, int count,
        MPI_Datatype datatype, __D_MPI_REQUEST *request);
int RECORDER_MPI_DECL(MPI_Finalized)(int *flag);
int RECORDER_MPI_DECL(MPI_Cart_rank) (MPI_Comm comm, CONST int coords[], int *rank);
int RECORDER_MPI_DECL(MPI_Cart_create) (MPI_Comm comm_old, int ndims, CONST int dims[], CONST int periods[], int reorder, MPI_Comm *comm_cart);
int RECORDER_MPI_DECL(MPI_Cart_get) (MPI_Comm comm, int maxdims, int dims[], int periods[], int coords[]);
int RECORDER_MPI_DECL(MPI_Cart_shift) (MPI_Comm comm, int direction, int disp, int *rank_source, int *rank_dest);
int RECORDER_MPI_DECL(MPI_Wait) (MPI_Request *request, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_Send) (CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Recv) (void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_Sendrecv) (CONST void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype,
                                        int source, int recvtag, MPI_Comm comm, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_Isend) (CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request);
int RECORDER_MPI_DECL(MPI_Irecv) (void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request);
int RECORDER_MPI_DECL(MPI_Waitall) (int count, MPI_Request requests[], MPI_Status statuses[]);
int RECORDER_MPI_DECL(MPI_Waitsome) (int incount, MPI_Request requests[], int *outcount, int indices[], MPI_Status statuses[]);
int RECORDER_MPI_DECL(MPI_Waitany) (int count, MPI_Request requests[], int *indx, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_Ssend) (CONST void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
int RECORDER_MPI_DECL(MPI_Comm_split) (MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
int RECORDER_MPI_DECL(MPI_Comm_create) (MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm);
int RECORDER_MPI_DECL(MPI_Comm_dup) (MPI_Comm comm, MPI_Comm *newcomm);
int RECORDER_MPI_DECL(MPI_File_seek) (MPI_File fh, MPI_Offset offset, int whence);
int RECORDER_MPI_DECL(MPI_File_seek_shared) (MPI_File fh, MPI_Offset offset, int whence);
int RECORDER_MPI_DECL(MPI_File_get_size) (MPI_File fh, MPI_Offset *offset);
int RECORDER_MPI_DECL(MPI_Test) (MPI_Request *request, int *flag, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_Testall) (int count, MPI_Request requests[], int *flag, MPI_Status statuses[]);
int RECORDER_MPI_DECL(MPI_Testsome) (int incount, MPI_Request requests[], int *outcount, int indices[], MPI_Status statuses[]);
int RECORDER_MPI_DECL(MPI_Testany) (int count, MPI_Request requests[], int *indx, int *flag, MPI_Status *status);
int RECORDER_MPI_DECL(MPI_Ireduce) (const void *sbuf, void *rbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request *request);
int RECORDER_MPI_DECL(MPI_Igather) (const void *sbuf, int scount, MPI_Datatype stype, void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm, MPI_Request *request);
int RECORDER_MPI_DECL(MPI_Iscatter) (const void *sbuf, int scount, MPI_Datatype stype, void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm, MPI_Request *request);
int RECORDER_MPI_DECL(MPI_Ialltoall) (const void *sbuf, int scount, MPI_Datatype stype, void *rbuf, int rcount, MPI_Datatype rtype, MPI_Comm comm, MPI_Request * request);
int RECORDER_MPI_DECL(MPI_Comm_free) (MPI_Comm *comm);
int RECORDER_MPI_DECL(MPI_Cart_sub) (MPI_Comm comm, const int remain_dims[], MPI_Comm *newcomm);
int RECORDER_MPI_DECL(MPI_Comm_split_type) (MPI_Comm comm, int split_type, int key, MPI_Info info, MPI_Comm *newcomm);
