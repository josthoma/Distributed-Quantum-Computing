
#include <stdio.h>
#include <stdlib.h>
#include "../QuEST/include/QuEST.h"




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



void writeAvgDoubleArray(
    FILE *file,
    char *key,
    double *valueArr,
    int numRepetitions)
{
    fprintf(file, "%s:", key);
    double valueAvg = 0;
    for (int i = 0; i < numRepetitions; i++) {
        valueAvg += valueArr[i];
    }
    valueAvg = valueAvg/numRepetitions;
    fprintf(file, " %lf\n", valueAvg);
}




void writeAvgUnsignedLongArray(
    FILE *file,
    char *key,
    unsigned long *valueArr,
    int numRepetitions)
{
    unsigned long valueAvg = 0;
    fprintf(file, "%s:", key);
    for (int i = 0; i < numRepetitions; i++) {
        valueAvg += valueArr[i];
    }
    valueAvg = valueAvg/numRepetitions;
    fprintf(file, " %lu\n", valueAvg);
}










void writeStringToDict(
    FILE *file,
    char *key,
    char *value)
{
    fprintf(file, "%s: %s\n", key, value);
}

void writeIntToDict(
    FILE *file,
    char *key,
    double value)
{
    fprintf(file, "%s: %f\n", key, value);
}

void writeDoubleArrToDict(
    FILE *file,
    char *key,
    double *valueArr,
    int numRepetitions)
{
    fprintf(file, "%s:", key);
    for (int i = 0; i < numRepetitions; i++) {
        fprintf(file, " %lf", valueArr[i]);
    }
    fprintf(file, "\n");
}

void writeUnsignedLongArrToDict(
    FILE *file,
    char *key,
    unsigned long *valueArr,
    int numRepetitions)
{
    fprintf(file, "%s:", key);
    for (int i = 0; i < numRepetitions; i++) {
        fprintf(file, " %lu", valueArr[i]);
    }
    fprintf(file, "\n");
}

FILE *openDictWrite(
    char *filename)
{
    return fopen(filename, "a+");
}
void *closeDictWrite(
    FILE *file)
{
    fclose(file);
}

double avgDoubleArr(
    double *array,
    int length)
{
    double res = 0;
    for (int i = 0; i < length; i++) {
        res += array[i];
    }
    return res/length;
}

double avgUnsignedLongArr(
    unsigned long *array,
    int length)
{
    unsigned long res = 0;
    for (int i = 0; i < length; i++) {
        res += array[i];
    }
    return res/length;
}
