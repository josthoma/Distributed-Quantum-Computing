cd build

platform="Cluster"
framework="mul_node"
filename="mul_Data"
circuitDepth="100"
# numQubits=20
maxNumQubits=16
minNumQubits=15
numRepetitions="100"

numThreads="1"
export OMP_NUM_THREADS=10

for ((numQubits=$minNumQubits; numQubits<$maxNumQubits; numQubits++))
do
    mpirun -np 1 ./newRandomCircuitBenchmarker ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done
