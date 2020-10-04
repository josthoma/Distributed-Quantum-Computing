#!/bin/env bash

depth_values=(10 20)
threads_values=(2 3)

trial=${SLURM_ARRAY_TASK_ID}
threads=${threads_values[$(( trial % ${#threads_values[@]} ))]}
trial=$(( trial / ${#threads_values[@]} ))
depth=${depth_values[$(( trial % ${#depth_values[@]} ))]}

source ../../prep.sh
export OMP_NUM_THREADS=$threads
export OMP_PROC_BIND=spread

qubits=8
reps=10
platform="ARC"
framework="QuEST"
filename="benchmark_results/${platform}_${framework}/threads${threads}/depth${depth}/qubits${qubits}.txt"

## C is dumb
mkdir -p $filename   ## makes dir and file
rm -r $filename      ## removes file, keeps dir

./RandomCircuitBenchmarker $platform $framework $filename $threads $depth $qubits $reps