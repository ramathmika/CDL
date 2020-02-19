#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *fin, *fout;
	int ca,line = 1;

	fin = fopen("addtxt.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	printf("%d ",line);
	ca = getc(fin);
	while(ca != EOF){
		if(ca == '\n'){
			line++;
			printf("%c%d ",ca,line);
		}

		else
			printf("%c",ca);
		ca = getc(fin);
	}

	fclose(fin);
	return 0;
}