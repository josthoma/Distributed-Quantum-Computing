#include <stdio.h>
#include <stdlib.h>
#include "../QuEST/include/QuEST.h"

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
    int numRepetitions);


void writeAvgUnsignedLongArray(
    FILE *file,
    char *key,
    unsigned long *valueArr,
    int numRepetitions);

void writeAvgDoubleArray(
    FILE *file,
    char *key,
    double *valueArr,
    int numRepetitions);

void writeAvgDoubleArray_check(
    FILE *file,
    char *key,
    double *valueArr,
    int numRepetitions,
    QuESTEnv env);


void writeStringToDict(
    FILE *file,
    char *key,
    char *value);

void writeIntToDict(
    FILE *file,
    char *key,
    double value);


void writeDoubleArrToDict(
    FILE *file,
    char *key,
    double *valueArr,
    int numRepetitions);

void writeUnsignedLongArrToDict(
    FILE *file,
    char *key,
    unsigned long *valueArr,
    int numRepetitions);

FILE *openDictWrite(
    char *filename);

void closeDictWrite(
    FILE* file);

double avgDoubleArr(
    double *array,
    int length);

double avgUnsignedLongArr(
    unsigned long *array,
    int length);