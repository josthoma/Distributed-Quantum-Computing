cd build

platform="TengsMac"
framework="single_node"
filename="sinpleData"
circuitDepth="100"
# numQubits=20
maxNumQubits=16
minNumQubits=15
numRepetitions="1"

numThreads="2"
export OMP_NUM_THREADS=2

for ((numQubits=$minNumQubits; numQubits<$maxNumQubits; numQubits++))
do
    ./newRandomCircuitBenchmarker ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done
