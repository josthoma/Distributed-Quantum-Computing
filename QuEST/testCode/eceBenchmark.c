#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>
#include <sched.h>
#include <unistd.h>

#include "helper.h"
#include "newRandomCircuits.h"
#include "../QuEST/include/QuEST.h"
#include "../CTools/memorymeasure.h"

#include <sys/sysinfo.h>

int checkInputKeyNum(int narg)
{
	int numInpKeys = sizeof(INPUT_KEYS) / sizeof(char *);
	if (narg - 1 != numInpKeys)
	{
		printf("InputKey Num Error: narg: %d, required numInputKeys: %d\n", narg, numInpKeys);
		printf("Please call as ./eceBenchmark");
		for (int i = 0; i < numInpKeys; i++)
			printf(" %s", INPUT_KEYS[i]);
		printf("\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main(int narg, char *varg[])
{

	char hostname[1024];
	gethostname(hostname, sizeof(hostname));
	char check[] = "cloud4";
	if (strcmp(hostname, check) == 0)
	{
		printf("cloud4 cpuset\n");
		cpu_set_t set;
		CPU_ZERO(&set); // clear cpu mask
		CPU_SET(0, &set);
		CPU_SET(1, &set); // set cpu 1
		CPU_SET(2, &set);
		CPU_SET(3, &set);
		sched_setaffinity(0, sizeof(cpu_set_t), &set); // 0 is the calling process
	}
	else
	{
		printf("cloud5 cpuset\n");
		cpu_set_t set;
		CPU_ZERO(&set); // clear cpu mask
		CPU_SET(0, &set);
		CPU_SET(1, &set); // set cpu 1
		CPU_SET(2, &set);
		CPU_SET(3, &set);
		CPU_SET(4, &set);
		CPU_SET(5, &set);
		CPU_SET(6, &set);
		CPU_SET(7, &set);
		CPU_SET(8, &set);
		CPU_SET(9, &set);
		CPU_SET(10, &set);
		CPU_SET(11, &set);
		sched_setaffinity(0, sizeof(cpu_set_t), &set); // 0 is the calling process
	}
	printf("Processor %s\n", hostname);

	/****************************************************/
	/* ***************** Preparation ****************** */
	/****************************************************/

	// Check number of input keys
	if (checkInputKeyNum(narg) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// Prepare simulation parameters by reading cmd args
	char *platform = varg[1];
	char *framework = varg[2];
	char *filename = varg[3];
	// int numThreads = atoi(varg[4]);
	int circuitDepth = atoi(varg[4]);

	int numQubits = atoi(varg[5]);
	int numRepetitions = atoi(varg[6]);

	unsigned long rSeed = 0;

	/****************************************************/
	/* ***************** Simulation ******************* */
	/****************************************************/

	// seed RandomCircuits
	srand(rSeed);
	QuESTEnv env = createQuESTEnv();
	Qureg qureg = createQureg(numQubits, env);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	printf("Number of Chunks are : %d\n", qureg.numChunks);
	double *durations = malloc(numRepetitions * sizeof(double));
	double *normErrors = malloc(numRepetitions * sizeof(double));

	int memArrSize = numRepetitions * sizeof(unsigned long);
	unsigned long *currRealMems = malloc(memArrSize);
	unsigned long *peakRealMems = malloc(memArrSize);
	unsigned long *currVirtMems = malloc(memArrSize);
	unsigned long *peakVirtMems = malloc(memArrSize);

	// clear arrays (so that on sys that don't support memory measure, the output is less confusing)
	for (int i = 0; i < numRepetitions; i++)
	{
		durations[i] = 0;
		currRealMems[i] = currVirtMems[i] = 0;
		peakRealMems[i] = peakVirtMems[i] = 0;
	}

	struct timeval timeInstance;
	gettimeofday(&timeInstance, NULL);
	long double startTime = (timeInstance.tv_sec +
							 (long double)timeInstance.tv_usec / pow(10, 6));

	// simulate random circuit and collect data
	for (int trial = 0; trial < numRepetitions; trial++)
	{
		durations[trial] = (double)applyRandomCircuit(qureg, circuitDepth, env);

		// printf("Total Circuit: %f\n",durations[trial]);
		// getMemory(
		// &currRealMems[trial], &peakRealMems[trial],
		// &currVirtMems[trial], &peakVirtMems[trial]);

		// printf("rank: %d, dur: %fs, error: %2.e,  ", env.rank, durations[trial], normErrors[trial]);
	}
	// printf("#CPUs: %d\n", get_nprocs());

	gettimeofday(&timeInstance, NULL);
	long double endTime = (timeInstance.tv_sec +
						   (long double)timeInstance.tv_usec / pow(10, 6));
	// printf("%Lf\n", endTime - startTime);

	/****************************************************/
	/* ***************** Ending *********************** */
	/****************************************************/

	destroyQureg(qureg, env);
	destroyQuESTEnv(env);

	free(durations);

	return EXIT_SUCCESS;
}
