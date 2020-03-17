#include <stdio.h>
#include <stdlib.h>
#include "/home/path/to/valgrind/gleipnir/gleipnir.h"

#define N 10

int main(){

int *arr, i, *pin;
double *array, *dif, *sky;
	i=3;
	arr = (int *) malloc ( N * sizeof(int) );
	pin = (int *) malloc ( N * sizeof(int) );
	dif = (double *) malloc ( 2 * N * sizeof(double) );
	sky = (double *) malloc ( 2 * N * sizeof(double) );
	array = (double *) malloc ( 2 * N * sizeof(double));
	GL_START;
	for (i=0; i < N; i++){
			*(arr+i) = i;
			*(pin+i) = i;
			*(array+i) = *(arr+i);
	}
	for (i=0; i < 2 * N; i++){
			*(array+i) = i;
			*(dif+i) = *(array+i);
			*(sky+i) = *(dif+i);
	}
	GL_STOP;
	free(arr);	
return 0;
}
