#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

char ** strs;

char * encrypt(char * input);
char * letterScramble(char * input);
char * generate(int n, char * str, int rank, char * inDict);

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

char * swap(char * str, int i, int j){
	char temp = str[i];
	str[i] = str[j];
	str[j] = temp;
	return str;
}

char * generate(int n, char * str, char * inDict){
	if (n == 2){
		if (strcmp(str, inDict)==0)
			return str;
		else 
			return NULL;
	}
	for (int i=1;i<n;i++){
		char * ret = generate(n-1, str, rank, inDict);
		if (ret != NULL && strcmp(ret, inDict)==0)
			return ret;
		if (n%2 == 0) // n is even
			str = swap(str, i, n-1);
		else 
			str = swap(str, 1, n-1);
	}
	return NULL;
}

int main(int argc, char const *argv[]){
	
	char message[100] = "the cat";
	/* starting MPI stuff */
	int myRank;
	int numMPI;
	char msg[26];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numMPI);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (myRank != 0){
		char buff[26];
		MPI_Recv(msg,26,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(buff,26,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		char * ret = generate(numMPI, msg, buff);
		if (ret != NULL){
			sprintf(ret,"%d:%s",myRank,ret);
			MPI_Send(ret, strlen(ret)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}

	} else {
		char * inDict = encrypt(message);
		char * eDict = letterScramble(inDict);
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

		// each process will get a string starting with a different letter
		numMPI = strlen(eDict);
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

		for(int k=1;k<numMPI;k++){
			MPI_Send(strs[k], strlen(strs[k])+1, MPI_CHAR,k,0,MPI_COMM_WORLD);
			MPI_Send(inDict, strlen(inDict)+1, MPI_CHAR,k,0,MPI_COMM_WORLD);
		}
		char * ret = generate(numMPI, strs[0], inDict);
		char buff[26];
		if (ret == NULL){
			MPI_Recv(buff,26,MPI_CHAR,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("%s\n", buff);
		} else 
		printf("%s\n", ret);

	}

	MPI_Finalize();
	return 0;
}
