#!/bin/bash

### LSF syntax
#BSUB -nnodes 2                   #number of nodes
#BSUB -W 2                      #walltime in minutes
#BSUB -e macsio.2node.out             #stderr
#BSUB -o macsio.2node.err             #stdout
#BSUB -J mac.2                    #name of job
#BSUB -q pbatch                   #queue to use

NPROC=2

((part_size = 1024 / NPROC * 32))
CWD=/g/g92/xu23/summer-2021/recorder-PROFILE_PATH
GOTCHA_PATH=/g/g92/xu23/installs/gotcha-1.0.3
ORIGINAL_RECORDER_PATH=/g/g92/xu23/installs/Recorder-2.2
GOTCHA_RECORDER_PATH=/g/g92/xu23/summer-2021/recorder-install-with-ini
BIN=/g/g92/xu23/apps/spack/opt/spack/linux-rhel7-power9le/gcc-8.3.1/macsio-1.1-roxkbe7gqsmjjg2st5d6rgyp5s6ijdnd
PROFILE_PATH=/g/g92/xu23/summer-2021/recorder-PROFILE_PATH

for NPROC in 1 2 4
do
    # original recorder
    export RECORDER_TRACES_DIR=$PROFILE_PATH/original
    jsrun -n ${NPROC} -E LD_PRELOAD=${GOTCHA_PATH}/lib64/libgotcha.so.2:${GOTCHA_RECORDER_PATH}/lib/librecorder.so \
    ${BIN}/macsio --interface silo --avg_num_parts 4 --part_size ${part_size}K --part_type unstructured --part_dim 3 --vars_per_part 16 --num_dumps 4 --parallel_file_mode MIF 8 \
    &>> ${PROFILE_PATH}/app-stdio/macsio.${file_system}.${scaling}.${NPROC}.silo.4.${part_size}K.unstructured.3.16.4.MIF8.recorder.txt
    mv ${CWD}/recorder-logs ${PROFILE_PATH}/recorder-logs/macsio.${file_system}.${scaling}.${NPROC}.silo.4.${part_size}K.unstructured.3.16.4.MIF8.recorder-log
    printf "\n ========================================== Recorder END ============================================= \n\n\n"
    printf "\n ========================================== Recorder END ============================================= \n\n\n" &>> ${PROFILE_PATH}/app-stdio/macsio.${file_system}.${scaling}.${NPROC}.silo.4.${part_size}K.unstructured.3.16.4.MIF8.recorder.txt

done