
# git pull
# ./run_cmake.sh

platform="Cluster"
framework="mul_node"
filename="mul_Data"
# circuitDepth="24" # different meaning
numRepetitions=1 # not used in later execution
cd build

# ======================== Var Qubits ==========================
# var qubits
numQubits=31
slot1=1
slot2=1
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file

# different meaning
for ((circuitDepth=0; circuitDepth<$numQubits; circuitDepth++))
do
	result_name="result_nqubits_${numQubits}_slot1_${slot2}_slot2_${slot2}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
	for ((index=0; index<$numRepetitions; index++))
		do
			echo "${circuitDepth} ${index}"
			# ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
			/export2/export/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${circuitDepth} ${numQubits} ${numRepetitions} >> ../results/$result_name
			# ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
			echo "over"
		done
done

chmod -R 777 *
