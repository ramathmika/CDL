#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fin, *fout;
	int ca,cb;

	fin = fopen("q1in.c","r");
	if(fin == NULL){
		printf("Cannot open file.\n");
		exit(0);
	}
	fout = fopen("q1out.c","w");

	ca = getc(fin);
	while(ca != EOF){
		if(ca == '\t' || ca == ' ' || ca == '\n'){
			ca = ' ';
			cb = getc(fin);
			while(cb == ' ' || cb == '\t' || cb == '\n')
				cb = getc(fin);
			putc(ca,fout);
			putc(cb,fout);
		}
		else
			putc(ca,fout);
		ca = getc(fin);
	}

	fclose(fin);
	fclose(fout);

	return 0;
}