
cd build

platform="TengsMac"
framework="single_node"
filename="sinpleData"
circuitDepth="10"
# numQubits=20
maxNumQubits=24
minNumQubits=24

for ((numQubits=$minNumQubits; numQubits<=$maxNumQubits; numQubits++))
do
	./eceBenchmark ${platform} ${framework} ${filename} ${circuitDepth} ${numQubits} ${numRepetitions}
done
