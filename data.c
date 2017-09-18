#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct param{
	int start;
	int end;
	int gridSize;
} Param;

int ** readGrid;
int ** writeGrid;

void initArrays(int gridSize);
void setArray(int gridSize);


void initArrays(int gridSize){
	//init the arrays
	int i = 0;
	readGrid = malloc(sizeof(int *)*gridSize);
    for(i=0;i<gridSize;i++){
        readGrid[i] = malloc(sizeof(int)*gridSize);
    }

    i = 0;
	writeGrid = malloc(sizeof(int *)*gridSize);
    for(i=0;i<gridSize;i++){
        writeGrid[i] = malloc(sizeof(int)*gridSize);
    }
    setArray(gridSize);
}

void freeArrays(int gridSize){
	int i = 0;
	for(i=0;i<gridSize;i++){
		free(readGrid[i]);
	}
	for(i=0;i<gridSize;i++){
		free(writeGrid[i]);
	}
	free(readGrid);
	free(writeGrid);
}

void setArray(int gridSize){
	int i=0, j=0;
	srand(time(NULL));
	for (i=0;i<gridSize;i++){
		for (j=0; j<gridSize;++j){
			int num = rand()%5;
			if (num == 1){
				readGrid[i][j] = 1;
			} else {
				readGrid[i][j] = 0;
			}
		}
	}
}

void printGrid(int gridSize, int ** grid){
	int i=0, j=0;
	for (i=0;i<gridSize;i++){
		for (j=0; j<gridSize;++j){
			printf("%d", grid[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void nextGen(void * ptr){
	Param * param = (Param*)ptr;
	int i = param->start; 
	int j = 0;
	printf("%d-%d\n", i,param->end);
	for (i=param->start;i<param->end;i++){
		printf("%d\n", i);
		for (j=0; j<param->gridSize;++j){
			int neighbours = 0;
			if (i > 0){
				//check up
				if (readGrid[i-1][j] == 1)
					neighbours++;
				if (j > 0){
					//check upper left
					if (readGrid[i-1][j-1] == 1)
						neighbours++;
				}
				if (j < (param->gridSize-1)){
					//check upper right
					if (readGrid[i-1][j+1] == 1)
						neighbours++;
				}
			}
			if (i < (param->gridSize-1)){
				//check down
				if (readGrid[i+1][j] == 1)
					neighbours++;
				if (j > 0){
					//check lowwer left
					if (readGrid[i+1][j-1] == 1)
						neighbours++;
				}
				if (j < (param->gridSize-1)){
					//check lower right
					if (readGrid[i+1][j+1] == 1)
						neighbours++;
				}	
			}
			if (j > 0){
				//check left
				if (readGrid[i][j-1] == 1)
					neighbours++;
			}
			if (j < (param->gridSize-1)){
				//check right
				if (readGrid[i][j+1] == 1)
					neighbours++;
			}

			printf("%d\n", neighbours);
			if(readGrid[i][j] == 1){
				if(neighbours==2 || neighbours == 3)
					writeGrid[i][j] = 1; //lives
				else 
					writeGrid[i][j] = 0;//dies
			} else {
				if (neighbours == 3)
					writeGrid[i][j] = 1;//bbay
				else 
					writeGrid[i][j] = 0;//nothing
			}
		}
	}
}

void swapGrids(gridSize){
	int i=0, j=0;
	for (i=0;i<gridSize;i++){
		for (j=0; j<gridSize;++j){
			readGrid[i][j] = writeGrid[i][j];
		}
	}
}

int main(int argc, char const *argv[]) {
	
	if (argc < 4){
		printf("Please enter the number of threads, the size of the\ngrid and the number of iterations as command line arguments\n");
		exit(0);
	}

	//up to 4 command line arguments
	int numThreads = atol(argv[1]);
	int gridSize = atol(argv[2]);
	int numIter = atol(argv[3]);
	int display = 0;

	if (numThreads < 1){
		printf("The number of threads must be greater than zero.\n");
		exit(0);
	}

	if (argc == 5){
		if (strcmp(argv[4], "-d")==0){
			display = 1;
		}
	}

	//init the arrays
	initArrays(gridSize);
	if (display == 1)
		printGrid(gridSize, readGrid);

	//do thread things
	long thread = 0;
	pthread_t * threadList = malloc(sizeof(pthread_t)*numThreads);
	Param ** paramList = malloc(sizeof(Param)*numThreads);
	int i;
	for (i=0;i<numIter;i++){

		for (thread=0;thread<numThreads;thread++){
			//get the columns to work on
			int start = (gridSize/numThreads)*thread;
			int end = ((gridSize/numThreads)*(thread+1))-1;
			printf("%d: %d-%d\n", thread,start,end);
			paramList[thread] = malloc(sizeof(Param));
			paramList[i]->start = start;
			paramList[i]->end = end;
			paramList[i]->gridSize = gridSize;
			pthread_create(&threadList[thread], NULL, nextGen,(void*)paramList[i]);
		}
		for (thread = 0; thread < numThreads; thread++){
      		pthread_join(threadList[thread], NULL); 
   		}

		printGrid(gridSize, writeGrid);
		//write over grid
		swapGrids(gridSize);
		//if (display == 1)
		//	printGrid(gridSize, readGrid);
		
	}

	if (i=0;i<numThreads;i++){
   		free(paramList[i]);
	}
   free(paramList);

   free(threadList);
   freeArrays(gridSize);
	return 0;
}