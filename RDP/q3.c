#include "token.c"
#include <stdio.h>

struct token lookahead;

char first_S[1][2] = {"a\0"};
char follow_S[1][2] = {"$\0"};

char first_A[1][2] = {"b\0"};
char follow_A[1][2] = {"c\0"};

char first_A1[2][2] = {"b\0","@\0"};
char follow_A1[1][2] = {"c\0"}; 

char first_B[1][2] = {"d\0"};
char follow_B[1][2] = {"e\0"}; 

int search(char lex[],char str[][2],int n){
	for(int i=0;i<n;i++)
		if(!strcmp(lex,str[i]))
			return 1;
	return 0;
}

void B(FILE *fin,char ca){
	printf("B: %s\n",lookahead.lexeme);

	if(!strcmp(lookahead.lexeme,"d")){
		lookahead = getNextToken(fin,ca);
	}
	else{
		fprintf(stderr,"Error in B\n");
		exit(0);
	}
}

void A1(FILE *fin, char ca){
	printf("A1: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_A1,2)){
		if(!strcmp(lookahead.lexeme,"b")){
			lookahead = getNextToken(fin,ca);
			A1(fin,ca);
		}
	}
	else if(search(lookahead.lexeme,follow_A1,1))
		return;
	else{
		fprintf(stderr,"Error in A1\n");
		exit(0);
	}
}


void A(FILE *fin, char ca){
	printf("A: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_A,1)){
		if(!strcmp(lookahead.lexeme,"b")){
			lookahead = getNextToken(fin,ca);
			A1(fin,ca);
		}
		else 
			return;
	}
	else{
		fprintf(stderr,"Error in A\n");
		exit(0);
	}
}


void S(FILE *fin, char ca){
	printf("S: %s\n",lookahead.lexeme);

	if(search(lookahead.lexeme,first_S,1)){
		if(!strcmp(lookahead.lexeme,"a")){
			lookahead = getNextToken(fin,ca);
			A(fin,ca);
			if(!strcmp(lookahead.lexeme,"c")){
				lookahead = getNextToken(fin,ca);
				B(fin,ca);
				if(!strcmp(lookahead.lexeme,"e")){
					lookahead = getNextToken(fin,ca);
					return;
				}
			}
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
	FILE *fin = fopen("q3txt.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	parser(fin);

	return 0;
}