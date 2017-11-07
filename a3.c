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
#	pragma omp parallel for num_threads(numThreads) reduction(+: num)
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

		/* initialize matriceis */
		int ** m1 = initMatrix(100);
		int ** m2 = initMatrix(1000);
		int ** m3 = initMatrix(10000);
		int ** m4 = initMatrix(20000);
		/* initialize vectors */
		int * v1 = initVector(100);
		int * v2 = initVector(1000);
		int * v3 = initVector(10000);
		int * v4 = initVector(20000);

		printf("Size        Threads\n");
		printf("       1       2       4\n");
		time_t t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12;

		time_t start = time(NULL);
		int * sum1 = multiply(m1,v1,100,1);
		time_t end = time(NULL);
		t1 = end - start;
		start = time(NULL);
		int * sum2 = multiply(m1,v1,100,2);
		end = time(NULL);
		t2 = end - start;
		start = time(NULL);
		int * sum3 = multiply(m1,v1,100,4);
		end = time(NULL);
		t3 = end - start;
		printf("       %ld       %ld       %ld\n", t1,t2,t3);

		ime_t start = time(NULL);
		int * sum4 = multiply(m2,v2,1000,1);
		time_t end = time(NULL);
		t4 = end - start;
		start = time(NULL);
		int * sum5 = multiply(m2,v2,1000,2);
		end = time(NULL);
		t5 = end - start;
		start = time(NULL);
		int * sum6 = multiply(m2,v2,1000,4);
		end = time(NULL);
		t6 = end - start;
		printf("       %ld       %ld       %ld\n", t4,t5,t6);

		ime_t start = time(NULL);
		int * sum7 = multiply(m3,v3,10000,1);
		time_t end = time(NULL);
		t7 = end - start;
		start = time(NULL);
		int * sum8 = multiply(m3,v3,10000,2);
		end = time(NULL);
		t8 = end - start;
		start = time(NULL);
		int * sum9 = multiply(m3,v3,10000,4);
		end = time(NULL);
		t9 = end - start;
		printf("       %ld       %ld       %ld\n", t7,t8,t9);

		ime_t start = time(NULL);
		int * sum10 = multiply(m4,v4,20000,1);
		time_t end = time(NULL);
		t10 = end - start;
		start = time(NULL);
		int * sum11 = multiply(m4,v4,20000,2);
		end = time(NULL);
		t11 = end - start;
		start = time(NULL);
		int * sum12 = multiply(m4,v4,20000,4);
		end = time(NULL);
		t12 = end - start;
		printf("       %ld       %ld       %ld\n", t10,t11,t12);

		deleteMatrix(sum1,100);
		deleteMatrix(sum2,100);
		deleteMatrix(sum3,100);
		deleteMatrix(sum4,1000);
		deleteMatrix(sum5,1000);
		deleteMatrix(sum6,1000);
		deleteMatrix(sum7,10000);
		deleteMatrix(sum8,10000);
		deleteMatrix(sum9,10000);
		deleteMatrix(sum10,20000);
		deleteMatrix(sum11,20000);
		deleteMatrix(sum12,20000);

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