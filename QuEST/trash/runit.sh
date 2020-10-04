cd build

platform="TengsMac"
framework="single_node"
filename="myData"
numThreads="2"
circuitDepth="30"
numQubits="5"
numRepetitions="1"

if [ "$1" == "0" ]
then
    if make; then
        echo "** make success **"
        ./newRandomCircuitBenchmarker ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
        exit 1
    else
        echo "** make failed **"
    fi
else
    echo "Direct make ailed, start from cmake"
fi


rm -r *
if CC=gcc-8 cmake ..; then
    echo "** cmake success **"
    if make; then
        echo "** make success **"
        ./newRandomCircuitBenchmarker ${platform} ${framework} ${filename} ${numThreads} ${circuitDepth} ${numQubits} ${numRepetitions}
    else
        echo "** make failed **"
    fi
else
    echo "** cmake failed **"
fi

cd ..
