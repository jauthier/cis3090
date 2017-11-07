#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --------- Matrix Functions --------- */

int ** initMatrix(int size){
	srand(time(NULL));
	int ** matrix = malloc(sizeof(int *)*size);
	int i = 0;
	for (i=0;i<size;i++){
		matrix[i] = malloc(sizeof(int)*size);
		int j = 0;
		for (j=0;j<size;j++){
			//get random num
			int num = rand()%10;
			matrix[i][j] = num;
		}
	}
	return matrix;
}

void printMatrix(int ** matrix, int size){
	int i = 0;
	for (i=0;i<size;i++){
		int j = 0;
		for (j=0;j<size;j++){
			printf("%d", matrix[i][j]);
		}
		printf("\n");
	}	
}

void deleteMatrix(int ** matrix, int size){
	int i = 0;
	for (i=0;i<size;i++){
		free(matrix[i]);
	}
	free(matrix);
}

/* --------- Vector Functions --------- */

int * initVector(int size){\
	srand(time(NULL));
	int * vector = malloc(sizeof(int)*size);
	int i = 0;
	for (i=0;i<size;i++){
		//get random num
		int num = rand()%10;
		vector[i] = num;
	}
	return vector;
}

void printVector(int * vector, int size){
	int i = 0;
	for (i=0;i<size;i++){
		printf("%d\n", vector[i]);
	}
}

void deleteVector(int * vector, int size){
	free(vector);
}


int main(int argc, char * argv[]){
	
	if (argc < 1){
		printf("Please provide the number of threads and the size of the the matrix.\n");
		exit(0);
	}
	int size = 5;
	int ** matrix = initMatrix(size);
	printMatrix(matrix, size);
	deleteMatrix(matrix, size);

	printf("\n");
	int * vector = initVector(size);
	printVector(vector,size);
	deleteVector(vector,size);
	return 0;
}