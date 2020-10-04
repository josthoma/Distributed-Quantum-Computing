
# git pull
# ./run_cmake.sh

platform="Cluster"
framework="mul_node"
filename="mul_Data"
# circuitDepth="24" # different meaning
circuitDepth="28" # different meaning
numRepetitions=1
cd build

# ======================== Var Qubits ==========================
# var qubits
maxNumQubits=31
minNumQubits=31
slot1=1
slot2=1
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file

for ((numQubits=$minNumQubits; numQubits<=$maxNumQubits; numQubits++))
do
	result_name="result_nqubits_${numQubits}_slot1_${slot2}_slot2_${slot2}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
	# ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
	/export2/export/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${circuitDepth} ${numQubits} ${numRepetitions}
	# ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done

chmod -R 777 *
