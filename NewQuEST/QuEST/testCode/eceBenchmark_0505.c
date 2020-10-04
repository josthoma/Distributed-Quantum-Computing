#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
// #include <mpi.h>
#include <sched.h>

#include "helper.h"
#include "newRandomCircuits.h"
#include "../QuEST/include/QuEST.h"
#include "../CTools/memorymeasure.h"



int checkInputKeyNum(int narg) {
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
	cpu_set_t set;
	CPU_ZERO(&set);        // clear cpu mask
	CPU_SET(0, &set);      // set cpu 0
	CPU_SET(1, &set);      // set cpu 1
	CPU_SET(2, &set);      // set cpu 1
	CPU_SET(3, &set);      // set cpu 1
	CPU_SET(4, &set);      // set cpu 1
	CPU_SET(5, &set);      // set cpu 1
	CPU_SET(6, &set);      // set cpu 0
	CPU_SET(7, &set);      // set cpu 0
	CPU_SET(8, &set);      // set cpu 0
	CPU_SET(9, &set);      // set cpu 0
	CPU_SET(10, &set);      // set cpu 0
	CPU_SET(11, &set);      // set cpu 0
	sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process	
	// printf("%d\n",CPU_COUNT());

    struct timeval timeInstance;
    gettimeofday(&timeInstance, NULL);
    long double startTime = (
   		timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));

	/****************************************************/
	/* ***************** Preparation ****************** */
	/****************************************************/

	// Check number of input keys
	if (checkInputKeyNum(narg) == EXIT_FAILURE) {
		return EXIT_FAILURE;	
	}

	// Prepare simulation parameters by reading cmd args
	char *platform = varg[1];
	char *framework = varg[2];
	char *filename = varg[3];
	int numThreads = atoi(varg[4]);
	int circuitDepth = atoi(varg[5]);
	int numQubits = atoi(varg[6]);
	int numRepetitions = atoi(varg[7]);

	// generate a random seed from hashing params
	unsigned long rSeed = hashParams(
		platform, framework, filename,
		numThreads, circuitDepth, numQubits, numRepetitions);



	/****************************************************/
	/* ***************** Simulation ******************* */
	/****************************************************/

	// seed RandomCircuits
	srand(rSeed);
	QuESTEnv env = createQuESTEnv();
	Qureg qureg = createQureg(numQubits, env);

	double *durations = malloc(numRepetitions * sizeof(double));

	// clear arrays (so that on sys that don't support memory measure, the output is less confusing)
	for (int i = 0; i < numRepetitions; i++)
	{
		durations[i] = 0;
	}

	// simulate random circuit and collect data
	for (int trial = 0; trial < numRepetitions; trial++)
	{
		durations[trial] = (double)applyRandomCircuit(qureg, circuitDepth);
		printf("Total Circuit: %f\n",durations[trial]);
	}


	/****************************************************/
	/* ***************** Ending *********************** */
	/****************************************************/

	destroyQureg(qureg, env);
	destroyQuESTEnv(env);

	free(durations);

	return EXIT_SUCCESS;
}

