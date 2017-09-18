#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * runner(void * args);

int main(int argc, char const *argv[])
{
	
	char name[5] = "Jess";
	//thread id
	pthread_t thread;
	//create attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	//create a pthread
	pthread_create(&thread, &attr, runner, &name);

	pthread_join(thread, NULL);
	printf("done\n");
	return 0;
}

void * runner(void * args){

	char * name = (char *)args;
	pthread_exit(0);
}

