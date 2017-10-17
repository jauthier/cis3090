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
	char * eDict = letterScramble(ret);
	return ret;
}

char * letterScramble(char * input){

	int len = strlen(input);
	srand(time(NULL));
	char * eDict = malloc(sizeof(char)*(len+1));
	strcpy(eDict, input);

	for (int i = 0;i<len;i++){
		int swap = rand()%len;
		printf("%d\n", swap);

		char hold = eDict[i];
		eDict[i] = eDict[swap];
		eDict[swap] = hold;
	}
	printf("%s\n", eDict);
	return eDict;
}

int main(int argc, char const *argv[]){
	char message[100] = "the cat";
	char * ret = encrypt(message);
	printf("%s\n", ret);
	free(ret);
	return 0;
}