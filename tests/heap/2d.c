#include <stdio.h>
#include <stdlib.h>
#include "/home/pavlosaimoniotis/valgrind/gleipnir/gleipnir.h"

#define N 32 

int main(){

int *arr, i, j;
	arr = (int *) malloc ( N * N * sizeof(int) );
	GL_START;
	for (i=0; i < N; i++){
		for (j=0; j < N; j++){
			*(arr+i*N+j) = i;
		}
	}
	for (j=0; j < N; j++){
		for (i=0; i < N; i++){
			*(arr+i*N+j) = i;
		}
	}
	for (i=0; i < N; i++){
		for (j=0; j < N; j++){
			*(arr+i*N+j) = i;
		}
	}
	for (j=0; j < N; j++){
		for (i=0; i < N; i++){
			*(arr+i*N+j) = i;
		}
	}
	GL_STOP;
	free(arr);	
return 0;
}
