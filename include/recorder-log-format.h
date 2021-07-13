/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted for any purpose (including commercial purposes)
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or materials provided with the distribution.
 *
 * 3. In addition, redistributions of modified forms of the source or binary
 *    code must carry prominent notices stating that the original code was
 *    changed and the date of the change.
 *
 * 4. All publications or advertising materials mentioning features or use of
 *    this software are asked, but not required, to acknowledge that it was
 *    developed by The HDF Group and by the National Center for Supercomputing
 *    Applications at the University of Illinois at Urbana-Champaign and
 *    credit the contributors.
 *
 * 5. Neither the name of The HDF Group, the name of the University, nor the
 *    name of any Contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission from
 *    The HDF Group, the University, or the Contributor, respectively.
 *
 * DISCLAIMER:
 * THIS SOFTWARE IS PROVIDED BY THE HDF GROUP AND THE CONTRIBUTORS
 * "AS IS" WITH NO WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED. In no
 * event shall The HDF Group or the Contributors be liable for any damages
 * suffered by the users arising out of the use of this software, even if
 * advised of the possibility of such damage.
 *
 * Portions of Recorder were developed with support from the Lawrence Berkeley
 * National Laboratory (LBNL) and the United States Department of Energy under
 * Prime Contract No. DE-AC02-05CH11231.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __RECORDER_LOG_FORMAT_H
#define __RECORDER_LOG_FORMAT_H

#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#if !defined PRId64 || defined(PRI_MACROS_BROKEN)
#ifndef __WORDSIZE
#error failed to detect PRId64 or word size
#endif
# undef PRId64
#if __WORDSIZE == 64
# define PRId64 "ld"
#else
# define PRId64 "lld"
#endif
#endif
#if !defined PRIu64 || defined(PRI_MACROS_BROKEN)
#ifndef __WORDSIZE
#error failed to detect PRId64 or word size
#endif
# undef PRIu64
#if __WORDSIZE == 64
# define PRIu64 "lu"
#else
# define PRIu64 "llu"
#endif
#endif

/* For each function call in the trace file */
typedef struct Record_t {
    char status;                // peephole compressed or not
    double tstart, tend;
    unsigned char func_id;      // we have about 200 functions in total
    int arg_count;
    char **args;                // Store all arguments in array
    int res;                    // result returned from the original function call
} Record;


// Compression method, use peephole compression by default
enum CompressionMode_t { COMP_TEXT=0, COMP_BINARY=1, COMP_RECORDER=2, COMP_ZLIB=3 };
typedef enum CompressionMode_t CompressionMode;


typedef struct RecorderGlobalDef_t {
    double time_resolution;
    int total_ranks;
    CompressionMode compression_mode;
    int peephole_window_size;
} RecorderGlobalDef;


typedef struct RecorderLocalDef_t {
    double start_timestamp;
    double end_timestamp;
    int num_files;                  // number of files accessed by the rank
    int total_records;              // total number of records we have written
    char **filemap;                 // mapping of filenames and integer ids. only set when read the local def file
    size_t *file_sizes;             // size of each file accessed. only set when read back the local def file
    int function_count[256];        // counting the functions at runtime
} RecorderLocalDef;



static const char* func_list[] = {
    // POSIX I/O - 72 functions
    "creat",        "creat64",      "open",         "open64",   "close",
    "write",        "read",         "lseek",        "lseek64",  "pread",
    "pread64",      "pwrite",       "pwrite64",     "readv",    "writev",
    "mmap",         "mmap64",       "fopen",        "fopen64",  "fclose",
    "fwrite",       "fread",        "ftell",        "fseek",    "fsync",
    "fdatasync",    "__xstat",      "__xstat64",    "__lxstat", "__lxstat64",
    "__fxstat",     "__fxstat64",   "getcwd",       "mkdir",    "rmdir",
    "chdir",        "link",         "linkat",       "unlink",   "symlink",
    "symlinkat",    "readlink",     "readlinkat",   "rename",   "chmod",
    "chown",        "lchown",       "utime",        "opendir",  "readdir",
    "closedir",     "rewinddir",    "mknod",        "mknodat",  "fcntl",
    "dup",          "dup2",         "pipe",         "mkfifo",   "umask",
    "fdopen",       "fileno",       "access",       "faccessat","tmpfile",
    "remove",       "truncate",     "ftruncate",    /* "vfprintf", */ "msync",
    "fseeko",       "ftello",

#ifndef RECORDER_GOTCHA
    // MPI 87 functions
    "PMPI_File_close",              "PMPI_File_set_size",       "PMPI_File_iread_at",
    "PMPI_File_iread",              "PMPI_File_iread_shared",   "PMPI_File_iwrite_at",
    "PMPI_File_iwrite",             "PMPI_File_iwrite_shared",  "PMPI_File_open",
    "PMPI_File_read_all_begin",     "PMPI_File_read_all",       "PMPI_File_read_at_all",
    "PMPI_File_read_at_all_begin",  "PMPI_File_read_at",        "PMPI_File_read",
    "PMPI_File_read_ordered_begin", "PMPI_File_read_ordered",   "PMPI_File_read_shared",
    "PMPI_File_set_view",           "PMPI_File_sync",           "PMPI_File_write_all_begin",
    "PMPI_File_write_all",          "PMPI_File_write_at_all_begin", "PMPI_File_write_at_all",
    "PMPI_File_write_at",           "PMPI_File_write",          "PMPI_File_write_ordered_begin",
    "PMPI_File_write_ordered",      "PMPI_File_write_shared",   "PMPI_Finalize",
    "PMPI_Finalized",               "PMPI_Init",                "PMPI_Init_thread",
    /* no wrapper */ "PMPI_Wtime",                   "PMPI_Comm_rank",           "PMPI_Comm_size",
    "PMPI_Get_processor_name",      /* "PMPI_Get_processor_name", */  "PMPI_Comm_set_errhandler",
    "PMPI_Barrier",                 "PMPI_Bcast",               "PMPI_Gather",
    "PMPI_Gatherv",                 "PMPI_Scatter",             "PMPI_Scatterv",
    "PMPI_Allgather",               "PMPI_Allgatherv",          "PMPI_Alltoall",
    "PMPI_Reduce",                  "PMPI_Allreduce",           "PMPI_Reduce_scatter",
    "PMPI_Scan",                    "PMPI_Type_commit",         /* no wrapper */ "PMPI_Type_contiguous",
    /* no wrapper */ "PMPI_Type_extent",            /* no wrapper */ "PMPI_Type_free",          /* no wrapper */ "PMPI_Type_hindexed",
    /* no wrapper */ "PMPI_Op_create",              /* no wrapper */ "PMPI_Op_free",            /* no wrapper */ "PMPI_Type_get_envelope",
    /* no wrapper */ "PMPI_Type_size",
    // Added 2019/01/07
    "PMPI_Cart_rank",               "PMPI_Cart_create",         "PMPI_Cart_get",
    "PMPI_Cart_shift",              "PMPI_Wait",                "PMPI_Send",
    "PMPI_Recv",                    /* no wrapper */ "PMPI_Sendrecv",            "PMPI_Isend",
    "PMPI_Irecv",
    // Added 2020/02/24
    "PMPI_Info_create",             /* no wrapper */ "PMPI_Info_set",            /* no wrapper */ "PMPI_Info_get",
    // Added 2020/08/06
    "PMPI_Waitall",                 "PMPI_Waitsome",            "PMPI_Waitany",
    "PMPI_Ssend",
    // Added 2020/08/17
    "PMPI_Comm_split",              "PMPI_Comm_dup",            "PMPI_Comm_create",
    // Added 2020/08/27
    "PMPI_File_seek",               "PMPI_File_seek_shared",
    // Added 2020/11/05, 2020/11/13
    "PMPI_File_get_size",           "PMPI_Ibcast",
    // Added 2020/12/18
    "PMPI_Test",                    "PMPI_Testall",             "PMPI_Testsome",
    "PMPI_Testany",                 "PMPI_Ireduce",             "PMPI_Iscatter",
    "PMPI_Igather",                 "PMPI_Ialltoall",
    // Added 2021/01/25
    "PMPI_Comm_free",               "PMPI_Cart_sub",            "PMPI_Comm_split_type",
#else
    "MPI_Comm_size",              "MPI_Comm_rank",       "MPI_Get_processor_name",
    "MPI_Comm_set_errhandler",              "MPI_Barrier",   "MPI_Bcast",
    "MPI_Ibcast",             "MPI_Gather",  "MPI_Scatter",
    "MPI_Gatherv",       "MPI_Scatterv",
    "MPI_Allgather",  "MPI_Allgatherv",        "MPI_Alltoall",
    "MPI_Reduce", "MPI_Allreduce",   "MPI_Reduce_scatter",
    "MPI_Scan",           "MPI_Type_create_darray",           "MPI_Type_commit",
    "MPI_File_open",          "MPI_File_close", "MPI_File_sync",
    "MPI_File_set_size",           "MPI_File_set_view",          "MPI_File_read",
    "MPI_File_read_at",      "MPI_File_read_at_all",   "MPI_File_read_all",
    "MPI_File_read_shared",               "MPI_File_read_ordered",                "MPI_File_read_at_all_begin",
    "MPI_File_read_all_begin",                   "MPI_File_read_ordered_begin",           "MPI_File_iread_at",
    "MPI_File_iread",      "MPI_File_iread_shared",  "MPI_File_write",
    "MPI_File_write_at",                 "MPI_File_write_at_all",               "MPI_File_write_all",
    "MPI_File_write_shared",                 "MPI_File_write_ordered",             "MPI_File_write_at_all_begin",
    "MPI_File_write_all_begin",               "MPI_File_write_ordered_begin",          "MPI_File_iwrite_at",
    "MPI_File_iwrite",                  "MPI_File_iwrite_shared",           "MPI_Finalized",
    "MPI_Cart_rank",                    "MPI_Cart_create",         "MPI_Cart_get",
    "MPI_Cart_shift",             "MPI_Wait",           "MPI_Send",
    "MPI_Recv",               "MPI_Sendrecv",             "MPI_Isend",
    "MPI_Irecv",
    "MPI_Waitall",               "MPI_Waitsome",         "MPI_Waitany",
    "MPI_Ssend",              "MPI_Comm_split",                "MPI_Comm_create",
    "MPI_Comm_dup",                    "MPI_File_seek",            "MPI_File_seek_shared",
    "MPI_File_get_size",
    "MPI_Test",             "MPI_Testall",            "MPI_Testsome",
    "MPI_Testany",                 "MPI_Ireduce",            "MPI_Igather",
    "MPI_Iscatter",
    // Added 2020/08/17
    "MPI_Ialltoall",              "MPI_Comm_free",            "MPI_Cart_sub",
    // Added 2020/08/27
    "MPI_Comm_split_type",               "PMPI_Init", "PMPI_Init_thread",
    // Added 2020/11/05, 2020/11/13
    "MPI_Init",           "MPI_Init_thread",
    // Added 2020/12/18
    "PMPI_Finalize",                    "MPI_Finalize",            
#endif /* RECORDER_GOTCHA */
    // HDF5 I/O - 74 functions
    "H5Fcreate",            "H5Fopen",              "H5Fclose",     "H5Fflush", // File interface
    "H5Gclose",             "H5Gcreate1",           "H5Gcreate2",   // Group interface
    "H5Gget_objinfo",       "H5Giterate",           "H5Gopen1",
    "H5Gopen2",             "H5Dclose",             "H5Dcreate1",
    "H5Dcreate2",           "H5Dget_create_plist",  "H5Dget_space", // Dataset interface
    "H5Dget_type",          "H5Dopen1",             "H5Dopen2",
    "H5Dread",              "H5Dwrite",             "H5Dset_extent",
    "H5Sclose",                                                     // Dataspace interface
    "H5Screate",            "H5Screate_simple",     "H5Sget_select_npoints",
    "H5Sget_simple_extent_dims", "H5Sget_simple_extent_npoints", "H5Sselect_elements",
    "H5Sselect_hyperslab",  "H5Sselect_none",       "H5Tclose",     // Datatype interface
    "H5Tcopy",              "H5Tget_class",         "H5Tget_size",
    "H5Tset_size",          "H5Tcreate",            "H5Tinsert",
    "H5Aclose",             "H5Acreate1",           "H5Acreate2",   // Attribute interface
    "H5Aget_name",          "H5Aget_num_attrs",     "H5Aget_space",
    "H5Aget_type",          "H5Aopen",              "H5Aopen_idx",
    "H5Aopen_name",         "H5Aread",              "H5Awrite",
    "H5Pclose",             "H5Pcreate",            "H5Pget_chunk", // Property List interface
    "H5Pget_mdc_config",    "H5Pset_alignment",     "H5Pset_chunk",
    "H5Pset_dxpl_mpio",     "H5Pset_fapl_core",     "H5Pset_fapl_mpio",
    /* "H5Pset_fapl_mpiposix", */ "H5Pset_istore_k",      "H5Pset_mdc_config",
    "H5Pset_meta_block_size","H5Lexists",           "H5Lget_val",   // Link interface
    "H5Literate",           "H5Oclose",             "H5Oget_info",  // Object interface
    "H5Oget_info_by_name",  "H5Oopen",
    "H5Pset_coll_metadata_write",                   "H5Pget_coll_metadata_write",   // collective metadata
    "H5Pset_all_coll_metadata_ops",                 "H5Pget_all_coll_metadata_ops"
};

#endif /* __RECORDER_LOG_FORMAT_H */
