#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void setArray(int gridSize){
	int i=0, j=0;
	srand(time(NULL));
	for (i=0;i<gridSize;i++){
		for (j=0; j<gridSize;++j){
			int num = rand()%5;
			printf("%d\n", num);
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

	if (argc == 5){
		if (strcmp(argv[4], "-d")==0){
			display = 1;
		}
	}


	//init the arrays
	initArrays(gridSize);
	//printGrid(gridSize, readGrid);

	return 0;
}