all: data task

data: data.c
	gcc -Wall data.c -o data -lpthread

task: task.c
	gcc -Wall task.c -o task -lpthread
