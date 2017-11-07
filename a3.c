#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

/* --------- Matrix Functions --------- */

int ** initMatrix(int size){
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

/* --------- Matrix Multiplication --------- */

int * multiply(int ** matrix, int * vector, int size, int numThreads){
	int * sum = malloc(sizeof(int)*size);
	int i = 0;
	for (i=0;i<size;i++){
		int num = 0;
		int j = 0;
#	pragma omp parallel for num_threads(numThreads) \ reduction(+: num)
		for (j=0;j<size;j++){
			num = num + (matrix[i][j]*vector[j]);
		}
		sum[i] = num;
	}
	return sum;
}



int main(int argc, char * argv[]){
	
	if (argc < 2){
		printf("Please provide the number of threads and the size of the the matrix.\n");
		exit(0);
	}

	srand(time(NULL));
	if (strcmp(argv[1], "-g")==0){

		/*int ** m1 = initMatrix(100);
		int ** m2 = initMatrix(1000);
		int ** m3 = initMatrix(10000);
		int ** m4 = initMatrix(20000);

		int * v1 = initVector(100);
		int * v2 = initVector(1000);
		int * v3 = initVector(10000);
		int * v4 = initVector(20000);

		time_t start = time(NULL);*/


	} else {
		if (argc != 3){
			printf("Please provide the number of threads and the size of the the matrix.\n");
			exit(0);
		}
		int numThreads = atoi(argv[1]);
		int size = atoi(argv[2]);
	
		int ** matrix = initMatrix(size);
		int * vector = initVector(size);
		int * sum = multiply(matrix, vector, size, numThreads);
		printMatrix(matrix, size);
		printf("\nX\n");
		printVector(vector,size);
		printf("\n=\n");
		printVector(sum,size);
		deleteMatrix(matrix, size);
		deleteVector(vector,size);
		deleteVector(sum,size);
	}

	return 0;
}