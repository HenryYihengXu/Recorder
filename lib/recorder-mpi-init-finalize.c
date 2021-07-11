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

#define _XOPEN_SOURCE 500
#define _GNU_SOURCE /* for RTLD_NEXT */

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <dlfcn.h>

#include "mpi.h"
#include "recorder.h"

#ifdef RECORDER_GOTCHA
#include "gotcha_map_recorder_list.h"
#endif

#ifndef PRIORITY
#define PRIORITY 1
#endif

static double local_tstart, local_tend;
static int rank, nprocs;

void recorder_init(int *argc, char ***argv) {
#ifndef RECORDER_GOTCHA
    MAP_OR_FAIL(PMPI_Comm_size)
    MAP_OR_FAIL(PMPI_Comm_rank)
    RECORDER_REAL_CALL(PMPI_Comm_rank)(MPI_COMM_WORLD, &rank);
    RECORDER_REAL_CALL(PMPI_Comm_size)(MPI_COMM_WORLD, &nprocs);
#else
    MAP_OR_FAIL(MPI_Comm_size)
    MAP_OR_FAIL(MPI_Comm_rank)
    RECORDER_REAL_CALL(MPI_Comm_rank)(MPI_COMM_WORLD, &rank);
    RECORDER_REAL_CALL(MPI_Comm_size)(MPI_COMM_WORLD, &nprocs);
#endif /* RECORDER_GOTCHA */
    

    logger_init(rank, nprocs);
    utils_init();
    local_tstart = recorder_wtime();
}

void recorder_finalize() {
    logger_finalize();
    utils_finalize();

    local_tend = recorder_wtime();

    if (rank == 0)
        printf("[Recorder] elapsed time on rank 0: %.2f\n", local_tend-local_tstart);
}

#ifndef RECORDER_GOTCHA
int PMPI_Init(int *argc, char ***argv) {
    MAP_OR_FAIL(PMPI_Init)
    int ret = RECORDER_REAL_CALL(PMPI_Init) (argc, argv);
    recorder_init(argc, argv);
    return ret;
}

int PMPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
    MAP_OR_FAIL(PMPI_Init_thread)
    int ret = RECORDER_REAL_CALL(PMPI_Init_thread) (argc, argv, required, provided);
    recorder_init(argc, argv);
    return ret;
}

int MPI_Init(int *argc, char ***argv) {
    MAP_OR_FAIL(PMPI_Init)
    int ret = RECORDER_REAL_CALL(PMPI_Init) (argc, argv);
    recorder_init(argc, argv);
    return ret;
}

int MPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
    MAP_OR_FAIL(PMPI_Init_thread)
    int ret = RECORDER_REAL_CALL(PMPI_Init_thread) (argc, argv, required, provided);
    recorder_init(argc, argv);
    return ret;
}

int PMPI_Finalize(void) {
    recorder_finalize();
    MAP_OR_FAIL(PMPI_Finalize);
    return RECORDER_REAL_CALL(PMPI_Finalize) ();
}

int MPI_Finalize(void) {
    recorder_finalize();
    MAP_OR_FAIL(PMPI_Finalize);
    int res = RECORDER_REAL_CALL(PMPI_Finalize) ();
    return res;
}

#else /* RECORDER_GOTCHA */
    #ifndef WITH_INIT_FINI
    /* MPI_Init and MPI_Init_thread are not wrapped, because they are where gotcha init is called. Before 
       gotcha init, there is no way to wrap functions.
    */
    /* I have to wrap PMPI_Init and PMPI_Init_thread, though they can also be the entrance of Recorder.
       So applications are required to only use MPI_Init and MPI_Init_thread. Otherwise if users diable
       mpi tracing, there is no way for Recorder to start.
    */
    /* Here I'm not using the macro, because if users define DISABLE_MPI, I need to make sure these Recorder
       entries and exits won't be ignored
    */
    int __recorder_gotcha_wrap_PMPI_Init(int *argc, char ***argv) {
        printf("In PMPI_Init wrapper\n");
        setup_recorder_gotcha_wrappers(PRIORITY);
        MAP_OR_FAIL(PMPI_Init)
        int ret = RECORDER_REAL_CALL(PMPI_Init) (argc, argv);
        recorder_init(argc, argv);
        return ret;
    }

    int __recorder_gotcha_wrap_PMPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
        setup_recorder_gotcha_wrappers(PRIORITY);
        MAP_OR_FAIL(PMPI_Init_thread)
        int ret = RECORDER_REAL_CALL(PMPI_Init_thread) (argc, argv, required, provided);
        recorder_init(argc, argv);
        return ret;
    }

    int MPI_Init(int *argc, char ***argv) {
        setup_recorder_gotcha_wrappers(PRIORITY);
        MAP_OR_FAIL(PMPI_Init)
        int ret = RECORDER_REAL_CALL(PMPI_Init) (argc, argv);
        recorder_init(argc, argv);
        return ret;
    }

    int MPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
        setup_recorder_gotcha_wrappers(PRIORITY);
        MAP_OR_FAIL(PMPI_Init_thread)
        int ret = RECORDER_REAL_CALL(PMPI_Init_thread) (argc, argv, required, provided);
        recorder_init(argc, argv);
        return ret;
    }

    int __recorder_gotcha_wrap_PMPI_Finalize(void) {
        recorder_finalize();
        MAP_OR_FAIL(PMPI_Finalize);
        return RECORDER_REAL_CALL(PMPI_Finalize) ();
    }

    int __recorder_gotcha_wrap_MPI_Finalize(void) {
        recorder_finalize();
        MAP_OR_FAIL(MPI_Finalize);
        int res = RECORDER_REAL_CALL(MPI_Finalize) ();
        return res;
    }

    #else /* WITH_INIT_FINI */
    int __recorder_gotcha_wrap_PMPI_Init(int *argc, char ***argv) {
        MAP_OR_FAIL(PMPI_Init)
        int ret = RECORDER_REAL_CALL(PMPI_Init) (argc, argv);
        recorder_init(argc, argv);
        return ret;
    }

    int __recorder_gotcha_wrap_PMPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
        MAP_OR_FAIL(PMPI_Init_thread)
        int ret = RECORDER_REAL_CALL(PMPI_Init_thread) (argc, argv, required, provided);
        recorder_init(argc, argv);
        return ret;
    }

    int __recorder_gotcha_wrap_MPI_Init(int *argc, char ***argv) {
        // fprintf(stderr, "\n\nRecorder MPI_Init wrapper is called\n\n");
        MAP_OR_FAIL(MPI_Init)
        int ret = RECORDER_REAL_CALL(MPI_Init) (argc, argv);
        recorder_init(argc, argv);
        return ret;
    }

    int __recorder_gotcha_wrap_MPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
        MAP_OR_FAIL(MPI_Init_thread)
        int ret = RECORDER_REAL_CALL(MPI_Init_thread) (argc, argv, required, provided);
        recorder_init(argc, argv);
        return ret;
    }

    int __recorder_gotcha_wrap_PMPI_Finalize(void) {
        recorder_finalize();
        MAP_OR_FAIL(PMPI_Finalize);
        return RECORDER_REAL_CALL(PMPI_Finalize) ();
    }

    int __recorder_gotcha_wrap_MPI_Finalize(void) {
        fprintf(stderr, "In MPI_Finalize Recorder Finalizing\n");
        recorder_finalize();
        MAP_OR_FAIL(MPI_Finalize);
        int res = RECORDER_REAL_CALL(MPI_Finalize) ();
        return res;
    }
    
    static void ld_preload_init(void) __attribute__((constructor));
    static void ld_preload_finalize(void) __attribute__((destructor));
    static void ld_preload_init(void)
    {
        fprintf(stderr, "\n\nIn ld constructor setting up gotcha wrappers\n\n");
        setup_recorder_gotcha_wrappers(PRIORITY);
    }

    // static void ld_preload_finalize(void)
    // {
    //     recorder_finalize();
    // }
    #endif /* WITH_INIT_FINI */
#endif /* RECORDER_GOTCHA */
