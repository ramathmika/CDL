#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char keywords[32][100] = {"int","char","float","double","if","while","do","for","else","auto","const","short","struct","unsigned","break","continue","long","signed","switch","void","case","default","enum","goto","register","sizeof","typedef","volatile","extern","return","static","union"};

int find(char cb[]){
	for(int i=0;i<32;i++)
		if(strcmp(keywords[i],cb) == 0)
			return 1;
	return 0;
}

int main(){
	FILE *fin;
	int ca,line = 1,column = 0, i=0;
	char cb[100];

	fin = fopen("q3in.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	printf("Keyword\tLine\tColumn\n");

	ca = getc(fin);
	while(ca != EOF){
		// printf("Scanned: %s\n",cb);
		// printf("Row: %d. column: %d\n",line,column);
		fscanf(fin,"%s",cb);

		if(ca == '\n'){
			line++;
			column = 0;
		}
		else{
			column += strlen(cb);;
		}

		if(find(cb)){
			for(int i=0;i<strlen(cb);i++)
				printf("%c",toupper(cb[i]));
			printf("\t%d\t%d\n",line,column);
		}
		column += strlen(cb);
		ca = getc(fin);

		/*if(ca == '\n'){
			line++;
			column = 0;
		}
		else
			column++;

		if(ca>='a' && ca<='z'){
			while(ca >= 'a' && ca <= 'z'){
				cb[i++] = ca;
				ca = getc(fin);
			}

			cb[i] = '\0';

			// printf("String: %s\n",cb);

			if(find(cb)){
				for(int i=0;i<strlen(cb);i++)
					printf("%c",toupper(cb[i]));
				printf("\t%d\t%d\n",line,column);
			}

			for(int j=0;j<i;j++)
				cb[i] = '\0';
			i = 0;

		}


		ca = getc(fin);*/

	}

	fclose(fin);

	return 0;
}