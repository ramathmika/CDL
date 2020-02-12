#include "token.c"
#include <stdio.h>

struct token lookahead;

char first_S[3][2] = {"a\0",">\0","(\0"};
char follow_S[3][2] = {"$\0",",\0",")\0"};

char first_T[3][2] = {"a\0",">\0","(\0"};
char follow_T[1][2] = {")\0"};

char first_T1[2][2] = {",\0","@\0"};
char follow_T1[1][2] = {")\0"}; 

int search(char lex[],char str[][2],int n){
	for(int i=0;i<n;i++)
		if(!strcmp(lex,str[i]))
			return 1;
	return 0;
}

void T1(FILE *fin, char ca){
	//printf("T1: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_T1,2)){
		if(!strcmp(lookahead.lexeme,",")){
			lookahead = getNextToken(fin,ca);
			S(fin,ca);
			lookahead = getNextToken(fin,ca);
			T1(fin,ca);
		}
	}
	else if(search(lookahead.lexeme,follow_T1,1))
		return;
	else{
		fprintf(stderr,"Error in T1\n");
		exit(0);
	}
}

void S(FILE *fin,char ca);


void T(FILE *fin, char ca){
	//printf("T: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_T,3)){
		S(fin,ca);
		lookahead = getNextToken(fin,ca);
		T1(fin,ca);
	}
	else{
		fprintf(stderr,"Error in T\n");
		exit(0);
	}
}


void S(FILE *fin, char ca){
	//printf("S: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_S,3)){
		if(!strcmp(lookahead.lexeme,"(")){
			lookahead = getNextToken(fin,ca);
			T(fin,ca);
		}
		else
			return;
	}
	else{
		fprintf(stderr,"Error in S\n");
		exit(0);
	}

}

void parser(FILE *fin){
	char ca;

	lookahead = getNextToken(fin,ca);

	S(fin,ca);

	lookahead = getNextToken(fin,ca);

	if(!strcmp(lookahead.lexeme,"$"))
		printf("Successfuly parsed.\n");
	else
		printf("Error in parsing.\n");

}

int main(){
	FILE *fin = fopen("q1txt.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	parser(fin);

	return 0;
}