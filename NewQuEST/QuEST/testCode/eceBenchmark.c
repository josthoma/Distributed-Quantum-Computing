#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
// #include <mpi.h>
#include <sched.h>
#include <unistd.h>

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


int main(int narg, char *varg[]) {
	printf("--- Start Main ---\n");
	// char *platform = varg[1];
	// char *framework = varg[2];
	// char *filename = varg[3];
	// int numThreads = atoi(varg[4]);
	// int circuitDepth = atoi(varg[5]);
	int numQubits = atoi(varg[6]);
	int numRepetitions = atoi(varg[7]);


	double a = 0.1;

	QuESTEnv env = createQuESTEnv();
	Qureg qureg = createQureg(numQubits, env);

	for (int iii = 0; iii < numRepetitions; iii++) {
		initPlusState(qureg);
	}
	destroyQureg(qureg, env);
	destroyQuESTEnv(env);
	printf("--- End Main ---\n");

	return 1;
}