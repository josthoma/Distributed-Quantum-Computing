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

static inline int extractBit_copy (const int locationOfBitFromRight, const long long int theEncodedNumber) {
    return (theEncodedNumber & ( 1LL << locationOfBitFromRight )) >> locationOfBitFromRight;
}





void statevec_controlledPhaseFlip_debug (Qureg qureg, const int idQubit1, const int idQubit2)
{
    printf("r 0\n");
    long long int index;
    long long int stateVecSize;
    int bit1, bit2;

    const long long int chunkSize=qureg.numAmpsPerChunk;
    const long long int chunkId=qureg.chunkId;
    
    // dimension of the state vector
    stateVecSize = qureg.numAmpsPerChunk;
    qreal *stateVecReal = qureg.stateVec.real;
    qreal *stateVecImag = qureg.stateVec.imag;

    // printf("%f\n",qureg.stateVec.real[0]);
    // printf("%f\n",qureg.stateVec.real[1]);
    // printf("%f\n",qureg.stateVec.real[2]);
    // printf("%f\n",qureg.stateVec.real[3]);

    printf("%d\n",stateVecSize);
# ifdef _OPENMP
# pragma omp parallel for \
    default  (none)              \
    shared   (stateVecSize, stateVecReal,stateVecImag ) \
    private  (index,bit1,bit2)             \
    schedule (static)
# endif
    for (index=0; index<stateVecSize; index++) {
        printf("%d\n",index);
        bit1 = extractBit_copy (idQubit1, index+chunkId*chunkSize);
        bit2 = extractBit_copy (idQubit2, index+chunkId*chunkSize);
        printf("r 1\n");
        if (bit1 && bit2) {
            printf("%d %d\n", bit1, bit2);
            printf("r 1.1\n");
            stateVecReal [index] = - stateVecReal [index];
            printf("r 1.2\n");
            stateVecImag [index] = - stateVecImag [index];
        }
        printf("r 2\n");
    }

}

void controlledPhaseFlip_debug(Qureg qureg, const int idQubit1, const int idQubit2) {
    printf("q 0\n");
    statevec_controlledPhaseFlip_debug(qureg, idQubit1, idQubit2);
    // statevec_controlledPhaseFlip(qureg, idQubit1, idQubit2);
    printf("q 1\n");
}

int main(int narg, char *varg[]) {
	char *platform = varg[1];
	char *framework = varg[2];
	char *filename = varg[3];
	int numThreads = atoi(varg[4]);
	int circuitDepth = atoi(varg[5]);
	int numQubits = atoi(varg[6]);
	int numRepetitions = atoi(varg[7]);

	QuESTEnv env = createQuESTEnv();
	Qureg qureg;
	createQureg_debug(&qureg, numQubits, env);

	printf("%f\n",qureg.stateVec.real[3]);
	printf("%f\n",qureg.stateVec.imag[3]);

	printf("j 0\n");
	initPlusState(qureg);
	printf("j 1\n");
	controlledPhaseFlip_debug(qureg, 0, 1);
	printf("j 2\n");
	tGate(qureg, 0);
	printf("j 3\n");

// 	// --------------------------------------------
// 	int idQubit1 = 0;
// 	int idQubit2 = 1;
// 	   printf("r 0\n");
//     long long int index;
//     long long int stateVecSize;
//     int bit1, bit2;

//     const long long int chunkSize=qureg.numAmpsPerChunk;
//     const long long int chunkId=qureg.chunkId;
    
//     // dimension of the state vector
//     stateVecSize = qureg.numAmpsPerChunk;
//     qreal *stateVecReal = qureg.stateVec.real;
//     qreal *stateVecImag = qureg.stateVec.imag;

//     // printf("%f\n",qureg.stateVec.real[0]);
//     // printf("%f\n",qureg.stateVec.real[1]);
//     // printf("%f\n",qureg.stateVec.real[2]);
//     // printf("%f\n",qureg.stateVec.real[3]);

//     printf("%d\n",stateVecSize);
// # ifdef _OPENMP
// # pragma omp parallel for \
//     default  (none)              \
//     shared   (stateVecSize, stateVecReal,stateVecImag ) \
//     private  (index,bit1,bit2)             \
//     schedule (static)
// # endif
//     for (index=0; index<stateVecSize; index++) {
//         printf("%d\n",index);
//         bit1 = extractBit_copy (idQubit1, index+chunkId*chunkSize);
//         bit2 = extractBit_copy (idQubit2, index+chunkId*chunkSize);
//         printf("r 1\n");
//         if (bit1 && bit2) {
//             printf("r 1.1\n");
//             stateVecReal [index] = - stateVecReal [index];
//             printf("r 1.2\n");
//             stateVecImag [index] = - stateVecImag [index];
//         }
//         printf("r 2\n");
//     }
// 	// --------------------------------------------------------------------

// 	printf ("---------------------------------\n");
// 	controlledPhaseFlip(qureg, 0, 1);

	destroyQureg(qureg, env);
	destroyQuESTEnv(env);

	return 1;
}

int main_old(int narg, char *varg[])
{
#ifdef __GNUC__
    // note that clang 3.7 declares itself as a gcc 4.2"
    printf ("gcc detected version %d.%d\n", __GNUC__, __GNUC_MINOR__);
#endif
#ifdef __STDC_VERSION__
    printf("__STDC_VERSION__ = %ld \n", __STDC_VERSION__);
#endif
#ifdef __STRICT_ANSI__
    puts("__STRICT_ANSI__");
#endif

	printf("0\n");

	struct timeval timeInstance;
    gettimeofday(&timeInstance, NULL);
    long double startTime = (
   		timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));


	// cpu_set_t set;
	// CPU_ZERO(&set);        // clear cpu mask
	// CPU_SET(0, &set);      // set cpu 0
	// sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process	
	// printf("%d\n",CPU_COUNT());



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

	printf("1\n");

	// cpu_set_t set;
	// CPU_ZERO(&set);        // clear cpu mask
	// CPU_SET(numThreads, &set);      // set cpu 0
	// sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process	



	// // generate a random seed from hashing params
	// unsigned long rSeed = hashParams(
	// 	platform, framework, filename,
	// 	numThreads, circuitDepth, numQubits, numRepetitions);
	unsigned long rSeed = 0;



	/****************************************************/
	/* ***************** Simulation ******************* */
	/****************************************************/

	// seed RandomCircuits
	srand(rSeed);
	QuESTEnv env = createQuESTEnv();

	// cpu_set_t set;
	// CPU_SET(8, &set);
	// if (env.rank == 1) {
	// 	for (int tindex = 0; tindex < numThreads; tindex++) {
	// 		CPU_SET(tindex, &set);
	// 	}
	// }
	// sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process	


	// printf("p 1\n");
	char rfile[30];
	// printf("p 2\n");
	sprintf(rfile, "result_%d.txt",env.rank);
	// printf("p 3\n");

	FILE* rfid = fopen(rfile,"a+");


	Qureg qureg;
	createQureg_debug(&qureg, numQubits, env);
	// Qureg qureg = createQureg_debug(numQubits, env);
	// printf("%d\n",createQureg(numQubits, env).chunkId);
	// printf("%d\n",qureg.chunkId);
	printf("p 7\n");
	
	// destroyQureg(qureg, env);
	printf("p 8\n");
	// destroyQuESTEnv(env);




	double *durations = malloc(numRepetitions * sizeof(double));
	double *normErrors = malloc(numRepetitions * sizeof(double));

	int memArrSize = numRepetitions * sizeof(unsigned long);
	unsigned long *currRealMems = malloc(memArrSize);
	unsigned long *peakRealMems = malloc(memArrSize);
	unsigned long *currVirtMems = malloc(memArrSize);
	unsigned long *peakVirtMems = malloc(memArrSize);

	// printf("3");

	// clear arrays (so that on sys that don't support memory measure, the output is less confusing)
	for (int i = 0; i < numRepetitions; i++)
	{
		durations[i] = 0;
		currRealMems[i] = currVirtMems[i] = 0;
		peakRealMems[i] = peakVirtMems[i] = 0;
	}


	// simulate random circuit and collect data
	for (int trial = 0; trial < numRepetitions; trial++)
	{
		printf("p 9\n");

		durations[trial] = (double)applyRandomCircuit(qureg, circuitDepth);


		printf("p 10\n");

		printf("Total Circuit: %f\n",durations[trial]);
		getMemory(
			&currRealMems[trial], &peakRealMems[trial],
			&currVirtMems[trial], &peakVirtMems[trial]);

		char buff[100];
		sprintf(buff,"rank: %d, dur: %fs, error: %2.e,  ", env.rank, durations[trial], normErrors[trial]);
		printf(buff);
		fprintf(rfid, buff);
		sprintf(buff,"realMem: %lub (peak: %lub), ", currRealMems[trial], peakRealMems[trial]);
		printf(buff);
		fprintf(rfid, buff);
		sprintf(buff,"virtMem: %lub (peak: %lub)\n", currVirtMems[trial], peakVirtMems[trial]);
		printf(buff);
		fprintf(rfid, buff);
	}

	printf("p 11\n");

	// /****************************************************/
	// /* ***************** Ending *********************** */
	// /****************************************************/


	destroyQureg(qureg, env);
	destroyQuESTEnv(env);
	printf("p 12\n");

	free(durations);

	gettimeofday(&timeInstance, NULL);
    long double endTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    printf("total time: %Lf\n",endTime - startTime);

	fprintf(rfid, "total time: %Lf\n",endTime - startTime);


	// int cpu_num = sched_getcpu();
	// printf("\ncpu_num: %d\n",cpu_num);

	// long double a[4];
	// FILE* fp = fopen("/proc/self/stat","r");
	// fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
	// fclose(fp);
	// printf("%Lf  %Lf %Lf %Lf\n",a[0],a[1],a[2],a[3]);
	// sleep(1);

	int pid = getpid();
	// printf("\npid: %d\n",pid);
	char string[40];
	sprintf(string,"cat /proc/%d/stat > ./stat_%d",pid,pid);
	system(string);
	sprintf(string,"cat /proc/uptime > ./uptime_%d",pid);
	system(string);

	if (env.rank == 0) {
		system("cp -f ../dopython.py ./"); // copy
	}
	sprintf(string,"python dopython.py %d %d",pid, env.rank);
	system(string);

	// sprintf(string,"cp -a /proc/%d/task  ./task_%d_copy",pid,pid);
	// system(string);

	return EXIT_SUCCESS;
}

