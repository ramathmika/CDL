#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fin, *fout;
	int ca,cb,temp;

	fin = fopen("q2in.c","r");
	if(fin == NULL){
		printf("Cannot open file.\n");
		exit(0);
	}
	fout = fopen("q2out.c","w");

	ca = getc(fin);
	while(ca != EOF){
		if(ca == '"'){
			do{
				putc(ca,fout);
				ca = getc(fin);
			}while(ca != '"');
		}	
		else if(ca == '#'){
			while(ca != '\n')
				ca = getc(fin);
		}
		else
			putc(ca,fout);
		ca = getc(fin);
	}

	fclose(fin);
	fclose(fout);
	return 0;
}