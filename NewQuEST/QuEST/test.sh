git pull
CC=gcc CFLAGS='-g -Wall -Wextra' cmake ..
make
./eceBenchmark Cluster mul_node mul_Data 1 10 2 1
