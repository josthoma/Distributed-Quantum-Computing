
platform="TengsMac"
framework="single_node"
filename="new_Data"
numThreads="1"
circuitDepth="100"
# numQubits=20
maxNumQubits=25
minNumQubits=1
numRepetitions="1"


for ((numQubits=$minNumQubits; numQubits<=$maxNumQubits; numQubits++))
do
    python3 new_random_circuit_benchmarker.py ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
done
