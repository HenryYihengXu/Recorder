#ifdef RECORDER_GOTCHA

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include "mpi.h"
#include "recorder.h"
#include "hdf5.h"

hid_t RECORDER_HDF5_DECL(H5Fcreate)(const char *filename, unsigned flags, hid_t create_plist, hid_t access_plist);
hid_t RECORDER_HDF5_DECL(H5Fopen)(const char *filename, unsigned flags, hid_t access_plist);
herr_t RECORDER_HDF5_DECL(H5Fclose)(hid_t file_id);
herr_t RECORDER_HDF5_DECL(H5Fflush)(hid_t object_id, H5F_scope_t scope);
herr_t RECORDER_HDF5_DECL(H5Gclose)(hid_t group_id);
hid_t RECORDER_HDF5_DECL(H5Gcreate1)(hid_t loc_id, const char *name, size_t size_hint);
hid_t RECORDER_HDF5_DECL(H5Gcreate2)(hid_t loc_id, const char *name, hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id);
herr_t RECORDER_HDF5_DECL(H5Gget_objinfo)(hid_t loc_id, const char *name, hbool_t follow_link, H5G_stat_t *statbuf);
int RECORDER_HDF5_DECL(H5Giterate)(hid_t loc_id, const char *name, int *idx, H5G_iterate_t operator, void *operator_data);
hid_t RECORDER_HDF5_DECL(H5Gopen1)(hid_t loc_id, const char *name);
hid_t RECORDER_HDF5_DECL(H5Gopen2)(hid_t loc_id, const char *name, hid_t gapl_id);
herr_t RECORDER_HDF5_DECL(H5Dclose)(hid_t dataset_id);
hid_t RECORDER_HDF5_DECL(H5Dcreate1)(hid_t loc_id, const char *name, hid_t type_id, hid_t space_id, hid_t dcpl_id);
hid_t RECORDER_HDF5_DECL(H5Dcreate2)(hid_t loc_id, const char *name, hid_t dtype_id, hid_t space_id, hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id);
hid_t RECORDER_HDF5_DECL(H5Dget_create_plist)(hid_t dataset_id);
hid_t RECORDER_HDF5_DECL(H5Dget_space)(hid_t dataset_id);
hid_t RECORDER_HDF5_DECL(H5Dget_type)(hid_t dataset_id);
hid_t RECORDER_HDF5_DECL(H5Dopen1)(hid_t loc_id, const char *name);
hid_t RECORDER_HDF5_DECL(H5Dopen2)(hid_t loc_id, const char *name, hid_t dapl_id);
herr_t RECORDER_HDF5_DECL(H5Dread)(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t xfer_plist_id, void *buf);
herr_t RECORDER_HDF5_DECL(H5Dwrite)(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t xfer_plist_id, const void *buf);
herr_t RECORDER_HDF5_DECL(H5Dset_extent)(hid_t dset_id, const hsize_t size[]);
herr_t RECORDER_HDF5_DECL(H5Sclose)(hid_t space_id);
hid_t RECORDER_HDF5_DECL(H5Screate)(H5S_class_t type);
hid_t RECORDER_HDF5_DECL(H5Screate_simple)(int rank, const hsize_t *current_dims, const hsize_t *maximum_dims);
hssize_t RECORDER_HDF5_DECL(H5Sget_select_npoints)(hid_t space_id);
int RECORDER_HDF5_DECL(H5Sget_simple_extent_dims)(hid_t space_id, hsize_t *dims, hsize_t *maxdims);
hssize_t RECORDER_HDF5_DECL(H5Sget_simple_extent_npoints)(hid_t space_id);
herr_t RECORDER_HDF5_DECL(H5Sselect_elements)(hid_t space_id, H5S_seloper_t op, size_t num_elements, const hsize_t *coord);
herr_t RECORDER_HDF5_DECL(H5Sselect_hyperslab)(hid_t space_id, H5S_seloper_t op, const hsize_t *start, const hsize_t *stride, const hsize_t *count, const hsize_t *block);
herr_t RECORDER_HDF5_DECL(H5Sselect_none)(hid_t space_id);
herr_t RECORDER_HDF5_DECL(H5Tclose)(hid_t dtype_id);
hid_t RECORDER_HDF5_DECL(H5Tcopy)(hid_t dtype_id);
H5T_class_t RECORDER_HDF5_DECL(H5Tget_class)(hid_t dtype_id);
size_t RECORDER_HDF5_DECL(H5Tget_size)(hid_t dtype_id);
herr_t RECORDER_HDF5_DECL(H5Tset_size)(hid_t dtype_id, size_t size);
hid_t RECORDER_HDF5_DECL(H5Tcreate)(H5T_class_t class, size_t size);
herr_t RECORDER_HDF5_DECL(H5Tinsert)(hid_t dtype_id, const char *name, size_t offset, hid_t field_id);
herr_t RECORDER_HDF5_DECL(H5Aclose)(hid_t attr_id);
hid_t RECORDER_HDF5_DECL(H5Acreate1)(hid_t loc_id, const char *attr_name, hid_t type_id, hid_t space_id, hid_t acpl_id);
hid_t RECORDER_HDF5_DECL(H5Acreate2)(hid_t loc_id, const char *attr_name, hid_t type_id, hid_t space_id, hid_t acpl_id, hid_t aapl_id);
ssize_t RECORDER_HDF5_DECL(H5Aget_name)(hid_t attr_id, size_t buf_size, char *buf);
int RECORDER_HDF5_DECL(H5Aget_num_attrs)(hid_t loc_id);
hid_t RECORDER_HDF5_DECL(H5Aget_space)(hid_t attr_id);
hid_t RECORDER_HDF5_DECL(H5Aget_type)(hid_t attr_id);
hid_t RECORDER_HDF5_DECL(H5Aopen)(hid_t obj_id, const char *attr_name, hid_t aapl_id);
hid_t RECORDER_HDF5_DECL(H5Aopen_idx)(hid_t loc_id, unsigned int idx);
hid_t RECORDER_HDF5_DECL(H5Aopen_name)(hid_t loc_id, const char *name);
herr_t RECORDER_HDF5_DECL(H5Aread)(hid_t attr_id, hid_t mem_type_id, void *buf);
herr_t RECORDER_HDF5_DECL(H5Awrite)(hid_t attr_id, hid_t mem_type_id, const void *buf);
herr_t RECORDER_HDF5_DECL(H5Pclose)(hid_t plist);
hid_t RECORDER_HDF5_DECL(H5Pcreate)(hid_t cls_id);
int RECORDER_HDF5_DECL(H5Pget_chunk)(hid_t plist, int max_ndims, hsize_t *dims);
herr_t RECORDER_HDF5_DECL(H5Pget_mdc_config)(hid_t plist_id, H5AC_cache_config_t *config_ptr);
herr_t RECORDER_HDF5_DECL(H5Pset_alignment)(hid_t plist, hsize_t threshold, hsize_t alignment);
herr_t RECORDER_HDF5_DECL(H5Pset_chunk)(hid_t plist, int ndims, const hsize_t *dim);
herr_t RECORDER_HDF5_DECL(H5Pset_dxpl_mpio)(hid_t dxpl_id, H5FD_mpio_xfer_t xfer_mode);
herr_t RECORDER_HDF5_DECL(H5Pset_fapl_core)(hid_t fapl_id, size_t increment, hbool_t backing_store);
herr_t RECORDER_HDF5_DECL(H5Pset_fapl_mpio)(hid_t fapl_id, MPI_Comm comm, MPI_Info info);
herr_t RECORDER_HDF5_DECL(H5Pset_fapl_mpiposix)(hid_t fapl_id, MPI_Comm comm, hbool_t use_gpfs_hints);
herr_t RECORDER_HDF5_DECL(H5Pset_istore_k)(hid_t plist, unsigned ik);
herr_t RECORDER_HDF5_DECL(H5Pset_mdc_config)(hid_t plist_id, H5AC_cache_config_t *config_ptr);
herr_t RECORDER_HDF5_DECL(H5Pset_meta_block_size)(hid_t fapl_id, hsize_t size);
htri_t RECORDER_HDF5_DECL(H5Lexists)(hid_t loc_id, const char *name, hid_t lapl_id);
herr_t RECORDER_HDF5_DECL(H5Lget_val)(hid_t link_loc_id, const char *link_name, void *linkval_buff, size_t size, hid_t lapl_id);
herr_t RECORDER_HDF5_DECL(H5Literate)(hid_t group_id, H5_index_t index_type, H5_iter_order_t order, hsize_t *idx, H5L_iterate_t op, void *op_data);
herr_t RECORDER_HDF5_DECL(H5Oclose)(hid_t object_id);
/*
 * Exists in 1.8 not in 1.10, in 1.10 they are H5Oget_info1; H5Oget_info2
herr_t RECORDER_HDF5_DECL(H5Oget_info)(hid_t object_id, H5O_info_t *object_info);
herr_t RECORDER_HDF5_DECL(H5Oget_info_by_name)(hid_t loc_id, const char *object_name, H5O_info_t *object_info, hid_t lapl_id);
*/

hid_t RECORDER_HDF5_DECL(H5Oopen)(hid_t loc_id, const char *name, hid_t lapl_id);
herr_t RECORDER_HDF5_DECL(H5Pset_coll_metadata_write)(hid_t fapl_id, hbool_t is_collective);
herr_t RECORDER_HDF5_DECL(H5Pget_coll_metadata_write)(hid_t fapl_id, hbool_t* is_collective);
herr_t RECORDER_HDF5_DECL(H5Pset_all_coll_metadata_ops)(hid_t fapl_id, hbool_t is_collective);
herr_t RECORDER_HDF5_DECL(H5Pget_all_coll_metadata_ops)(hid_t fapl_id, hbool_t* is_collective);

#endif /* RECORDER_GOTCHA */
