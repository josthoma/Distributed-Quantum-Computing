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
    
    // populate all states
    initPlusState(qureg);

    
    gettimeofday(&timeInstance, NULL);
    long double currTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    printf("Time for initPlusState: %Lf\n",currTime-prevTime);
    prevTime = currTime;

    
    int cStartInd = -1;

    long double twoTime = 0.;
    long double singleTime =0.;
	long double time_tsum = 0.;
	long double time_xsum = 0.;
	long double time_ysum = 0.;

    for (int depth=0; depth < circuitDepth; depth++) {

        printf("==================\n");
        printf("depth: %d\n",depth);
    
        // update gates
        for (int i=0; i < numQubits; i++) {
            prevGates[i] = currGates[i];
            currGates[i] = None;
        }
        
        // apply CZs


        cStartInd += 1;
        cStartInd %= 3;
        for (int n=cStartInd; n<numQubits-1; n+=3) {
            // Time recording
            gettimeofday(&timeInstance, NULL);
            long double currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            prevTime = currTime;

            controlledPhaseFlip(qureg, n, n+1);// Teng checked

            // Time recording
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            twoTime += currTime - prevTime;
            prevTime = currTime;



            currGates[n] = currGates[n+1] = C;
        }



        
        // iterate only the not-C-gated qubits
        for (int n=0; n < numQubits; n++) {
            

            if (currGates[n] != None || prevGates[n] != C)
                continue;
            
            // Time recording
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            prevTime = currTime;


            // apply Toffoli gates
            if (!hasToffo[n]) {

				// Time recording
				gettimeofday(&timeInstance, NULL);
				long double time_t1 = (
					timeInstance.tv_sec + 
					(long double) timeInstance.tv_usec/pow(10,6));

                tGate(qureg, n);

				// Time recording
				gettimeofday(&timeInstance, NULL);
				long double time_t2 = (
					timeInstance.tv_sec + 
					(long double) timeInstance.tv_usec/pow(10,6));
				time_tsum += time_t2 - time_t1;



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

					// Time recording
					gettimeofday(&timeInstance, NULL);
					long double time_t1 = (
						timeInstance.tv_sec + 
						(long double) timeInstance.tv_usec/pow(10,6));

                    tGate(qureg, n);

					// Time recording
					gettimeofday(&timeInstance, NULL);
					long double time_t2 = (
						timeInstance.tv_sec + 
						(long double) timeInstance.tv_usec/pow(10,6));
					time_tsum += time_t2 - time_t1;
				}

                if (gate == X) {// apply sqrt_X

					// Time recording
					gettimeofday(&timeInstance, NULL);
					long double time_x1 = (
						timeInstance.tv_sec + 
						(long double) timeInstance.tv_usec/pow(10,6));

                    compactUnitary(qureg, n, alphaXY, betaX); // Teng checked

					// Time recording
					gettimeofday(&timeInstance, NULL);
					long double time_x2 = (
						timeInstance.tv_sec + 
						(long double) timeInstance.tv_usec/pow(10,6));
					time_xsum += time_x2 - time_x1;
				}


                if (gate == Y) {// apply sqrt_Y

					// Time recording
					gettimeofday(&timeInstance, NULL);
					long double time_y1 = (
						timeInstance.tv_sec + 
						(long double) timeInstance.tv_usec/pow(10,6));

                    compactUnitary(qureg, n, alphaXY, betaY); // Teng checked

					// Time recording
					gettimeofday(&timeInstance, NULL);
					long double time_y2 = (
						timeInstance.tv_sec + 
						(long double) timeInstance.tv_usec/pow(10,6));
					time_ysum += time_y2 - time_y1;
				}

            }
            // Time recording
            gettimeofday(&timeInstance, NULL);
            currTime = (
                timeInstance.tv_sec + 
                (long double) timeInstance.tv_usec/pow(10,6));
            singleTime += currTime - prevTime;

        }
        printf("==================\n");
    }

    printf("singleTime: %Lf\n",singleTime);
    printf("tTime: %Lf\n",time_tsum);
    printf("xTime: %Lf\n",time_xsum);
    printf("yTime: %Lf\n",time_ysum);
    printf("twoTime: %Lf\n",twoTime);
    
    // stop timing execution
    gettimeofday(&timeInstance, NULL);
    long double endTime = (
        timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
    
    
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
