#include <stdio.h>
#include "token.c"

struct token lookahead;

char firstProgram[][100] = {}

void Program(FILE *fin,char ca){
	lookahead = getNextToken(fin,ca);

	if(!strcmp(lookahead.lexeme,"main")){
		lookahead = getNextToken(fin,ca);
		if(!strcmp(lookahead.lexeme,"("))
	}
}

void parser(fin){
	char ca;

	Program(fin,ca);

	lookahead = getNextToken(fin,ca);

	if(!strcmp(lookahead.lexeme,"$"))
		printf("Sucess\n");
	else
		printf("Error\n");

}

int main(){
	FILE *fin = fopen("sample.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	parser(fin);
	
	return 0;

}