cd build
rm host_file
make

platform="Cluster"
framework="mul_node"
filename="mul_Data"
circuitDepth="100"
# numQubits=20
maxNumQubits=5
minNumQubits=3
numRepetitions=1

numThreads=12
slot1=1
slot2=1

export OMP_NUM_THREADS=${numThreads}
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file

for ((numQubits=$minNumQubits; numQubits<$maxNumQubits; numQubits++))
do
	result_name="result_nqubits_${numQubits}_slot1_${slot2}_slot2_${slot2}_nThreads_${numThreads}_cDepth_${circuitDepth}_nRepeat_${numRepetitions}"
	~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
	cp ${result_name} ../results/
done
