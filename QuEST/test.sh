git pull
CC=gcc cmake ..
make
./eceBenchmark Cluster mul_node mul_Data 1 10 2 1
