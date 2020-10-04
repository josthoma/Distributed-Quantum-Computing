#!/bin/sh -l

module load openmpi/2.1.6

cd $SLURM_SUBMIT_DIR

cd QuEST/
./mul_qubits.sh


