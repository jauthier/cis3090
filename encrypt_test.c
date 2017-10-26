#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int main(int argc, char const *argv[]){
	char message[100] = "the cat";
	char * inDict = encrypt(message);
	printf("%s\n", inDict);
	char * eDict = letterScramble(inDict);
	printf("%s\n", eDict);

	int len = strlen(message);
	char * enMsg = malloc(sizeof(char)*(len+1));
	for (int i=0;i<len;i++){
		if (message[i] != ' '||message[i] != '\n'){
			int pos = getPos(message[i], inDict);
			enMsg[i] = eDict[pos];
		} else {
			enMsg[i] = message[i];
		}
	}
	enMsg[len] = '\0';
	printf("%s\n", enMsg);

	free(inDict);
	free(eDict);
	free(enMsg);
	return 0;
}