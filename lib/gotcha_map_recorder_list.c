#ifdef RECORDER_GOTCHA
#include <gotcha/gotcha.h>
#include "recorder.h"
#include "recorder-posix.h"
#include "recorder-mpi.h"
#include "recorder-hdf5.h"

/* define gotcha-specific state to use with our wrapper */
#define RECORDER_WRAPPEE_HANDLE_DECL(func) \
gotcha_wrappee_handle_t wrappee_handle_ ## func

#define RECORDER_WRAPPEE_HANDLE(func) wrappee_handle_ ## func

/* POSIX I/O */
RECORDER_WRAPPEE_HANDLE_DECL(close);
RECORDER_WRAPPEE_HANDLE_DECL(fclose);
RECORDER_WRAPPEE_HANDLE_DECL(fsync);
RECORDER_WRAPPEE_HANDLE_DECL(fdatasync);
RECORDER_WRAPPEE_HANDLE_DECL(mmap64);
RECORDER_WRAPPEE_HANDLE_DECL(mmap);
RECORDER_WRAPPEE_HANDLE_DECL(msync);
RECORDER_WRAPPEE_HANDLE_DECL(creat);
RECORDER_WRAPPEE_HANDLE_DECL(creat64);
RECORDER_WRAPPEE_HANDLE_DECL(open64);
RECORDER_WRAPPEE_HANDLE_DECL(open);
RECORDER_WRAPPEE_HANDLE_DECL(fopen64);
RECORDER_WRAPPEE_HANDLE_DECL(fopen);
// stat/fstat/lstat are wrappers in GLIBC and dlsym can not hook them.
// Instead, xstat/lxstat/fxstat are their GLIBC implementations so we can hook them.
RECORDER_WRAPPEE_HANDLE_DECL(__xstat);
RECORDER_WRAPPEE_HANDLE_DECL(__xstat64);
RECORDER_WRAPPEE_HANDLE_DECL(__lxstat);
RECORDER_WRAPPEE_HANDLE_DECL(__lxstat64);
RECORDER_WRAPPEE_HANDLE_DECL(__fxstat);
RECORDER_WRAPPEE_HANDLE_DECL(__fxstat64);
RECORDER_WRAPPEE_HANDLE_DECL(pread64);
RECORDER_WRAPPEE_HANDLE_DECL(pread);
RECORDER_WRAPPEE_HANDLE_DECL(pwrite64);
RECORDER_WRAPPEE_HANDLE_DECL(pwrite);
RECORDER_WRAPPEE_HANDLE_DECL(readv);
RECORDER_WRAPPEE_HANDLE_DECL(writev);
RECORDER_WRAPPEE_HANDLE_DECL(fread);
RECORDER_WRAPPEE_HANDLE_DECL(fwrite);
RECORDER_WRAPPEE_HANDLE_DECL(fprintf);   // we need to use vprintf to trace fprintf so we can pass va_list
RECORDER_WRAPPEE_HANDLE_DECL(read);
RECORDER_WRAPPEE_HANDLE_DECL(write);
RECORDER_WRAPPEE_HANDLE_DECL(fseek);
RECORDER_WRAPPEE_HANDLE_DECL(ftell);
RECORDER_WRAPPEE_HANDLE_DECL(lseek64);
RECORDER_WRAPPEE_HANDLE_DECL(lseek);
/* Other POSIX Function Calls, not directly related to I/O */
// Files and Directories
RECORDER_WRAPPEE_HANDLE_DECL(getcwd);
RECORDER_WRAPPEE_HANDLE_DECL(mkdir);
RECORDER_WRAPPEE_HANDLE_DECL(rmdir);
RECORDER_WRAPPEE_HANDLE_DECL(chdir);
RECORDER_WRAPPEE_HANDLE_DECL(link);
RECORDER_WRAPPEE_HANDLE_DECL(linkat);
RECORDER_WRAPPEE_HANDLE_DECL(unlink);
RECORDER_WRAPPEE_HANDLE_DECL(symlink);
RECORDER_WRAPPEE_HANDLE_DECL(symlinkat);
RECORDER_WRAPPEE_HANDLE_DECL(readlink);
RECORDER_WRAPPEE_HANDLE_DECL(readlinkat);
RECORDER_WRAPPEE_HANDLE_DECL(rename);
RECORDER_WRAPPEE_HANDLE_DECL(chmod);
RECORDER_WRAPPEE_HANDLE_DECL(chown);
RECORDER_WRAPPEE_HANDLE_DECL(lchown);
RECORDER_WRAPPEE_HANDLE_DECL(utime);
RECORDER_WRAPPEE_HANDLE_DECL(opendir);
RECORDER_WRAPPEE_HANDLE_DECL(readdir);
RECORDER_WRAPPEE_HANDLE_DECL(closedir);
RECORDER_WRAPPEE_HANDLE_DECL(rewinddir); // no wrapper definition
RECORDER_WRAPPEE_HANDLE_DECL(mknod);
RECORDER_WRAPPEE_HANDLE_DECL(mknodat);
// Advanced File Operations
#ifndef DISABLE_FCNTL_TRACE
RECORDER_WRAPPEE_HANDLE_DECL(fcntl); // depends on DISABLE_FCNTL_TRACE, could be no wrapper definition
#endif
RECORDER_WRAPPEE_HANDLE_DECL(dup);
RECORDER_WRAPPEE_HANDLE_DECL(dup2);
RECORDER_WRAPPEE_HANDLE_DECL(pipe);
RECORDER_WRAPPEE_HANDLE_DECL(mkfifo);
RECORDER_WRAPPEE_HANDLE_DECL(umask);
RECORDER_WRAPPEE_HANDLE_DECL(fdopen);
RECORDER_WRAPPEE_HANDLE_DECL(fileno);
RECORDER_WRAPPEE_HANDLE_DECL(access);
RECORDER_WRAPPEE_HANDLE_DECL(faccessat);
RECORDER_WRAPPEE_HANDLE_DECL(tmpfile);
RECORDER_WRAPPEE_HANDLE_DECL(remove);
RECORDER_WRAPPEE_HANDLE_DECL(truncate);
RECORDER_WRAPPEE_HANDLE_DECL(ftruncate);
// Added 01/15/2021
RECORDER_WRAPPEE_HANDLE_DECL(fseeko);
RECORDER_WRAPPEE_HANDLE_DECL(ftello);
// Others
//int statfs(const char *path, struct statfs *buf);
//int fstatfs(int fd, struct statfs *buf);



/* MPI Function Calls */
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_size);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_rank);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Get_processor_name);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_set_errhandler);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Barrier);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Bcast);
// Add MPI_Ibcast on 2020/11/13
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Ibcast);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Gather);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Scatter);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Gatherv);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Scatterv);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Allgather);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Allgatherv);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Alltoall);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Reduce);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Allreduce);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Reduce_scatter);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Scan);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Type_create_darray);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Type_commit);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_open);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_close);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_sync);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_set_size);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_set_view);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_at);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_at_all);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_all);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_shared);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_ordered);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_at_all_begin);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_all_begin);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_read_ordered_begin);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_iread_at);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_iread);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_iread_shared);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_at);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_at_all);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_all);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_shared);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_ordered);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_at_all_begin);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_all_begin);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_write_ordered_begin);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_iwrite_at);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_iwrite);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_iwrite_shared);
// MPI_Finalize wrapper defined in recorder-mpi-init-finalize.c, not in recorder-mpi.c
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Finalize);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Finalized);
// MPI_Init wrapper defined in recorder-mpi-init-finalize.c, not in recorder-mpi.c
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Init); 
// MPI_Init_thread wrapper defined in recorder-mpi-init-finalize.c, not in recorder-mpi.c
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Init_thread);  
// Added 10 new MPI functinos on 2019/01/07
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Cart_rank);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Cart_create);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Cart_get);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Cart_shift);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Wait);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Send);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Recv);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Sendrecv);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Isend);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Irecv);
// Add MPI_Waitall, MPI_Waitsome, MPI_Waitany, MPI_Ssend on 2020/08/06
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Waitall);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Waitsome);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Waitany);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Ssend);
// Add MPI_Comm_split on 2020/08/17
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_split);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_create);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_dup);
// Add MPI_File_seek and MPI_File_seek_shared on 2020/08/27
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_seek);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_seek_shared);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_File_get_size);
// Add MPI_Test, MPI_Testany, MPI_Testsome, MPI_Testall,
// MPI_Ireduce, MPI_Igather, MPI_Iscatter and MPI_Ialltoall on 2020/12/18
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Test);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Testall);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Testsome);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Testany);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Ireduce);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Igather);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Iscatter);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Ialltoall);
// Add on 2021/01/25
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_free);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Cart_sub);
RECORDER_WRAPPEE_HANDLE_DECL(MPI_Comm_split_type);



/* NOTE: using HDF5 1.8 version */
/* HDF5 Function Calls */
/* File Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Fcreate);
RECORDER_WRAPPEE_HANDLE_DECL(H5Fopen);
RECORDER_WRAPPEE_HANDLE_DECL(H5Fclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Fflush);
/* Group Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Gclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Gcreate1);
RECORDER_WRAPPEE_HANDLE_DECL(H5Gcreate2);
RECORDER_WRAPPEE_HANDLE_DECL(H5Gget_objinfo);
RECORDER_WRAPPEE_HANDLE_DECL(H5Giterate);
RECORDER_WRAPPEE_HANDLE_DECL(H5Gopen1);
RECORDER_WRAPPEE_HANDLE_DECL(H5Gopen2);
/* Dataset Interface  */
RECORDER_WRAPPEE_HANDLE_DECL(H5Dclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dcreate1);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dcreate2);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dget_create_plist);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dget_space);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dget_type);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dopen1);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dopen2);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dread);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dwrite);
RECORDER_WRAPPEE_HANDLE_DECL(H5Dset_extent);
/* Dataspace Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Sclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Screate);
RECORDER_WRAPPEE_HANDLE_DECL(H5Screate_simple);
RECORDER_WRAPPEE_HANDLE_DECL(H5Sget_select_npoints);
RECORDER_WRAPPEE_HANDLE_DECL(H5Sget_simple_extent_dims);
RECORDER_WRAPPEE_HANDLE_DECL(H5Sget_simple_extent_npoints);
RECORDER_WRAPPEE_HANDLE_DECL(H5Sselect_elements);
RECORDER_WRAPPEE_HANDLE_DECL(H5Sselect_hyperslab);
RECORDER_WRAPPEE_HANDLE_DECL(H5Sselect_none);
/* Datatype Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Tclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Tcopy);
RECORDER_WRAPPEE_HANDLE_DECL(H5Tget_class);
RECORDER_WRAPPEE_HANDLE_DECL(H5Tget_size);
RECORDER_WRAPPEE_HANDLE_DECL(H5Tset_size);
RECORDER_WRAPPEE_HANDLE_DECL(H5Tcreate);
RECORDER_WRAPPEE_HANDLE_DECL(H5Tinsert);
/* Attribute Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Aclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Acreate1);
RECORDER_WRAPPEE_HANDLE_DECL(H5Acreate2);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aget_name);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aget_num_attrs);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aget_space);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aget_type);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aopen);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aopen_idx);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aopen_name);
RECORDER_WRAPPEE_HANDLE_DECL(H5Aread);
RECORDER_WRAPPEE_HANDLE_DECL(H5Awrite);
/* Property List Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Pclose);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pcreate);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pget_chunk);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pget_mdc_config);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_alignment);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_chunk);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_dxpl_mpio);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_fapl_core);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_fapl_mpio);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_fapl_mpiposix);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_istore_k);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_mdc_config);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_meta_block_size);
/* Link Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Lexists);
RECORDER_WRAPPEE_HANDLE_DECL(H5Lget_val);
RECORDER_WRAPPEE_HANDLE_DECL(H5Literate);
/* H5Literate1 H5Literate2 don't have wrapper definition */
// RECORDER_WRAPPEE_HANDLE_DECL(H5Literate1);
// RECORDER_WRAPPEE_HANDLE_DECL(H5Literate2);
/* Object Interface */
RECORDER_WRAPPEE_HANDLE_DECL(H5Oclose);
/* H5Oget_info H5Oget_info_by_name don't have wrapper definition because of version inconsistency */
// RECORDER_WRAPPEE_HANDLE_DECL(H5Oget_info);
// RECORDER_WRAPPEE_HANDLE_DECL(H5Oget_info_by_name);
RECORDER_WRAPPEE_HANDLE_DECL(H5Oopen);
/* Collective Metadata */
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_coll_metadata_write);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pget_coll_metadata_write);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pset_all_coll_metadata_ops);
RECORDER_WRAPPEE_HANDLE_DECL(H5Pget_all_coll_metadata_ops);


struct gotcha_binding_t recorder_wrappers[] = {
    /* POSIX I/O */
    { "close", RECORDER_POSIX_DECL(close), &RECORDER_WRAPPEE_HANDLE(close) },
    { "fclose", RECORDER_POSIX_DECL(fclose), &RECORDER_WRAPPEE_HANDLE(fclose) },
    { "fsync", RECORDER_POSIX_DECL(fsync), &RECORDER_WRAPPEE_HANDLE(fsync) },
    { "fdatasync", RECORDER_POSIX_DECL(fdatasync), &RECORDER_WRAPPEE_HANDLE(fdatasync) },
    { "mmap64", RECORDER_POSIX_DECL(mmap64), &RECORDER_WRAPPEE_HANDLE(mmap64) },
    { "mmap", RECORDER_POSIX_DECL(mmap), &RECORDER_WRAPPEE_HANDLE(mmap) },
    { "msync", RECORDER_POSIX_DECL(msync), &RECORDER_WRAPPEE_HANDLE(msync) },
    { "creat", RECORDER_POSIX_DECL(creat), &RECORDER_WRAPPEE_HANDLE(creat) },
    { "creat64", RECORDER_POSIX_DECL(creat64), &RECORDER_WRAPPEE_HANDLE(creat64) },
    { "open64", RECORDER_POSIX_DECL(open64), &RECORDER_WRAPPEE_HANDLE(open64) },
    { "open", RECORDER_POSIX_DECL(open), &RECORDER_WRAPPEE_HANDLE(open) },
    { "fopen64", RECORDER_POSIX_DECL(fopen64), &RECORDER_WRAPPEE_HANDLE(fopen64) },
    { "fopen", RECORDER_POSIX_DECL(fopen), &RECORDER_WRAPPEE_HANDLE(fopen) },
    { "__xstat", RECORDER_POSIX_DECL(__xstat), &RECORDER_WRAPPEE_HANDLE(__xstat) },
    { "__xstat64", RECORDER_POSIX_DECL(__xstat64), &RECORDER_WRAPPEE_HANDLE(__xstat64) },
    { "__lxstat", RECORDER_POSIX_DECL(__lxstat), &RECORDER_WRAPPEE_HANDLE(__lxstat) },
    { "__lxstat64", RECORDER_POSIX_DECL(__lxstat64), &RECORDER_WRAPPEE_HANDLE(__lxstat64) },
    { "__fxstat", RECORDER_POSIX_DECL(__fxstat), &RECORDER_WRAPPEE_HANDLE(__fxstat) },
    { "__fxstat64", RECORDER_POSIX_DECL(__fxstat64), &RECORDER_WRAPPEE_HANDLE(__fxstat64) },
    { "pread64", RECORDER_POSIX_DECL(pread64), &RECORDER_WRAPPEE_HANDLE(pread64) },
    { "pread", RECORDER_POSIX_DECL(pread), &RECORDER_WRAPPEE_HANDLE(pread) },
    { "pwrite64", RECORDER_POSIX_DECL(pwrite64), &RECORDER_WRAPPEE_HANDLE(pwrite64) },
    { "pwrite", RECORDER_POSIX_DECL(pwrite), &RECORDER_WRAPPEE_HANDLE(pwrite) },
    { "readv", RECORDER_POSIX_DECL(readv), &RECORDER_WRAPPEE_HANDLE(readv) },
    { "writev", RECORDER_POSIX_DECL(writev), &RECORDER_WRAPPEE_HANDLE(writev) },
    { "fread", RECORDER_POSIX_DECL(fread), &RECORDER_WRAPPEE_HANDLE(fread) },
    { "fwrite", RECORDER_POSIX_DECL(fwrite), &RECORDER_WRAPPEE_HANDLE(fwrite) },
    { "vfprintf", RECORDER_POSIX_DECL(vfprintf), &RECORDER_WRAPPEE_HANDLE(vfprintf) },
    { "read", RECORDER_POSIX_DECL(read), &RECORDER_WRAPPEE_HANDLE(read) },
    { "write", RECORDER_POSIX_DECL(write), &RECORDER_WRAPPEE_HANDLE(write) },
    { "fseek", RECORDER_POSIX_DECL(fseek), &RECORDER_WRAPPEE_HANDLE(fseek) },
    { "ftell", RECORDER_POSIX_DECL(ftell), &RECORDER_WRAPPEE_HANDLE(ftell) },
    { "lseek64", RECORDER_POSIX_DECL(lseek64), &RECORDER_WRAPPEE_HANDLE(lseek64) },
    { "lseek", RECORDER_POSIX_DECL(lseek), &RECORDER_WRAPPEE_HANDLE(lseek) },
    { "getcwd", RECORDER_POSIX_DECL(getcwd), &RECORDER_WRAPPEE_HANDLE(getcwd) },
    { "mkdir", RECORDER_POSIX_DECL(mkdir), &RECORDER_WRAPPEE_HANDLE(mkdir) },
    { "rmdir", RECORDER_POSIX_DECL(rmdir), &RECORDER_WRAPPEE_HANDLE(rmdir) },
    { "chdir", RECORDER_POSIX_DECL(chdir), &RECORDER_WRAPPEE_HANDLE(chdir) },
    { "link", RECORDER_POSIX_DECL(link), &RECORDER_WRAPPEE_HANDLE(link) },
    { "linkat", RECORDER_POSIX_DECL(linkat), &RECORDER_WRAPPEE_HANDLE(linkat) },
    { "unlink", RECORDER_POSIX_DECL(unlink), &RECORDER_WRAPPEE_HANDLE(unlink) },
    { "symlink", RECORDER_POSIX_DECL(symlink), &RECORDER_WRAPPEE_HANDLE(symlink) },
    { "symlinkat", RECORDER_POSIX_DECL(symlinkat), &RECORDER_WRAPPEE_HANDLE(symlinkat) },
    { "readlink", RECORDER_POSIX_DECL(readlink), &RECORDER_WRAPPEE_HANDLE(readlink) },
    { "readlinkat", RECORDER_POSIX_DECL(readlinkat), &RECORDER_WRAPPEE_HANDLE(readlinkat) },
    { "rename", RECORDER_POSIX_DECL(rename), &RECORDER_WRAPPEE_HANDLE(rename) },
    { "chmod", RECORDER_POSIX_DECL(chmod), &RECORDER_WRAPPEE_HANDLE(chmod) },
    { "chown", RECORDER_POSIX_DECL(chown), &RECORDER_WRAPPEE_HANDLE(chown) },
    { "lchown", RECORDER_POSIX_DECL(lchown), &RECORDER_WRAPPEE_HANDLE(lchown) },
    { "utime", RECORDER_POSIX_DECL(utime), &RECORDER_WRAPPEE_HANDLE(utime) },
    { "opendir", RECORDER_POSIX_DECL(opendir), &RECORDER_WRAPPEE_HANDLE(opendir) },
    { "readdir", RECORDER_POSIX_DECL(readdir), &RECORDER_WRAPPEE_HANDLE(readdir) },
    { "closedir", RECORDER_POSIX_DECL(closedir), &RECORDER_WRAPPEE_HANDLE(closedir) },
    // { "rewinddir", RECORDER_POSIX_DECL(rewinddir), &RECORDER_WRAPPEE_HANDLE(rewinddir) },
    { "mknod", RECORDER_POSIX_DECL(mknod), &RECORDER_WRAPPEE_HANDLE(mknod) },
    { "mknodat", RECORDER_POSIX_DECL(mknodat), &RECORDER_WRAPPEE_HANDLE(mknodat) },
#ifndef DISABLE_FCNTL_TRACE
    { "fcntl", RECORDER_POSIX_DECL(fcntl), &RECORDER_WRAPPEE_HANDLE(fcntl) },
#endif
    { "dup", RECORDER_POSIX_DECL(dup), &RECORDER_WRAPPEE_HANDLE(dup) },
    { "dup2", RECORDER_POSIX_DECL(dup2), &RECORDER_WRAPPEE_HANDLE(dup2) },
    { "pipe", RECORDER_POSIX_DECL(pipe), &RECORDER_WRAPPEE_HANDLE(pipe) },
    { "mkfifo", RECORDER_POSIX_DECL(mkfifo), &RECORDER_WRAPPEE_HANDLE(mkfifo) },
    { "umask", RECORDER_POSIX_DECL(umask), &RECORDER_WRAPPEE_HANDLE(umask) },
    { "fdopen", RECORDER_POSIX_DECL(fdopen), &RECORDER_WRAPPEE_HANDLE(fdopen) },
    { "fileno", RECORDER_POSIX_DECL(fileno), &RECORDER_WRAPPEE_HANDLE(fileno) },
    { "access", RECORDER_POSIX_DECL(access), &RECORDER_WRAPPEE_HANDLE(access) },
    { "faccessat", RECORDER_POSIX_DECL(faccessat), &RECORDER_WRAPPEE_HANDLE(faccessat) },
    { "tmpfile", RECORDER_POSIX_DECL(tmpfile), &RECORDER_WRAPPEE_HANDLE(tmpfile) },
    { "remove", RECORDER_POSIX_DECL(remove), &RECORDER_WRAPPEE_HANDLE(remove) },
    { "truncate", RECORDER_POSIX_DECL(truncate), &RECORDER_WRAPPEE_HANDLE(truncate) },
    { "ftruncate", RECORDER_POSIX_DECL(ftruncate), &RECORDER_WRAPPEE_HANDLE(ftruncate) },
    { "fseeko", RECORDER_POSIX_DECL(fseeko), &RECORDER_WRAPPEE_HANDLE(fseeko) },
    { "ftello", RECORDER_POSIX_DECL(ftello), &RECORDER_WRAPPEE_HANDLE(ftello) },
    
    /* MPI Function Calls */
    { "MPI_Comm_size", RECORDER_MPI_DECL(MPI_Comm_size), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_size) },
    { "MPI_Comm_rank", RECORDER_MPI_DECL(MPI_Comm_rank), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_rank) },
    { "MPI_Get_processor_name", RECORDER_MPI_DECL(MPI_Get_processor_name), &RECORDER_WRAPPEE_HANDLE(MPI_Get_processor_name) },
    { "MPI_Comm_set_errhandler", RECORDER_MPI_DECL(MPI_Comm_set_errhandler), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_set_errhandler) },
    { "MPI_Barrier", RECORDER_MPI_DECL(MPI_Barrier), &RECORDER_WRAPPEE_HANDLE(MPI_Barrier) },
    { "MPI_Bcast", RECORDER_MPI_DECL(MPI_Bcast), &RECORDER_WRAPPEE_HANDLE(MPI_Bcast) },
    { "MPI_Ibcast", RECORDER_MPI_DECL(MPI_Ibcast), &RECORDER_WRAPPEE_HANDLE(MPI_Ibcast) },
    { "MPI_Gather", RECORDER_MPI_DECL(MPI_Gather), &RECORDER_WRAPPEE_HANDLE(MPI_Gather) },
    { "MPI_Scatter", RECORDER_MPI_DECL(MPI_Scatter), &RECORDER_WRAPPEE_HANDLE(MPI_Scatter) },
    { "MPI_Gatherv", RECORDER_MPI_DECL(MPI_Gatherv), &RECORDER_WRAPPEE_HANDLE(MPI_Gatherv) },
    { "MPI_Scatterv", RECORDER_MPI_DECL(MPI_Scatterv), &RECORDER_WRAPPEE_HANDLE(MPI_Scatterv) },
    { "MPI_Allgather", RECORDER_MPI_DECL(MPI_Allgather), &RECORDER_WRAPPEE_HANDLE(MPI_Allgather) },
    { "MPI_Allgatherv", RECORDER_MPI_DECL(MPI_Allgatherv), &RECORDER_WRAPPEE_HANDLE(MPI_Allgatherv) },
    { "MPI_Alltoall", RECORDER_MPI_DECL(MPI_Alltoall), &RECORDER_WRAPPEE_HANDLE(MPI_Alltoall) },
    { "MPI_Reduce", RECORDER_MPI_DECL(MPI_Reduce), &RECORDER_WRAPPEE_HANDLE(MPI_Reduce) },
    { "MPI_Allreduce", RECORDER_MPI_DECL(MPI_Allreduce), &RECORDER_WRAPPEE_HANDLE(MPI_Allreduce) },
    { "MPI_Reduce_scatter", RECORDER_MPI_DECL(MPI_Reduce_scatter), &RECORDER_WRAPPEE_HANDLE(MPI_Reduce_scatter) },
    { "MPI_Scan", RECORDER_MPI_DECL(MPI_Scan), &RECORDER_WRAPPEE_HANDLE(MPI_Scan) },
    { "MPI_Type_create_darray", RECORDER_MPI_DECL(MPI_Type_create_darray), &RECORDER_WRAPPEE_HANDLE(MPI_Type_create_darray) },
    { "MPI_Type_commit", RECORDER_MPI_DECL(MPI_Type_commit), &RECORDER_WRAPPEE_HANDLE(MPI_Type_commit) },
    { "MPI_File_open", RECORDER_MPI_DECL(MPI_File_open), &RECORDER_WRAPPEE_HANDLE(MPI_File_open) },
    { "MPI_File_close", RECORDER_MPI_DECL(MPI_File_close), &RECORDER_WRAPPEE_HANDLE(MPI_File_close) },
    { "MPI_File_sync", RECORDER_MPI_DECL(MPI_File_sync), &RECORDER_WRAPPEE_HANDLE(MPI_File_sync) },
    { "MPI_File_set_size", RECORDER_MPI_DECL(MPI_File_set_size), &RECORDER_WRAPPEE_HANDLE(MPI_File_set_size) },
    { "MPI_File_set_view", RECORDER_MPI_DECL(MPI_File_set_view), &RECORDER_WRAPPEE_HANDLE(MPI_File_set_view) },
    { "MPI_File_read", RECORDER_MPI_DECL(MPI_File_read), &RECORDER_WRAPPEE_HANDLE(MPI_File_read) },
    { "MPI_File_read_at", RECORDER_MPI_DECL(MPI_File_read_at), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_at) },
    { "MPI_File_read_at_all", RECORDER_MPI_DECL(MPI_File_read_at_all), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_at_all) },
    { "MPI_File_read_all", RECORDER_MPI_DECL(MPI_File_read_all), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_all) },
    { "MPI_File_read_shared", RECORDER_MPI_DECL(MPI_File_read_shared), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_shared) },
    { "MPI_File_read_ordered", RECORDER_MPI_DECL(MPI_File_read_ordered), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_ordered) },
    { "MPI_File_read_at_all_begin", RECORDER_MPI_DECL(MPI_File_read_at_all_begin), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_at_all_begin) },
    { "MPI_File_read_all_begin", RECORDER_MPI_DECL(MPI_File_read_all_begin), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_all_begin) },
    { "MPI_File_read_ordered_begin", RECORDER_MPI_DECL(MPI_File_read_ordered_begin), &RECORDER_WRAPPEE_HANDLE(MPI_File_read_ordered_begin) },
    { "MPI_File_iread_at", RECORDER_MPI_DECL(MPI_File_iread_at), &RECORDER_WRAPPEE_HANDLE(MPI_File_iread_at) },
    { "MPI_File_iread", RECORDER_MPI_DECL(MPI_File_iread), &RECORDER_WRAPPEE_HANDLE(MPI_File_iread) },
    { "MPI_File_iread_shared", RECORDER_MPI_DECL(MPI_File_iread_shared), &RECORDER_WRAPPEE_HANDLE(MPI_File_iread_shared) },
    { "MPI_File_write", RECORDER_MPI_DECL(MPI_File_write), &RECORDER_WRAPPEE_HANDLE(MPI_File_write) },
    { "MPI_File_write_at", RECORDER_MPI_DECL(MPI_File_write_at), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_at) },
    { "MPI_File_write_at_all", RECORDER_MPI_DECL(MPI_File_write_at_all), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_at_all) },
    { "MPI_File_write_all", RECORDER_MPI_DECL(MPI_File_write_all), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_all) },
    { "MPI_File_write_shared", RECORDER_MPI_DECL(MPI_File_write_shared), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_shared) },
    { "MPI_File_write_ordered", RECORDER_MPI_DECL(MPI_File_write_ordered), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_ordered) },
    { "MPI_File_write_at_all_begin", RECORDER_MPI_DECL(MPI_File_write_at_all_begin), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_at_all_begin) },
    { "MPI_File_write_all_begin", RECORDER_MPI_DECL(MPI_File_write_all_begin), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_all_begin) },
    { "MPI_File_write_ordered_begin", RECORDER_MPI_DECL(MPI_File_write_ordered_begin), &RECORDER_WRAPPEE_HANDLE(MPI_File_write_ordered_begin) },
    { "MPI_File_iwrite_at", RECORDER_MPI_DECL(MPI_File_iwrite_at), &RECORDER_WRAPPEE_HANDLE(MPI_File_iwrite_at) },
    { "MPI_File_iwrite", RECORDER_MPI_DECL(MPI_File_iwrite), &RECORDER_WRAPPEE_HANDLE(MPI_File_iwrite) },
    { "MPI_File_iwrite_shared", RECORDER_MPI_DECL(MPI_File_iwrite_shared), &RECORDER_WRAPPEE_HANDLE(MPI_File_iwrite_shared) },
    { "MPI_Finalize", RECORDER_MPI_DECL(MPI_Finalize), &RECORDER_WRAPPEE_HANDLE(MPI_Finalize) },
    { "MPI_Finalized", RECORDER_MPI_DECL(MPI_Finalized), &RECORDER_WRAPPEE_HANDLE(MPI_Finalized) },
    { "MPI_Init", RECORDER_MPI_DECL(MPI_Init), &RECORDER_WRAPPEE_HANDLE(MPI_Init) },
    { "MPI_Init_thread", RECORDER_MPI_DECL(MPI_Init_thread), &RECORDER_WRAPPEE_HANDLE(MPI_Init_thread) },
    { "MPI_Cart_rank", RECORDER_MPI_DECL(MPI_Cart_rank), &RECORDER_WRAPPEE_HANDLE(MPI_Cart_rank) },
    { "MPI_Cart_create", RECORDER_MPI_DECL(MPI_Cart_create), &RECORDER_WRAPPEE_HANDLE(MPI_Cart_create) },
    { "MPI_Cart_get", RECORDER_MPI_DECL(MPI_Cart_get), &RECORDER_WRAPPEE_HANDLE(MPI_Cart_get) },
    { "MPI_Cart_shift", RECORDER_MPI_DECL(MPI_Cart_shift), &RECORDER_WRAPPEE_HANDLE(MPI_Cart_shift) },
    { "MPI_Wait", RECORDER_MPI_DECL(MPI_Wait), &RECORDER_WRAPPEE_HANDLE(MPI_Wait) },
    { "MPI_Send", RECORDER_MPI_DECL(MPI_Send), &RECORDER_WRAPPEE_HANDLE(MPI_Send) },
    { "MPI_Recv", RECORDER_MPI_DECL(MPI_Recv), &RECORDER_WRAPPEE_HANDLE(MPI_Recv) },
    { "MPI_Sendrecv", RECORDER_MPI_DECL(MPI_Sendrecv), &RECORDER_WRAPPEE_HANDLE(MPI_Sendrecv) },
    { "MPI_Isend", RECORDER_MPI_DECL(MPI_Isend), &RECORDER_WRAPPEE_HANDLE(MPI_Isend) },
    { "MPI_Irecv", RECORDER_MPI_DECL(MPI_Irecv), &RECORDER_WRAPPEE_HANDLE(MPI_Irecv) },
    { "MPI_Waitall", RECORDER_MPI_DECL(MPI_Waitall), &RECORDER_WRAPPEE_HANDLE(MPI_Waitall) },
    { "MPI_Waitsome", RECORDER_MPI_DECL(MPI_Waitsome), &RECORDER_WRAPPEE_HANDLE(MPI_Waitsome) },
    { "MPI_Waitany", RECORDER_MPI_DECL(MPI_Waitany), &RECORDER_WRAPPEE_HANDLE(MPI_Waitany) },
    { "MPI_Ssend", RECORDER_MPI_DECL(MPI_Ssend), &RECORDER_WRAPPEE_HANDLE(MPI_Ssend) },
    { "MPI_Comm_split", RECORDER_MPI_DECL(MPI_Comm_split), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_split) },
    { "MPI_Comm_create", RECORDER_MPI_DECL(MPI_Comm_create), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_create) },
    { "MPI_Comm_dup", RECORDER_MPI_DECL(MPI_Comm_dup), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_dup) },
    { "MPI_File_seek", RECORDER_MPI_DECL(MPI_File_seek), &RECORDER_WRAPPEE_HANDLE(MPI_File_seek) },
    { "MPI_File_seek_shared", RECORDER_MPI_DECL(MPI_File_seek_shared), &RECORDER_WRAPPEE_HANDLE(MPI_File_seek_shared) },
    { "MPI_File_get_size", RECORDER_MPI_DECL(MPI_File_get_size), &RECORDER_WRAPPEE_HANDLE(MPI_File_get_size) },
    { "MPI_Test", RECORDER_MPI_DECL(MPI_Test), &RECORDER_WRAPPEE_HANDLE(MPI_Test) },
    { "MPI_Testall", RECORDER_MPI_DECL(MPI_Testall), &RECORDER_WRAPPEE_HANDLE(MPI_Testall) },
    { "MPI_Testsome", RECORDER_MPI_DECL(MPI_Testsome), &RECORDER_WRAPPEE_HANDLE(MPI_Testsome) },
    { "MPI_Testany", RECORDER_MPI_DECL(MPI_Testany), &RECORDER_WRAPPEE_HANDLE(MPI_Testany) },
    { "MPI_Ireduce", RECORDER_MPI_DECL(MPI_Ireduce), &RECORDER_WRAPPEE_HANDLE(MPI_Ireduce) },
    { "MPI_Igather", RECORDER_MPI_DECL(MPI_Igather), &RECORDER_WRAPPEE_HANDLE(MPI_Igather) },
    { "MPI_Iscatter", RECORDER_MPI_DECL(MPI_Iscatter), &RECORDER_WRAPPEE_HANDLE(MPI_Iscatter) },
    { "MPI_Ialltoall", RECORDER_MPI_DECL(MPI_Ialltoall), &RECORDER_WRAPPEE_HANDLE(MPI_Ialltoall) },
    { "MPI_Comm_free", RECORDER_MPI_DECL(MPI_Comm_free), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_free) },
    { "MPI_Cart_sub", RECORDER_MPI_DECL(MPI_Cart_sub), &RECORDER_WRAPPEE_HANDLE(MPI_Cart_sub) },
    { "MPI_Comm_split_type", RECORDER_MPI_DECL(MPI_Comm_split_type), &RECORDER_WRAPPEE_HANDLE(MPI_Comm_split_type) },

    /* HDF5 Function Calls */
    /* NOTE: using HDF5 1.8 version */
    { "H5Fcreate", RECORDER_HDF5_DECL(H5Fcreate), &RECORDER_WRAPPEE_HANDLE(H5Fcreate) },
    { "H5Fopen", RECORDER_HDF5_DECL(H5Fopen), &RECORDER_WRAPPEE_HANDLE(H5Fopen) },
    { "H5Fclose", RECORDER_HDF5_DECL(H5Fclose), &RECORDER_WRAPPEE_HANDLE(H5Fclose) },
    { "H5Fflush", RECORDER_HDF5_DECL(H5Fflush), &RECORDER_WRAPPEE_HANDLE(H5Fflush) },
    { "H5Gclose", RECORDER_HDF5_DECL(H5Gclose), &RECORDER_WRAPPEE_HANDLE(H5Gclose) },
    { "H5Gcreate1", RECORDER_HDF5_DECL(H5Gcreate1), &RECORDER_WRAPPEE_HANDLE(H5Gcreate1) },
    { "H5Gcreate2", RECORDER_HDF5_DECL(H5Gcreate2), &RECORDER_WRAPPEE_HANDLE(H5Gcreate2) },
    { "H5Gget_objinfo", RECORDER_HDF5_DECL(H5Gget_objinfo), &RECORDER_WRAPPEE_HANDLE(H5Gget_objinfo) },
    { "H5Giterate", RECORDER_HDF5_DECL(H5Giterate), &RECORDER_WRAPPEE_HANDLE(H5Giterate) },
    { "H5Gopen1", RECORDER_HDF5_DECL(H5Gopen1), &RECORDER_WRAPPEE_HANDLE(H5Gopen1) },
    { "H5Gopen2", RECORDER_HDF5_DECL(H5Gopen2), &RECORDER_WRAPPEE_HANDLE(H5Gopen2) },
    { "H5Dclose", RECORDER_HDF5_DECL(H5Dclose), &RECORDER_WRAPPEE_HANDLE(H5Dclose) },
    { "H5Dcreate1", RECORDER_HDF5_DECL(H5Dcreate1), &RECORDER_WRAPPEE_HANDLE(H5Dcreate1) },
    { "H5Dcreate2", RECORDER_HDF5_DECL(H5Dcreate2), &RECORDER_WRAPPEE_HANDLE(H5Dcreate2) },
    { "H5Dget_create_plist", RECORDER_HDF5_DECL(H5Dget_create_plist), &RECORDER_WRAPPEE_HANDLE(H5Dget_create_plist) },
    { "H5Dget_space", RECORDER_HDF5_DECL(H5Dget_space), &RECORDER_WRAPPEE_HANDLE(H5Dget_space) },
    { "H5Dget_type", RECORDER_HDF5_DECL(H5Dget_type), &RECORDER_WRAPPEE_HANDLE(H5Dget_type) },
    { "H5Dopen1", RECORDER_HDF5_DECL(H5Dopen1), &RECORDER_WRAPPEE_HANDLE(H5Dopen1) },
    { "H5Dopen2", RECORDER_HDF5_DECL(H5Dopen2), &RECORDER_WRAPPEE_HANDLE(H5Dopen2) },
    { "H5Dread", RECORDER_HDF5_DECL(H5Dread), &RECORDER_WRAPPEE_HANDLE(H5Dread) },
    { "H5Dwrite", RECORDER_HDF5_DECL(H5Dwrite), &RECORDER_WRAPPEE_HANDLE(H5Dwrite) },
    { "H5Dset_extent", RECORDER_HDF5_DECL(H5Dset_extent), &RECORDER_WRAPPEE_HANDLE(H5Dset_extent) },
    { "H5Sclose", RECORDER_HDF5_DECL(H5Sclose), &RECORDER_WRAPPEE_HANDLE(H5Sclose) },
    { "H5Screate", RECORDER_HDF5_DECL(H5Screate), &RECORDER_WRAPPEE_HANDLE(H5Screate) },
    { "H5Screate_simple", RECORDER_HDF5_DECL(H5Screate_simple), &RECORDER_WRAPPEE_HANDLE(H5Screate_simple) },
    { "H5Sget_select_npoints", RECORDER_HDF5_DECL(H5Sget_select_npoints), &RECORDER_WRAPPEE_HANDLE(H5Sget_select_npoints) },
    { "H5Sget_simple_extent_dims", RECORDER_HDF5_DECL(H5Sget_simple_extent_dims), &RECORDER_WRAPPEE_HANDLE(H5Sget_simple_extent_dims) },
    { "H5Sget_simple_extent_npoints", RECORDER_HDF5_DECL(H5Sget_simple_extent_npoints), &RECORDER_WRAPPEE_HANDLE(aaaaaaaaaa) },
    { "H5Sselect_elements", RECORDER_HDF5_DECL(H5Sselect_elements), &RECORDER_WRAPPEE_HANDLE(H5Sselect_elements) },
    { "H5Sselect_hyperslab", RECORDER_HDF5_DECL(H5Sselect_hyperslab), &RECORDER_WRAPPEE_HANDLE(H5Sselect_hyperslab) },
    { "H5Sselect_none", RECORDER_HDF5_DECL(H5Sselect_none), &RECORDER_WRAPPEE_HANDLE(H5Sselect_none) },
    { "H5Tclose", RECORDER_HDF5_DECL(H5Tclose), &RECORDER_WRAPPEE_HANDLE(H5Tclose) },
    { "H5Tcopy", RECORDER_HDF5_DECL(H5Tcopy), &RECORDER_WRAPPEE_HANDLE(H5Tcopy) },
    { "H5Tget_class", RECORDER_HDF5_DECL(H5Tget_class), &RECORDER_WRAPPEE_HANDLE(H5Tget_class) },
    { "H5Tget_size", RECORDER_HDF5_DECL(H5Tget_size), &RECORDER_WRAPPEE_HANDLE(H5Tget_size) },
    { "H5Tset_size", RECORDER_HDF5_DECL(H5Tset_size), &RECORDER_WRAPPEE_HANDLE(H5Tset_size) },
    { "H5Tcreate", RECORDER_HDF5_DECL(H5Tcreate), &RECORDER_WRAPPEE_HANDLE(H5Tcreate) },
    { "H5Tinsert", RECORDER_HDF5_DECL(H5Tinsert), &RECORDER_WRAPPEE_HANDLE(H5Tinsert) },
    { "H5Aclose", RECORDER_HDF5_DECL(H5Aclose), &RECORDER_WRAPPEE_HANDLE(H5Aclose) },
    { "H5Acreate1", RECORDER_HDF5_DECL(H5Acreate1), &RECORDER_WRAPPEE_HANDLE(H5Acreate1) },
    { "H5Acreate2", RECORDER_HDF5_DECL(H5Acreate2), &RECORDER_WRAPPEE_HANDLE(H5Acreate2) },
    { "H5Aget_name", RECORDER_HDF5_DECL(H5Aget_name), &RECORDER_WRAPPEE_HANDLE(H5Aget_name) },
    { "H5Aget_num_attrs", RECORDER_HDF5_DECL(H5Aget_num_attrs), &RECORDER_WRAPPEE_HANDLE(H5Aget_num_attrs) },
    { "H5Aget_space", RECORDER_HDF5_DECL(H5Aget_space), &RECORDER_WRAPPEE_HANDLE(H5Aget_space) },
    { "H5Aget_type", RECORDER_HDF5_DECL(H5Aget_type), &RECORDER_WRAPPEE_HANDLE(H5Aget_type) },
    { "H5Aopen", RECORDER_HDF5_DECL(H5Aopen), &RECORDER_WRAPPEE_HANDLE(H5Aopen) },
    { "H5Aopen_idx", RECORDER_HDF5_DECL(H5Aopen_idx), &RECORDER_WRAPPEE_HANDLE(H5Aopen_idx) },
    { "H5Aopen_name", RECORDER_HDF5_DECL(H5Aopen_name), &RECORDER_WRAPPEE_HANDLE(H5Aopen_name) },
    { "H5Aread", RECORDER_HDF5_DECL(H5Aread), &RECORDER_WRAPPEE_HANDLE(H5Aread) },
    { "H5Awrite", RECORDER_HDF5_DECL(H5Awrite), &RECORDER_WRAPPEE_HANDLE(H5Awrite) },
    { "H5Pclose", RECORDER_HDF5_DECL(H5Pclose), &RECORDER_WRAPPEE_HANDLE(H5Pclose) },
    { "H5Pcreate", RECORDER_HDF5_DECL(H5Pcreate), &RECORDER_WRAPPEE_HANDLE(H5Pcreate) },
    { "H5Pget_chunk", RECORDER_HDF5_DECL(H5Pget_chunk), &RECORDER_WRAPPEE_HANDLE(H5Pget_chunk) },
    { "H5Pget_mdc_config", RECORDER_HDF5_DECL(H5Pget_mdc_config), &RECORDER_WRAPPEE_HANDLE(H5Pget_mdc_config) },
    { "H5Pset_alignment", RECORDER_HDF5_DECL(H5Pset_alignment), &RECORDER_WRAPPEE_HANDLE(H5Pset_alignment) },
    { "H5Pset_chunk", RECORDER_HDF5_DECL(H5Pset_chunk), &RECORDER_WRAPPEE_HANDLE(H5Pset_chunk) },
    { "H5Pset_dxpl_mpio", RECORDER_HDF5_DECL(H5Pset_dxpl_mpio), &RECORDER_WRAPPEE_HANDLE(H5Pset_dxpl_mpio) },
    { "H5Pset_fapl_core", RECORDER_HDF5_DECL(H5Pset_fapl_core), &RECORDER_WRAPPEE_HANDLE(H5Pset_fapl_core) },
    { "H5Pset_fapl_mpio", RECORDER_HDF5_DECL(H5Pset_fapl_mpio), &RECORDER_WRAPPEE_HANDLE(H5Pset_fapl_mpio) },
    { "H5Pset_fapl_mpiposix", RECORDER_HDF5_DECL(H5Pset_fapl_mpiposix), &RECORDER_WRAPPEE_HANDLE(H5Pset_fapl_mpiposix) },
    { "H5Pset_istore_k", RECORDER_HDF5_DECL(H5Pset_istore_k), &RECORDER_WRAPPEE_HANDLE(H5Pset_istore_k) },
    { "H5Pset_mdc_config", RECORDER_HDF5_DECL(H5Pset_mdc_config), &RECORDER_WRAPPEE_HANDLE(H5Pset_mdc_config) },
    { "H5Pset_meta_block_size", RECORDER_HDF5_DECL(H5Pset_meta_block_size), &RECORDER_WRAPPEE_HANDLE(H5Pset_meta_block_size) },
    { "H5Lexists", RECORDER_HDF5_DECL(H5Lexists), &RECORDER_WRAPPEE_HANDLE(H5Lexists) },
    { "H5Lget_val", RECORDER_HDF5_DECL(H5Lget_val), &RECORDER_WRAPPEE_HANDLE(H5Lget_val) },
    { "H5Literate", RECORDER_HDF5_DECL(H5Literate), &RECORDER_WRAPPEE_HANDLE(H5Literate) },
    /* H5Literate1 H5Literate2 don't have wrapper definition */
    // { "H5Literate1", RECORDER_HDF5_DECL(H5Literate1), &RECORDER_WRAPPEE_HANDLE(H5Literate1) },
    // { "H5Literate2", RECORDER_HDF5_DECL(H5Literate2), &RECORDER_WRAPPEE_HANDLE(H5Literate2) },
    { "H5Oclose", RECORDER_HDF5_DECL(H5Oclose), &RECORDER_WRAPPEE_HANDLE(H5Oclose) },
    /* H5Oget_info H5Oget_info_by_name don't have wrapper definition because of version inconsistency */
    // { "H5Oget_info", RECORDER_HDF5_DECL(H5Oget_info), &RECORDER_WRAPPEE_HANDLE(H5Oget_info) },
    // { "H5Oget_info_by_name", RECORDER_HDF5_DECL(H5Oget_info_by_name), &RECORDER_WRAPPEE_HANDLE(H5Oget_info_by_name) },
    { "H5Oopen", RECORDER_HDF5_DECL(H5Oopen), &RECORDER_WRAPPEE_HANDLE(H5Oopen) },
    { "H5Pset_coll_metadata_write", RECORDER_HDF5_DECL(H5Pset_coll_metadata_write), &RECORDER_WRAPPEE_HANDLE(H5Pset_coll_metadata_write) },
    { "H5Pget_coll_metadata_write", RECORDER_HDF5_DECL(H5Pget_coll_metadata_write), &RECORDER_WRAPPEE_HANDLE(H5Pget_coll_metadata_write) },
    { "H5Pset_all_coll_metadata_ops", RECORDER_HDF5_DECL(H5Pset_all_coll_metadata_ops), &RECORDER_WRAPPEE_HANDLE(H5Pset_all_coll_metadata_ops) },
    { "H5Pget_all_coll_metadata_ops", RECORDER_HDF5_DECL(H5Pget_all_coll_metadata_ops), &RECORDER_WRAPPEE_HANDLE(H5Pget_all_coll_metadata_ops) },

};

#define GOTCHA_NFUNCS (sizeof(recorder_wrappers) / sizeof(gotcha_binding_t))

int setup_gotcha_wrappers(void)
{
    /* insert our I/O wrappers using gotcha */
    enum gotcha_error_t result;
    result = gotcha_wrap(recorder_wrappers, GOTCHA_NFUNCS, "recorder");
    if (result != GOTCHA_SUCCESS) {
        LOGERR("gotcha_wrap() returned %d", (int) result);
        if (result == GOTCHA_FUNCTION_NOT_FOUND) {
            /* one or more functions were not found */
            void* fn;
            gotcha_wrappee_handle_t* hdlptr;
            for (int i = 0; i < GOTCHA_NFUNCS; i++) {
                hdlptr = recorder_wrappers[i].function_handle;
                fn = gotcha_get_wrappee(*hdlptr);
                if (NULL == fn) {
                    LOGWARN("Gotcha failed to wrap function '%s'",
                            recorder_wrappers[i].name);
                }
            }
        } else {
            return -1;
        }
    }
    return 0;
}
#endif /* RECORDER_GOTCHA */