#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ** initMatrix(int size){
	srand(time(NULL));
	int ** matrix = malloc(sizeof(int *)*size);
	int i = 0;
	for (i=0;i<size;i++){
		matrix[i] = malloc(sizeof(int)*size);
		int j = 0;
		for (j=0;j<size;j++){
			//get random num
			int num = rand()%21;
			matrix[i][j] = num;
		}
	}
	return matrix;
}

/*int * initVector(int size){

}*/

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

int main(int argc, char * argv[]){
	
	if (argc < 1){
		printf("Please provide the number of threads and the size of the the matrix.\n");
		exit(0);
	}
	int ** matrix = initMatrix(5);
	printMatrix(matrix);
	deleteMatrix(matrix);
	return 0;
}