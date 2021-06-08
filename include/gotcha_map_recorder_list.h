#ifdef RECORDER_GOTCHA
#include <gotcha/gotcha.h>

/* define gotcha-specific state to use with our wrapper */
#define RECORDER_WRAPPEE_HANDLE_DECL(func) \
gotcha_wrappee_handle_t wrappee_handle_##func

#define RECORDER_WRAPPEE_HANDLE(func) wrappee_handle_##func

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

struct gotcha_binding_t *recorder_wrappers;

int setup_gotcha_wrappers(void);
#endif /* RECORDER_GOTCHA */