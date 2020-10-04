/** @file
 * Applies a random circuit
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#include "newRandomCircuits.h"
#include "../../QuEST/QuEST/include/QuEST.h"

const long double SQRT2 = 1.41421356237309504880168872420969807856967187537694;

int getRandomInteger(int min, int max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// circuitDepth becomes: i^th qubit Gate
long double applyRandomCircuit(Qureg qureg, int circuitDepth, QuESTEnv env)
{
    /*
     * PREPARE DATA STRUCTURES
     */

    // remembers previous gates
    int numQubits = qureg.numQubitsRepresented;

    // arguments for X^1/2 and Y^1/2 qubit rotations
    Complex alphaXY;
    alphaXY.real = 1 / SQRT2;
    alphaXY.imag = 0;
    Complex betaX, betaY;
    betaX.real = 0;
    betaX.imag = -1 / SQRT2;
    betaY.real = 1 / SQRT2;
    betaY.imag = 0;

    /*
     * PERFORM RANDOM GATE ALGORITHM
     */

    // start timing execution
    struct timeval timeInstance;

    // populate all states
    initPlusState(qureg);

    // Start Time
    gettimeofday(&timeInstance, NULL);
    long double startTime = (timeInstance.tv_sec +
                             (long double)timeInstance.tv_usec / pow(10, 6));

    compactUnitary(qureg, circuitDepth, alphaXY, betaX, env); // Teng checked

    /* ----------------- Keep These Lines, it is for checking correctness ----------------- */
    // for (int i = 0; i < qureg.numAmpsPerChunk; i++)
    //     printf("rank %d, %f  %f I\n", env.rank, qureg.stateVec.real[i], qureg.stateVec.imag[i]);

    // End Time
    gettimeofday(&timeInstance, NULL);
    long double endTime = (timeInstance.tv_sec +
                           (long double)timeInstance.tv_usec / pow(10, 6));

    return endTime - startTime;
}

// void repeatedlyTimeRandomCircuit(
//     QuESTEnv env, int numQubits,
//     int circuitDepth, int numRepetitions,
//     double *avgDuration, double *varDuration)
// {
//     // prepare qubits
//     Qureg qureg = createQureg(numQubits, env);
//     // repeat algorithm, recording duration
//     long double duration;
//     double durationSum = 0;
//     double durationSquaredSum = 0;
//     for (int i = 0; i < numRepetitions; i++)
//     {
//         duration = applyRandomCircuit(qureg, circuitDepth);
//         printf("%d: %Lf\n",i,duration);
//         durationSum += duration;
//         durationSquaredSum += pow(duration, 2);
//     }

//  // compute average and variance
//  *avgDuration = durationSum / numRepetitions;
//  *varDuration = (durationSquaredSum / numRepetitions - pow(*avgDuration, 2));

//  // free qubits
//  destroyQureg(qureg, env);
// }

// int main() {
//     QuESTEnv env = createQuESTEnv();
//     int numQubits = 5;
//     int circuitDepth = 10;
//     int numRepetitions = 12;
//     double avgDuration = 0;
//     double varDuration = 0;
//     repeatedlyTimeRandomCircuit(env, numQubits, circuitDepth, numRepetitions, &avgDuration, &varDuration);
//     return 0;
// }
