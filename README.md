# Overview (updated 10/20)

We implemented an adaptive algorithm on Quantum computing framework QuEST which enables the simulator to be run more efficiently on heterogenous distributed systems. Our experiments showed improved execution time for larger qubits (28,29 etc.)

This work was done under the guidance of Dr. Charlie Hu at Purdue University along with Teng Bian.

Note: We do not have any rights to the QuEST repository. We forked QuEST from https://github.com/QuEST-Kit/QuEST, modified the source code for our experiments.

If you want more details on the final results contact: thoma900@purdue.edu or biant@purdue.edu

# Instruction (updated 04/20)

How to run the script in cloud?
- Step 1: go to the corresponding folder
`cd export2/export/ece673-project/QuEST/mpiRun`
- Step 2: adjust the bash script file and run it
  + Example: var_nthreads.sh, you can change numQubits, slot1, slot2 or others
  + Excution: `./var_nthreads.sh`
  + Output: the results will be stored in ./results
- Step 3: ajust the python script file in analysis/ for data analysis, and run it
  + Example: analysis_var_threads.py, don't forget to make your changed parameters consistent in .py script and .sh script
  + Excution: `python analysis_var_threads.py`
- Step 4: check the resulting graph in ece673-project/QuEST/mpiRun/result_graphs


# Feel free to share any ideas or ask any questions below
1. Since I only wrote scientific code, which is considered good enough if it's usable, I have little experience of maintaining versions, updating README.md, or distributing tasks. So please let me know anything that you think more professional, or you could just edit it anyway you would like to. (Teng)

# What have been done right now
1. Benchmark test for QuEST and ProjectQ in Mac/Ubuntu
2. Ongoing benchmark test for QuEST in Brown Cluster

# How to Run QuEST experiments in personal computers
  - Required packages
    - CMake: version 3.1+
    - gcc-8 (gcc-9 or gcc-4 are not satisfactory, other versions are not tested)
    - Note: packages have been installed or satisfied in Teng's Mac/Ubuntu
  - How to run?
    - Downlaod the repository
      ```
      github clone https://github.com/tengbian/ece673-project.git
      ```
    - Start from the directory "ece673-project/QuEST"
      ```
      cd ece673-project/QuEST
      ```
    - Create "build" directory and go into it
      ```
      mkdir build
      cd build
      ```
    - Do cmake and make ([] is optional for current tests. But some code are written in C++, so it might be required later)
      ```
      CC=gcc-8 [CXX=g++-8] cmake ..
      make
      ```
    - Go to main directory "QuEST" and run bash script
      ```
      cd ..
      ./var_qubits.sh
      ```
  - How to adjust parameters of experiments
    - Edit file "QuEST/var_qubits.sh"
    
# How to Run QuEST experiments in Brown Cluster
  - Required packages
    - CMake: version 3.1+
    - gcc-8 (gcc-9 or gcc-4 are not satisfactory, other versions are not tested)
    - openmpi/2.1.6 (other versions are not tested)
    - Note: packages have been either loaded or installed in Teng's cluster account
  - How to run?
    - Downlaod the repository
      ```
      github clone https://github.com/tengbian/ece673-project.git
      ```
    - Start from the directory "ece673-project/QuEST"
      ```
      cd ece-673project/QuEST
      ```
    - Create "build" directory and go into it
      ```
      mkdir build
      cd build
      ```
    - Do cmake and make ([] is optional for current tests. But some code are written in C++, so it might be required later)
      ```
      CC=gcc cmake ..                # for single-node mode
      [CC=mpicc cmake -DDISTRIBUTED=1 .. # for multi-node mode]
      make
      ```
    - Go to the directory "ece673-project/runCluster/runQuEST" and submit job to the cluster
      ```
      cd ../../runCluster/runQuEST/
      ./ssub_sin.sh # for single-node mode
      [./ssub_mul.sh # for multi-node mode]
      ``` 
  - How to adjust parameters of experiments
    - Edit file "QuEST/var_qubits.sh" or "QuEST/mul_qubits.sh"
  - How to run mpi programs in MPI in cloud4/cloud5
    ~/local/openmpi/bin/mpirun --hostfile host_file mpi_hello_world

# Notes (Waiting for updating after this section):
The folder "QuEST" is cloned from its github. I renamed ".git" to "save_dot_git" in that folder to avoid conflicts while pushing, and to keep the version information as well. Everything in "QuEST" remains the same except:

1. "QuEST/CMakeLists.txt" is updated
  - change demo example file to test file (line 19,20 -> 22, 23)
2. "QuEST/testCode" folder is added
  - all test files are stored (currently newRandomCircuits.c)
3. "QuEST/CTools" folder is obtained from Tyson Jones's github

# Notes about MPI
- How to check MPI version
  ```
  mpirun --version
  ```
- How to run programs with MPI?
  ```
  mpirun -n <# of processes> -ppn <# of processes per node> -f <hostfile> ./myprog
  mpirun -n 4 -ppn 2 -f hosts ./myprog # Example
  ```



# Other Notes
1. a simple test file that construct random quantum circuits and print duration time
  - test file is "QuEST/testCode/newRandomCircuits.c", modified from Benchmark file "QuEST-ProjectQ-benchmarking/C/RandomCircuit.c"
  - refer to "How to use the current version"
 

# Other Comments
1. In "QuEST-ProjectQ-benchmarking/C/RandomCircuit.c", the QuEST authors mistook T gates for Toffoli gates in their Benchmark tests. Also, when they apply random gates (gate = X, Y), they are actually applying sqrt_X, sqrt_Y. But this doesn't matter much.




