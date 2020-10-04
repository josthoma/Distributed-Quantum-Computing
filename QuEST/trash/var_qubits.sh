cd build

platform="TengsMac"
framework="single_node"
filename="sinpleData"
circuitDepth="100"
# numQubits=20
maxNumQubits=16
minNumQubits=15
numRepetitions="5"

numThreads="1"
export OMP_NUM_THREADS=1

for ((numQubits=$minNumQubits; numQubits<$maxNumQubits; numQubits++))
do
    ./newRandomCircuitBenchmarker ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done
