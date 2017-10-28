#include <stdio.h>

char * swap(char * str, int i, int j){
	char temp = str[i];
	str[i] = str[j];
	str[j] = temp;
	return str;
}

void generate(int n, char * str){
	if (n == 1){
		printf("%s\n", str);
		return;
	}
	for (int i=1;i<n-1;i++){
		printf("%d\n", n);
		generate(n-1, str);
		if (n%2 == 0) // n is even
			str = swap(str, i, n-1);
		else 
			str = swap(str, 1, n-1);
	}
}

int main(int argc, char const *argv[]){
	/* code */
	char str[6] = "tahce";

	generate(5,str);

	return 0;
}