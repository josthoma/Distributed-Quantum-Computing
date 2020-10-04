/**
 *  Benchmarks the random circuit algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "helper.h"
#include "newRandomCircuits.h"
#include "../QuEST/include/QuEST.h"
#include "../CTools/memorymeasure.h"

const char *INPUT_KEYS[] = {
    "platform", "framework", "filename",
    "numThreads", "circuitDepth", "numQubits", "numRepetitions"
};

const char *OUTPUT_KEYS[] = {
    "durations", "normErrors"
    "currRealMemories", "peakRealMemories",
    "currVirtMemories", "peakVirtMemories"};





unsigned long hashParams(
	char *platform,
	char *framework,
	char *filename,
	int numThreads,
	int circuitDepth,
	int numQubits,
	int numRepetitions)
{

	unsigned long hash = 5381;
	int c;
	while ((c = *platform++))
		hash = ((hash << 5) + hash) + c;
	while ((c = *framework++))
		hash = ((hash << 5) + hash) + c;
	while ((c = *filename++))
		hash = ((hash << 5) + hash) + c;

	hash = ((hash << 5) + hash) + numThreads;
	hash = ((hash << 5) + hash) + circuitDepth;
	hash = ((hash << 5) + hash) + numQubits;
	hash = ((hash << 5) + hash) + numRepetitions;
	return hash;
}


int main(int narg, char *varg[])
{

	/*
	 * prepare simulation parameters
	 */

	// check all cmd arguments were passed
	int numInpKeys = sizeof(INPUT_KEYS) / sizeof(char *);
	printf("narg: %d\n",narg);
	printf("numInpKeys: %d\n",numInpKeys);

	if (narg - 1 != numInpKeys)
	{
		printf("Call as ./newRandomCircuitBenchmarker");
		for (int i = 0; i < numInpKeys; i++)
			printf(" %s", INPUT_KEYS[i]);
		printf("\n");
		return EXIT_FAILURE;
	}

	// read in cmd args, int'ing some of them
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

	// create the output file (to indicate launch)
	FILE *file = openDictWrite(filename);


	/*
	writeInputsToFile(file,
					  platform, framework, filename,
					  numThreads, circuitDepth, numQubits, numRepetitions, rSeed);
	closeDictWrite(file);
	*/

	// display simulation params
	printf("Simulating random circuit with params:\n");
	printf("platform:\t%s\n", platform);
	printf("framework:\t%s\n", framework);
	printf("filename:\t%s\n", filename);
	printf("numThreads:\t%d\n", numThreads);
	printf("circuitDepth:\t%d\n", circuitDepth);
	printf("numQubits:\t%d\n", numQubits);
	printf("numRepetitions:\t%d\n", numRepetitions);
	printf("randomSeed:\t%lu\n", rSeed);

	/*
	 * prepare quantum simulator and data structutes
	 */

	// seed RandomCircuits
	srand(rSeed);

	// construct simulator
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
		currRealMems[i] = currVirtMems[i] = 0;
		peakRealMems[i] = peakVirtMems[i] = 0;
	}

	/* 
	 * simulate random circuit and collect data
	 */
	for (int trial = 0; trial < numRepetitions; trial++)
	{

		durations[trial] = (double)applyRandomCircuit(qureg, circuitDepth);
		normErrors[trial] = 1 - calcTotalProb(qureg);
		getMemory(
			&currRealMems[trial], &peakRealMems[trial],
			&currVirtMems[trial], &peakVirtMems[trial]);

		printf("dur: %fs, error: %2.e,  ", durations[trial], normErrors[trial]);
		printf("realMem: %lub (peak: %lub), ", currRealMems[trial], peakRealMems[trial]);
		printf("virtMem: %lub (peak: %lub)\n", currVirtMems[trial], peakVirtMems[trial]);
	}

	if (env.rank == 0)
		printf("Here I am 0\n");
	printf("here 1\n");

	/*
	 * write collected data to file
	 */
	file = openDictWrite(filename);
// 
	// writeInputsToFile(file,
	// 				  platform, framework, filename,
	// 				  numThreads, circuitDepth, numQubits, numRepetitions, rSeed);

	writeDoubleArrToDict(file, "durations", durations, numRepetitions);//, 5);
	writeDoubleArrToDict(file, "normErrors", normErrors, numRepetitions);//, 5);
	writeUnsignedLongArrToDict(file, "currRealMemories", currRealMems, numRepetitions);
	writeUnsignedLongArrToDict(file, "peakRealMemories", peakRealMems, numRepetitions);
	writeUnsignedLongArrToDict(file, "currVirtMemories", currVirtMems, numRepetitions);
	writeUnsignedLongArrToDict(file, "peakVirtMemories", peakVirtMems, numRepetitions);
	closeDictWrite(file);

	double avgDurations = avgDoubleArr(durations, numRepetitions);
	unsigned long avgCurrRealMems = avgUnsignedLongArr(currRealMems, numRepetitions);
	unsigned long avgCurrVirtMems = avgUnsignedLongArr(currVirtMems, numRepetitions);

	/*
	file = openDictWrite(filename);
	fprintf(file, "%d %d %d %d %f %lu %lu\n", numThreads, circuitDepth, numQubits, numRepetitions, avgDurations, avgCurrRealMems, avgCurrVirtMems);
	closeDictWrite(file);
	*/

	/* 
	 * free memory 
	 */

	destroyQureg(qureg, env);
	destroyQuESTEnv(env);

	free(durations);
	free(normErrors);
	free(currRealMems);
	free(peakRealMems);
	free(currVirtMems);
	free(peakVirtMems);

	return EXIT_SUCCESS;
}

