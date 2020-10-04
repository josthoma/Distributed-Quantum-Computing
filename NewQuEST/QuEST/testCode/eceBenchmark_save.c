#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

#include "helper.h"
#include "newRandomCircuits.h"
#include "../QuEST/include/QuEST.h"
#include "../CTools/memorymeasure.h"



int checkInputKeyNum(int narg) {
	int numInpKeys = sizeof(INPUT_KEYS) / sizeof(char *);
	if (narg - 1 != numInpKeys)
	{
		printf("InputKey Num Error: narg: %d, required numInputKeys: %d\n", narg, numInpKeys);
		printf("Please call as ./newRandomCircuitBenchmarker");
		for (int i = 0; i < numInpKeys; i++)
			printf(" %s", INPUT_KEYS[i]);
		printf("\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


int main(int narg, char *varg[])
{
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

	printf("SIZE_MAX: %ld",SIZE_MAX);

	// seed RandomCircuits
	srand(rSeed);
	QuESTEnv env = createQuESTEnv();
	Qureg qureg = createQureg(numQubits, env);

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
		durations[i] = normErrors[i] = 0;
		currRealMems[i] = currVirtMems[i] = 0;
		peakRealMems[i] = peakVirtMems[i] = 0;
	}

	// simulate random circuit and collect data
	for (int trial = 0; trial < numRepetitions; trial++)
	{
		durations[trial] = (double)applyRandomCircuit(qureg, circuitDepth);
		normErrors[trial] = 1 - calcTotalProb(qureg);
		getMemory(
			&currRealMems[trial], &peakRealMems[trial],
			&currVirtMems[trial], &peakVirtMems[trial]);

		printf("rank: %d, dur: %fs, error: %2.e,  ", env.rank, durations[trial], normErrors[trial]);
		printf("realMem: %lub (peak: %lub), ", currRealMems[trial], peakRealMems[trial]);
		printf("virtMem: %lub (peak: %lub)\n", currVirtMems[trial], peakVirtMems[trial]);
	}


	/****************************************************/
	/* ***************** Writing *********************** */
	/****************************************************/

	double *durations_global = malloc(numRepetitions * sizeof(double));
	double *normErrors_global = malloc(numRepetitions * sizeof(double));
	unsigned long *currRealMems_global = malloc(memArrSize);
	unsigned long *peakRealMems_global = malloc(memArrSize);
	unsigned long *currVirtMems_global = malloc(memArrSize);
	unsigned long *peakVirtMems_global = malloc(memArrSize);

	for (int i = 0; i < numRepetitions; i++)
	{
		durations_global[i] = normErrors_global[i] = 0;
		currRealMems_global[i] = currVirtMems_global[i] = 0;
		peakRealMems_global[i] = peakVirtMems_global[i] = 0;
	}

	/*
	// write to file
	MPI_Reduce(durations, durations_global, numRepetitions, MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Reduce(normErrors, normErrors_global, numRepetitions, MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Reduce(currRealMems, currRealMems_global, numRepetitions, MPI_UNSIGNED_LONG,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(peakRealMems, peakRealMems_global, numRepetitions, MPI_UNSIGNED_LONG,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(currVirtMems, currVirtMems_global, numRepetitions, MPI_UNSIGNED_LONG,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(peakVirtMems, peakVirtMems_global, numRepetitions, MPI_UNSIGNED_LONG,MPI_SUM,0,MPI_COMM_WORLD);
	if (env.rank == 0) {
		FILE *file = fopen(filename, "a+");
		fprintf(file, "Simulating random circuit with params:\n");
		fprintf(file, "platform:\t%s\n", platform);
		fprintf(file, "framework:\t%s\n", framework);
		fprintf(file, "filename:\t%s\n", filename);
		fprintf(file, "numThreads:\t%d\n", numThreads);
		fprintf(file, "circuitDepth:\t%d\n", circuitDepth);
		fprintf(file, "numQubits:\t%d\n", numQubits);
		fprintf(file, "numRepetitions:\t%d\n", numRepetitions);
		fprintf(file, "randomSeed:\t%lu\n", rSeed);
		fprintf(file, "\n");

		fprintf(file, "Rank: Average on %d\n", env.rank);
		writeAvgDoubleArray(file, "avgDurations", durations_global, numRepetitions);
		writeAvgDoubleArray(file, "avgNormErrors", normErrors_global, numRepetitions);
		writeAvgUnsignedLongArray(file, "avgCurrRealMemories", currRealMems_global, numRepetitions);
		writeAvgUnsignedLongArray(file, "avgPeakRealMemories", peakRealMems_global, numRepetitions);
		writeAvgUnsignedLongArray(file, "avgCurrVirtMemories", currVirtMems_global, numRepetitions);
		writeAvgUnsignedLongArray(file, "avgPeakVirtMemories", peakVirtMems_global, numRepetitions);
		fprintf(file, "\n");
		fclose(file);
	}
	*/

	/****************************************************/
	/* ***************** Ending *********************** */
	/****************************************************/

	destroyQureg(qureg, env);
	destroyQuESTEnv(env);

	free(durations);
	free(normErrors);
	free(currRealMems);
	free(peakRealMems);
	free(currVirtMems);
	free(peakVirtMems);


    gettimeofday(&timeInstance, NULL);
    long double endTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    printf("%Lf",endTime - startTime);


	return EXIT_SUCCESS;
}

