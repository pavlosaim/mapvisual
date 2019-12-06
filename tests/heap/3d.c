#include <stdio.h>
#include <stdlib.h>
#include "/home/pavlosaimoniotis/valgrind/gleipnir/gleipnir.h"

#define ROWS 10 
#define COLS 20
#define DEPTH 10
int main(){

int *arr, i, j, k;
	arr = (int *) malloc ( ROWS * COLS * DEPTH *  sizeof(int) );
	GL_START;
	for (i=0; i < ROWS; i++){
		for (j=0; j < COLS; j++){
			for (k=0; k < DEPTH; k++){
				*(arr+i*(COLS*DEPTH)+j*DEPTH+k) = i;
			}
		}
	}
	GL_STOP;
	free(arr);	
return 0;
}
