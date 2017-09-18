all: data task

data: data.c
	gcc -Wall -o data data.c -lpthread

task: task.c
	gcc -Wall -o task task.c -lpthread

clear:
	rm data task
