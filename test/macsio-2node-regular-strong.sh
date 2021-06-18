#!/bin/bash

### LSF syntax
#BSUB -nnodes 2                   #number of nodes
#BSUB -W 2                      #walltime in minutes
#BSUB -e macsio.regular.strong.2node.out             #stderr
#BSUB -o macsio.regular.strong.2node.err             #stdout
#BSUB -J mac.reg.str.2                    #name of job
#BSUB -q pbatch                   #queue to use

num_proc=64
num_nodes=2
file_system=regular
scaling=strong-scaling

((part_size = 1024 / num_proc * 32))
cwd=/g/g92/xu23/summer-2021/recorder-logs
recorder_path=/g/g92/xu23/summer-2021/recorder-install
darshan_path=/g/g92/xu23/installs/darshan-3.2.1
bin=/g/g92/xu23/apps/spack/opt/spack/linux-rhel7-power9le/gcc-8.3.1/macsio-1.1-roxkbe7gqsmjjg2st5d6rgyp5s6ijdnd
profiles=/g/g92/xu23/summer-2021

export LD_PRELOAD=/g/g92/xu23/summer-2021/recorder-install/lib/librecorder.so:/g/g92/xu23/apps/GOTCHA-1.0.3/lib64/libgotcha.so.2
export RECORDER_TRACES_DIR=$profiles/recorder-logs
srun --ntasks-per-node 32 -n $num_proc ${bin}/macsio --interface silo --avg_num_parts 4 --part_size ${part_size}K --part_type unstructured --part_dim 3 --vars_per_part 16 --num_dumps 4 --parallel_file_mode MIF 8 
mv ${cwd}/recorder-logs ${profiles}/recorder-logs/macsio.${file_system}.${scaling}.${num_proc}.silo.4.${part_size}K.unstructured.3.16.4.MIF8.recorder-log
