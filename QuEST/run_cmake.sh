cd build
rm -rf *
CC=/export2/export/bin/mpicc cmake -DDISTRIBUTED=1 ..
# CXX=/usr/bin/g++ cmake ..
# MPI_CXX=/export2/export/bin/mpic++ cmake ..
make 
