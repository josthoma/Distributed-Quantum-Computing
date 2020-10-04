
platform="Cluster"
framework="mul_node"
filename="mul_Data"
circuitDepth="100"
numRepetitions=1
cd ../build

# ======================== Var Slot 1 ==========================
numQubits=20
numThreads=1
export OMP_NUM_THREADS=${numThreads}

slot1=1
slot2=0
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file
result_name="result_nqubits_${numQubits}_slot1_${slot1}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
mv ${result_name} ../mpiRun/results/${result_name}

slot1=2
slot2=0
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file
result_name="result_nqubits_${numQubits}_slot1_${slot1}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
mv ${result_name} ../mpiRun/results/${result_name}

slot1=4
slot2=0
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file
result_name="result_nqubits_${numQubits}_slot1_${slot1}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
mv ${result_name} ../mpiRun/results/${result_name}

slot1=8
slot2=0
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file
result_name="result_nqubits_${numQubits}_slot1_${slot1}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
mv ${result_name} ../mpiRun/results/${result_name}
