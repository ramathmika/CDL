#include <stdio.h>
#include "token.c"

struct token lookahead;

void T(FILE *fin, char ca){
	lookahead = getNextToken(fin,ca);
	//printf("Token3: %s\n",lookahead.lexeme);
	
	if(!strcmp(lookahead.lexeme,"*")){
		lookahead = getNextToken(fin,ca);
		//printf("Token4: %s\n",lookahead.lexeme);
	
		if(!strcmp(lookahead.tok_type,"Number")){
			lookahead = getNextToken(fin,ca);
			//printf("Token5: %s\n",lookahead.lexeme);
	
			T(fin,ca);
		}
		else{
			fprintf(stderr,"Error in T after *\n");
			exit(0);
		}
	}
	else 
		return;
}

void E(FILE *fin,char ca){

	lookahead = getNextToken(fin,ca);
	//printf("Token1: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Number")){
		lookahead = getNextToken(fin,ca);
		//printf("Token2: %s\n",lookahead.lexeme);
	
		T(fin,ca);
	}
	else{
		fprintf(stderr,"Error in E\n");
		exit(0);
	}
}



int main(){
	FILE *fin = fopen("sample.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}


	E(fin,ca);

	lookahead = getNextToken(fin,ca);

	if(!strcmp(lookahead.lexeme,"$"))
		printf("Success\n");
	else
		printf("Error\n");

	return 0;

}