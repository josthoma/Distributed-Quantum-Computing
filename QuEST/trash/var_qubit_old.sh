cd build

platform="TengsMac"
framework="single_node"
filename="myData"
numThreads="1"
circuitDepth="30"
maxNumQubits=25
minNumQubits=15
numRepetitions="1"


for ((numQubits=$minNumQubits; numQubits<$maxNumQubits; numQubits++))
do
    ./newRandomCircuitBenchmarker ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done
