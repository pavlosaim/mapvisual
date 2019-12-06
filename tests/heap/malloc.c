#include <stdio.h>
#include <stdlib.h>
#include "/home/pavlosaimoniotis/valgrind/gleipnir/gleipnir.h"

#define N 10

int main(){

int *arr, i;
	i=3;
	arr = (int *) malloc ( N * sizeof(int) );
	GL_START;
	for (i=0; i < N; i++){
			*(arr+i) = i;
	}
	GL_STOP;
	free(arr);	
return 0;
}
