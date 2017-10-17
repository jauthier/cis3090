#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*encription*/
char * encrypt(char * input){

	char inDict[27] = "\0";

	int i = 0; // for iterating through the input string
	int len = strlen(input);
	for (i=0; i<len; i++){

		if(strstr(inDict, &input[i]) == NULL) {
			sprintf(inDict,"%s%c",inDict, input[i]);
		}
	}
	char * ret = malloc(sizeof(char)*(strlen(inDict)+1));
	strcpy(ret, inDict);
	return ret;
}

int main(int argc, char const *argv[]){
	char message[100] = "the cat";
	char * ret = encrypt(message);
	printf("%s\n", ret);
	free(ret);
	return 0;
}