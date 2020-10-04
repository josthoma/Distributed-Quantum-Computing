
platform="Cluster"
framework="mul_node"
filename="mul_Data"
circuitDepth="100"
numRepetitions=1
cd ../build


# ======================== Var Threads ==========================
numQubits=22


slot1=8
slot2=8
echo "cloud4 slots=${slot1}" > host_file
echo "cloud5 slots=${slot2}" >> host_file

minNumThreads=1
maxNumThreads=12

for ((numThreads=$minNumThreads; numThreads<=$maxNumThreads; numThreads++))
do
	export OMP_NUM_THREADS=${numThreads}
	result_name="result_nqubits_${numQubits}_slot1_${slot1}_slot2_${slot2}_nthreads_${numThreads}_cdepth_${circuitDepth}_nrepetition_${numRepetitions}"
	~/local/openmpi/bin/mpirun --hostfile host_file ./eceBenchmark ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions} > ${result_name}
	mv ${result_name} ../mpiRun/results/${result_name}
done
