/** @file 
 * Reads your C's mac process' memory usage
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newMemorymeasure.h"


/*
 * Measures the current (and peak) resident and virtual memory
 * usage of your linux C process, in bytes, accurate to nearest kB.
 * Returns a 0 if memory info access was successful, else prints
 * an error message and returns 1
 */

// in size of Bytes
unsigned long sizeStringToInteger(char *string) {
    char unit = string[strlen(string)-1];
    char value[10];
    strncpy(value, string, strlen(string)-1);
    switch(unit) {
        case 'K': return (unsigned long) atof(value)*1024;
        case 'M': return (unsigned long) atof(value)*1024*1024;
        case 'G': return (unsigned long) atof(value)*1024*1024*1024;
        default: return (unsigned long) atof(value);
    }
}

int getMemory(
	unsigned long *currRealMem, unsigned long *peakRealMem,
	unsigned long *currVirtMem, unsigned long *peakVirtMem) {
	
	// stores each word in status file
	char buffer[1024] = "";
	
	// linux file contains this-process info
    char cmd[100];
    sprintf(cmd, "vmmap %d -summary > memoryFile", getpid());
    int status = system(cmd);

	FILE* file = NULL;
	file = fopen("memoryFile", "r");
	if (file == NULL) {
		printf("Call to getMemory FAILED; "
			   "linux file proc/self/status not found!\n");
		return 1;
	}

    char virtSize[10];
    char realSize[10];
	// read the entire file, recording mems in kB
    while(fscanf(file, "%1023s", buffer) == 1) {
        if (strcmp(buffer, "TOTAL") == 0) {
            fscanf(file, "%s", virtSize);
            fscanf(file, "%s", realSize);
            *currVirtMem = sizeStringToInteger(virtSize);
            *currRealMem = sizeStringToInteger(realSize);
            *peakVirtMem = sizeStringToInteger(virtSize);
            *peakRealMem = sizeStringToInteger(realSize);
        }
    }
    fclose(file);

    system("rm memoryFile");
	return 0;
}


// int main(int narg, char **argv) {
//     unsigned long a = 0;
//     unsigned long b = 0;
//     unsigned long c = 0;
//     unsigned long d = 0;
//     getMemory(&a, &b, &c, &d);
//     return 0;
// }