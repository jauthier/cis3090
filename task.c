/*
** task.c
** CIS3090 Assigmnet 1
** Author: Jessica Authier, 0849720
** Last Modified: 10/01/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

typedef struct Point{
	int column;
	int row;
	struct Point * next;
} Point;

typedef struct queue{
	Point * head;
	Point * tail;
} Queue;

Queue * occupied;
Queue * unoccupied;

int ** readGrid;
int ** writeGrid;

int done;

void initArrays(int gridSize);
void setArray(int gridSize);
void freeArrays(int gridSize);
Point * initPoint(int col, int row);
void deletePoint(Point * toDelete);
Queue* initQueue();
void deleteQueue(Queue * toDelete);
void addBack(Point * pt, Queue * addTo);
Point * removeFront(Queue * queue);
int isEmpty(Queue * queue);
void printGrid(int gridSize, int ** grid);
void * neighbourCount(void * size);
void * writeAlive(void *);
void * writeDead(void *);
void swapGrids(int gridSize);

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
			if (num == 1){
				readGrid[i][j] = 1;
			} else {
				readGrid[i][j] = 0;
			}
		}
	}
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

Point * initPoint(int col, int row){
	Point * newPoint = malloc(sizeof(Point));
	newPoint->column = col;
	newPoint->row = row;
	newPoint->next = NULL;
	return newPoint;
}

void deletePoint(Point * toDelete){
	free(toDelete);
}

Queue* initQueue(){
	Queue * newQueue = malloc(sizeof(Queue));
	newQueue->head = NULL;
	newQueue->tail = NULL;
	return newQueue;
}

void deleteQueue(Queue * toDelete){
	Point * pt = toDelete->head;
	while (pt != NULL){
		Point *hold = pt;
		pt = pt->next;
		free(hold);
	}
	free(toDelete);
}

void addBack(Point * pt, Queue * addTo){
	if (addTo->tail != NULL){ /* the list already has elements*/
		addTo->tail->next = pt;
		addTo->tail = pt;
	} else { /* there are no elements */
		addTo->head = pt;
		addTo->tail = pt;
	}
}

Point * removeFront(Queue * queue){
	if (queue->head == NULL){ /* there are no elements */
		return NULL;
	} else if (queue->head->next == NULL){ /* there is only one element*/
		Point * hold = queue-> head;
		queue->head = NULL;
		queue->tail = NULL;
		return hold; 
	} else {
		Point * hold = queue-> head;
		queue->head = queue->head->next;
		return hold;
	}
}

int isEmpty(Queue * queue){
	if (queue->head == NULL)
		return 1;
	else 
		return 0;
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

void * neighbourCount(void * size){
	int gridSize = *(int *)size;
	int i = 0; 
	int j = 0;
	int oc = 0;
	int unoc = 0;
	for (i=0;i<gridSize;i++){
		for (j=0; j<gridSize;++j){
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
				if (j < (gridSize-1)){
					//check upper right
					if (readGrid[i-1][j+1] == 1)
						neighbours++;
				}
			}
			if (i < (gridSize-1)){
				//check down
				if (readGrid[i+1][j] == 1)
					neighbours++;
				if (j > 0){
					//check lowwer left
					if (readGrid[i+1][j-1] == 1)
						neighbours++;
				}
				if (j < (gridSize-1)){
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
			if (j < (gridSize-1)){
				//check right
				if (readGrid[i][j+1] == 1)
					neighbours++;
			}

			Point * pt = initPoint(i,j);

			if(readGrid[i][j] == 1){
				if(neighbours==2 || neighbours == 3){
					addBack(pt,occupied);
					oc++;
				} else {
					addBack(pt,unoccupied);
					unoc++;
				} 
			} else {
				if (neighbours == 3){
					addBack(pt,occupied);
					oc++;
				} else {
					addBack(pt,unoccupied);
					unoc++;
				}
			}
		}
	}
	done = 1;
	return NULL;
}

void * writeAlive(void * blank){
	while (done == 0 || isEmpty(occupied) == 0 || isEmpty(unoccupied) == 0){
		Point * pt = removeFront(occupied);
		if (pt != NULL){ // the list was not empty
			writeGrid[pt->column][pt->row] = 1;
			deletePoint(pt);
		}
	}
	return NULL;
}

void * writeDead(void * blank){
	while (done == 0 || isEmpty(unoccupied) == 0 || isEmpty(occupied) == 0){
		Point * pt = removeFront(unoccupied);
		if (pt != NULL){ // the list was not empty
			writeGrid[pt->column][pt->row] = 0;
			deletePoint(pt);
		}
	}
	return NULL;
}

void swapGrids(int gridSize){
	int i=0, j=0;
	for (i=0;i<gridSize;i++){
		for (j=0; j<gridSize;++j){
			readGrid[i][j] = writeGrid[i][j];
		}
	}
}

int main(int argc, char const *argv[]) {
	
	if (argc < 3){
		printf("Please enter the number of threads, the size of the\ngrid and the number of iterations as command line arguments\n");
		exit(0);
	}

	//up to 4 command line arguments
	int gridSize = atol(argv[1]);
	int numIter = atol(argv[2]);
	int display = 0;

	if (argc == 4){
		if (strcmp(argv[3], "-d")==0){
			display = 1;
		}
	}

	//init the arrays
	initArrays(gridSize);
	if (display == 1)
		printGrid(gridSize, readGrid);

	//init the queues
	occupied = initQueue();
	unoccupied = initQueue();

	//do thread things
	int i;
	pthread_t t0,t1,t2;
	for (i=0;i<numIter;i++){
		done = 0;
		pthread_create(&t0,NULL,neighbourCount, (void*)&gridSize);
		pthread_create(&t1,NULL,writeAlive);
		pthread_create(&t2,NULL,writeDead);
		
      	pthread_join(t0, NULL);
      	pthread_join(t1, NULL);
      	pthread_join(t2, NULL); 

		//write over grid
		swapGrids(gridSize);
		if (display == 1)
			printGrid(gridSize, readGrid);		
	}
  
	deleteQueue(occupied);
	deleteQueue(unoccupied);
    freeArrays(gridSize);
	return 0;
}
