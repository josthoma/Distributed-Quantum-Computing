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


const long double SQRT2 =   1.41421356237309504880168872420969807856967187537694;

enum gateID {X, Y, T, H, C, None};


int getRandomInteger(int min, int max) {
    return min + rand()/(RAND_MAX/(max - min + 1) + 1);
}

enum gateID pickRandomGate(enum gateID prevGate) {
    
    int choice = getRandomInteger(0,1);
    switch(prevGate) {
        case T:  return (choice)? X:Y;
        case X:  return (choice)? Y:T;
        case Y:  return (choice)? T:X;
        default: break;
    }
    
    choice = getRandomInteger(0,2);
    if (choice == 0)
        return X;
    if (choice == 1)
        return Y;
    return T;
}


long double applyRandomCircuit(Qureg qureg, int circuitDepth) {
    /*
     * PREPARE DATA STRUCTURES
     */

    printf("n 0\n");
     
    // remembers previous gates
    int numQubits = qureg.numQubitsRepresented;
    int *hasToffo = malloc(numQubits * sizeof(int));
    enum gateID *prevGates = malloc(numQubits * sizeof(enum gateID));
    enum gateID *prevSings = malloc(numQubits * sizeof(enum gateID));
    enum gateID *currGates = malloc(numQubits * sizeof(enum gateID));
    for (int i=0; i < numQubits; i++) {
        hasToffo[i] = 0;
        prevGates[i] = None;
        prevSings[i] = T;
        currGates[i] = H;
    }

    printf("n 1\n");
    
    // arguments for X^1/2 and Y^1/2 qubit rotations
    Complex alphaXY; 
    alphaXY.real = 1/SQRT2; alphaXY.imag = 0;
    Complex betaX, betaY;
    betaX.real = 0;         betaX.imag = -1/SQRT2;
    betaY.real = 1/SQRT2;   betaY.imag = 0;
    
    
    /*
     * PERFORM RANDOM GATE ALGORITHM
     */
     

    // start timing execution
    struct timeval timeInstance;
    gettimeofday(&timeInstance, NULL);
    long double startTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    long double prevTime = startTime;
    
    printf("n 1.1\n");

    // populate all states
    initPlusState(qureg);

    printf("n 1.2\n");

    gettimeofday(&timeInstance, NULL);
    long double currTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    // printf("initPlusState: %Lf\n",currTime - startTime);
    prevTime = currTime;
    
    printf("n 2\n");


    
    int cStartInd = -1;

    long double twoTime = 0.;
    long double singleTime =0.;
	long double time_tsum = 0.;
	long double time_xsum = 0.;
	long double time_ysum = 0.;

    for (int depth=0; depth < circuitDepth; depth++) {
        printf("================= Depth = %d ================\n",depth);
    
        printf("%d\n",numQubits);
        // update gates
        for (int i=0; i < numQubits; i++) {
            printf("n 2,0, %d\n", i);
            if (currGates[i] == T || currGates[i] == H) {
                printf("c 1\n");
            }
            else {
                printf("c 2\n");
            }
            prevGates[i] = currGates[i];
            printf("n 2,1, %d\n", i);
            currGates[i] = None;
            printf("n 2,2, %d\n", i);
        }
        
        // apply CZs


    printf("n 3\n");
        cStartInd += 1;
        cStartInd %= 3;
        for (int n=cStartInd; n<numQubits-1; n+=3) {

            printf("n 3, %d\n",n);

            // Time 
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            prevTime = currTime;

            printf("n 3, 1, %d\n",n);
            controlledPhaseFlip(qureg, n, n+1);
            printf("n 3, 2, %d\n",n);

            // Time 
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            twoTime += currTime - prevTime;

            currGates[n] = currGates[n+1] = C;
        }

    printf("n 4\n");


        
        // iterate only the not-C-gated qubits
        for (int n=0; n < numQubits; n++) {
            

            if (currGates[n] != None || prevGates[n] != C)
                continue;
            

            // apply Toffoli gates
            if (!hasToffo[n]) {

                // Time 
                gettimeofday(&timeInstance, NULL);
                currTime = (
                    timeInstance.tv_sec + 
                    (long double) timeInstance.tv_usec/pow(10,6));
                prevTime = currTime;

                tGate(qureg, n);

                // Time 
                gettimeofday(&timeInstance, NULL);
                currTime = (
                    timeInstance.tv_sec + 
                    (long double) timeInstance.tv_usec/pow(10,6));
                time_tsum += currTime - prevTime;

                hasToffo[n] = 1;
                prevSings[n] = T;
                currGates[n] = T;
            }
            // appply random gates
            else {
                enum gateID gate = pickRandomGate(prevSings[n]);
                prevSings[n] = gate;
                currGates[n] = gate;
                if (gate == T) {

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    prevTime = currTime;

                    tGate(qureg, n);

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    time_tsum += currTime - prevTime;
				}

                if (gate == X) {// apply sqrt_X

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    prevTime = currTime;

                    compactUnitary(qureg, n, alphaXY, betaX); // Teng checked


                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    time_xsum += currTime - prevTime;
				}


                if (gate == Y) {// apply sqrt_Y

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    prevTime = currTime;


                    compactUnitary(qureg, n, alphaXY, betaY); // Teng checked


                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    time_ysum += currTime - prevTime;

				}

            }


        }
    }
    // stop timing execution
    gettimeofday(&timeInstance, NULL);
    long double endTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    
    
    // printf("twoTime: %Lf\n",twoTime);
    // printf("tTime: %Lf\n",time_tsum);
    // printf("xTime: %Lf\n",time_xsum);
    // printf("yTime: %Lf\n",time_ysum);



    // if (qureg.numQubitsInStateVec == 2) {
    //     FILE* fid = fopen("saveFinalState.txt","a+");
    //     for (int findex = 0; findex < 4; findex++) {
    //         fprintf(fid,"%f + 1.j * %f\n",qureg.stateVec.real[findex], qureg.stateVec.imag[findex]);
    //     }
    //     fclose(fid);
    // }

    
    /*
     * FREE MEMORY
     */
    
    free(hasToffo);
    free(prevGates);
    free(prevSings);
    free(currGates);
    
    return endTime - startTime;
}




long double applyRandomCircuit_debug(Qureg* qureg_p, int circuitDepth) {
    /*
     * PREPARE DATA STRUCTURES
     */

    Qureg qureg = *qureg_p;

    printf("n 0\n");
     
    // remembers previous gates
    int numQubits = qureg.numQubitsRepresented;
    int *hasToffo = malloc(numQubits * sizeof(int));
    enum gateID *prevGates = malloc(numQubits * sizeof(enum gateID));
    enum gateID *prevSings = malloc(numQubits * sizeof(enum gateID));
    enum gateID *currGates = malloc(numQubits * sizeof(enum gateID));
    for (int i=0; i < numQubits; i++) {
        hasToffo[i] = 0;
        prevGates[i] = None;
        prevSings[i] = T;
        currGates[i] = H;
    }
    printf("n 1\n");
    
    // arguments for X^1/2 and Y^1/2 qubit rotations
    Complex alphaXY; 
    alphaXY.real = 1/SQRT2; alphaXY.imag = 0;
    Complex betaX, betaY;
    betaX.real = 0;         betaX.imag = -1/SQRT2;
    betaY.real = 1/SQRT2;   betaY.imag = 0;
    
    
    /*
     * PERFORM RANDOM GATE ALGORITHM
     */
     

    // start timing execution
    struct timeval timeInstance;
    gettimeofday(&timeInstance, NULL);
    long double startTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    long double prevTime = startTime;
    
    printf("n 1.1\n");

    // populate all states
    initPlusState(qureg);

    printf("n 1.2\n");

    gettimeofday(&timeInstance, NULL);
    long double currTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    // printf("initPlusState: %Lf\n",currTime - startTime);
    prevTime = currTime;
    
    printf("n 2\n");


    
    int cStartInd = -1;

    long double twoTime = 0.;
    long double singleTime =0.;
	long double time_tsum = 0.;
	long double time_xsum = 0.;
	long double time_ysum = 0.;

    for (int depth=0; depth < circuitDepth; depth++) {
        printf("================= Depth = %d ================\n",depth);
    
        // update gates
        for (int i=0; i < numQubits; i++) {
            prevGates[i] = currGates[i];
            currGates[i] = None;
        }
        
        // apply CZs


        cStartInd += 1;
        cStartInd %= 3;
        for (int n=cStartInd; n<numQubits-1; n+=3) {

            // Time 
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            prevTime = currTime;

            controlledPhaseFlip(qureg, n, n+1);// Teng checked

            // Time 
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            twoTime += currTime - prevTime;

            currGates[n] = currGates[n+1] = C;
        }

    printf("n 3\n");


        
        // iterate only the not-C-gated qubits
        for (int n=0; n < numQubits; n++) {
            

            if (currGates[n] != None || prevGates[n] != C)
                continue;
            

            // apply Toffoli gates
            if (!hasToffo[n]) {

                // Time 
                gettimeofday(&timeInstance, NULL);
                currTime = (
                    timeInstance.tv_sec + 
                    (long double) timeInstance.tv_usec/pow(10,6));
                prevTime = currTime;

                tGate(qureg, n);

                // Time 
                gettimeofday(&timeInstance, NULL);
                currTime = (
                    timeInstance.tv_sec + 
                    (long double) timeInstance.tv_usec/pow(10,6));
                time_tsum += currTime - prevTime;

                hasToffo[n] = 1;
                prevSings[n] = T;
                currGates[n] = T;
            }
            // appply random gates
            else {
                enum gateID gate = pickRandomGate(prevSings[n]);
                prevSings[n] = gate;
                currGates[n] = gate;
                if (gate == T) {

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    prevTime = currTime;

                    tGate(qureg, n);

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    time_tsum += currTime - prevTime;
				}

                if (gate == X) {// apply sqrt_X

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    prevTime = currTime;

                    compactUnitary(qureg, n, alphaXY, betaX); // Teng checked


                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    time_xsum += currTime - prevTime;
				}


                if (gate == Y) {// apply sqrt_Y

                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    prevTime = currTime;


                    compactUnitary(qureg, n, alphaXY, betaY); // Teng checked


                    // Time 
                    gettimeofday(&timeInstance, NULL);
                    currTime = (
                        timeInstance.tv_sec + 
                        (long double) timeInstance.tv_usec/pow(10,6));
                    time_ysum += currTime - prevTime;

				}

            }


        }
    }
    // stop timing execution
    gettimeofday(&timeInstance, NULL);
    long double endTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    
    
    // printf("twoTime: %Lf\n",twoTime);
    // printf("tTime: %Lf\n",time_tsum);
    // printf("xTime: %Lf\n",time_xsum);
    // printf("yTime: %Lf\n",time_ysum);



    // if (qureg.numQubitsInStateVec == 2) {
    //     FILE* fid = fopen("saveFinalState.txt","a+");
    //     for (int findex = 0; findex < 4; findex++) {
    //         fprintf(fid,"%f + 1.j * %f\n",qureg.stateVec.real[findex], qureg.stateVec.imag[findex]);
    //     }
    //     fclose(fid);
    // }

    
    /*
     * FREE MEMORY
     */
    
    free(hasToffo);
    free(prevGates);
    free(prevSings);
    free(currGates);
    
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
