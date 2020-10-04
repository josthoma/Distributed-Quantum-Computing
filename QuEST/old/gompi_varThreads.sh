
git pull
./run_cmake.sh

platform="Cluster"
framework="mul_node"
filename="mul_Data"
circuitDepth="100"
numRepetitions=1
cd build

# ======================== Var Qubits ==========================
# var qubits
numQubits = 22
slot1=1
slot2=0
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file


maxNumThreads=1
minNumThreads=12

for ((numThreads=$minNumThreads; numThreads<=$maxNumThreads; numThreads++))
do
	export OMP_NUM_THREADS=${numThreads}
	result_name="result_nqubits_${numQubits}_slot1_${slot2}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
	~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
	# ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done
