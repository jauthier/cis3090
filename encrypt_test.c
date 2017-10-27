#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

char ** strs;

char * encrypt(char * input);
char * letterScramble(char * input);

/*encription*/
char * encrypt(char * input){

	char inDict[27] = "\0";

	int i = 0; // for iterating through the input string
	int len = strlen(input);
	for (i=0; i<len; i++){

		if(strstr(inDict, &input[i]) == NULL) {
			if ((input[i] != ' ')&&(input[i] != '\n'))
				sprintf(inDict,"%s%c",inDict, input[i]);
		}
	}
	char * ret = malloc(sizeof(char)*(strlen(inDict)+1));
	strcpy(ret, inDict);
	return ret;
}

char * letterScramble(char * input){

	int len = strlen(input);
	srand(time(NULL));
	char * eDict = malloc(sizeof(char)*(len+1));
	strcpy(eDict, input);

	for (int i = 0;i<len;i++){
		int swap = rand()%len;

		char hold = eDict[i];
		eDict[i] = eDict[swap];
		eDict[swap] = hold;
	}
	return eDict;
}

int getPos(char c, char * str){
	int len = strlen(str);
	for (int i=0;i<len;i++){
		if (str[i] == c)
			return i;
	}
	return -1;
}

char * decryption(char * enMsg, char * inDict, char * eDict){
	int len = strlen(enMsg);
	char * ret = malloc(sizeof(char)*(len+1));
	for (int i=0;i<len;i++){
		if (enMsg[i] != ' ' && enMsg[i] != '\n'){
			int pos = getPos(enMsg[i], eDict);
			ret[i] = inDict[pos];
		} else {
			ret[i] = enMsg[i];
		}
	}
	ret[len] = '\0';
	return ret;
}

int main(int argc, char const *argv[]){
	char message[100] = "the cat";
	char * inDict = encrypt(message);
	printf("%s\n", inDict);
	char * eDict = letterScramble(inDict);
	printf("%s\n", eDict);

	int len = strlen(message);
	char * enMsg = malloc(sizeof(char)*(len+1));
	for (int i=0;i<len;i++){
		if (message[i] != ' ' && message[i] != '\n'){
			int pos = getPos(message[i], inDict);
			enMsg[i] = eDict[pos];
		} else {
			enMsg[i] = message[i];
		}
	}
	enMsg[len] = '\0';
	printf("%s\n", enMsg);


	// each process will get a string starting with a different letter
	int numMPI = strlen(eDict);
	// make an array of the strings for each process
	strs = malloc(sizeof(char *)*numMPI);
	int i = 0;
	for (i=0; i<numMPI; i++){
		strs[i] = malloc(sizeof(char)*(numMPI+1));
	}

	strcpy(strs[0], eDict);
	i = 0;
	for (int j=1;j<numMPI;j++){ // skip the first str
		for (i=0;i<numMPI-1;i++){
			strs[j][i] = strs[j-1][i+1];
		}
		strs[j][numMPI-1] = strs[j-1][0];
		strs[j][numMPI] = '\0';
	}
	i=0;
	for(i=0;i<numMPI;i++){
		printf("%s\n", strs[i]);
	}

	// pushed up to here -- Test it
	int myRank;
	char msg[26];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numMPI);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (myRank != 0){
		MPI_Recv(msg,26,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Send(msg, strlen(msg)+1, MPI_CHAR,0,0,MPI_COMM_WORLD);
	} else {
		printf("%s\n", strs[myRank]);
		// need to send the other processes their strings
		for(int k=1;k<numMPI;k++){
			MPI_Send(strs[k], strlen(msg)+1, MPI_CHAR,k,0,MPI_COMM_WORLD);

			MPI_Recv(msg,26,MPI_CHAR,k,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("%s\n", msg);
		}
	}


	MPI_Finalize();



	free(inDict);
	free(eDict);
	free(enMsg);
	return 0;
}

