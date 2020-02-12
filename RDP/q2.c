#include "token.c"
#include <stdio.h>

struct token lookahead;

char first_E[2][3] = {"(\0","id\0"};
char follow_E[2][3] = {"$\0",")\0"};

char first_E1[2][3] = {"+\0","@\0"};
char follow_E1[2][3] = {"$\0",")\0"};

char first_T[2][3] = {"(\0","id\0"};
char follow_T[3][3] = {"$\0",")\0","+\0"};

char first_T1[2][3] = {"*\0","@\0"};
char follow_T1[3][3] = {"$\0",")\0","+\0"};

char first_F[2][3] = {"(\0","id\0"};
char follow_F[4][3] = {"*\0","$\0",")\0","+\0"};

int search(char lex[],char str[][3],int n){
	for(int i=0;i<n;i++)
		if(!strcmp(lex,str[i]))
			return 1;
	return 0;
}

void F(FILE *fin,char ca){
	printf("F: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_F,2)){
		if(!strcmp(lookahead.lexeme,"(")){
			lookahead = getNextToken(fin,ca);
			E(fin,ca);
			if(!strcmp(lookahead.lexeme,")")){
				lookahead = getNextToken(fin,ca);
				return;
			}
		}
		else
			return;
	}
	else{
		fprintf(stderr,"Error in F1\n");
		exit(0);
	}
}

void T1(FILE *fin,char ca){
	printf("T1: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_T1,2)){
		if(!strcmp(lookahead.lexeme,"*")){
			lookahead = getNextToken(fin,ca);
			F(fin,ca);
			lookahead = getNextToken(fin,ca);
			T1(fin,ca);
		}
	}
	else if(search(lookahead.lexeme,follow_T1,3)){
		int tf = fseek(fin,-1,SEEK_CUR);
		return;
	}
	else{
		fprintf(stderr,"Error in T1\n");
		exit(0);
	}
	//lookahead = getNextToken(fin,ca);
}

void T(FILE *fin,char ca){
	printf("T: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_T,2)){
		F(fin,ca);
		lookahead = getNextToken(fin,ca);
		T1(fin,ca);
	}
	else{
		fprintf(stderr,"Error in T\n");
		exit(0);
	}
}

void E1(FILE *fin, char ca){
	printf("E1: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_E1,2)){
		if(!strcmp(lookahead.lexeme,"+")){
			lookahead = getNextToken(fin,ca);
			T(fin,ca);
			lookahead = getNextToken(fin,ca);
			E1(fin,ca);
		}
	}
	else if(search(lookahead.lexeme,follow_E1,2)){
		int tf = fseek(fin,-1,SEEK_CUR);
		return;
	}
	else{
		fprintf(stderr,"Error in E1\n");
		exit(0);
	}
	//lookahead = getNextToken(fin,ca);
}

void E(FILE *fin, char ca){
	printf("E: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_E,2)){
		T(fin,ca);
		lookahead = getNextToken(fin,ca);
		E1(fin,ca);
	}
	else{
		fprintf(stderr,"Error in E\n");
		exit(0);
	}
}

void parser(FILE *fin){
	char ca;

	lookahead = getNextToken(fin,ca);

	E(fin,ca);

	lookahead = getNextToken(fin,ca);

	if(!strcmp(lookahead.lexeme,"$"))
		printf("Successfuly parsed.\n");
	else
		printf("Error in parsing.\n");

}

int main(){
	FILE *fin = fopen("q2txt.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	parser(fin);

	return 0;
}