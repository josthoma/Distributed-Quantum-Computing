#include<stdio.h>
#include <omp.h>
#include<sys/time.h>
#include<math.h>

int main() {
	double x = omp_get_wtime();
	printf("%f\n",x);

	struct timeval timeInstance;
    gettimeofday(&timeInstance, NULL);
    long double endTime = (
   		timeInstance.tv_sec + 
        (long double) timeInstance.tv_usec/pow(10,6));
	printf("%Lf\n", endTime);



}
