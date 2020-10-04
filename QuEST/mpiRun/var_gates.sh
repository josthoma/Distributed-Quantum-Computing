
platform="Cluster"
framework="mul_node"
filename="mul_Data"
circuitDepth="100"
numRepetitions=1
cd ../build


# ======================== Var Qubits ==========================
maxNumQubits=25
minNumQubits=1
slot1=1
slot2=0
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file
numThreads=1
export OMP_NUM_THREADS=${numThreads}

for ((numQubits=$minNumQubits; numQubits<=$maxNumQubits; numQubits++))
do
	result_name="result_nqubits_${numQubits}_slot1_${slot2}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
	~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
	mv ${result_name} ../mpiRun/results/${result_name}
done
'
