#include "token.c"

struct token lookahead;

char first_Decl[3][5] = {"int\0","char\0","@\0"};

int search(char lex[],char str[][5],int n){
	for(int i=0;i<n;i++)
		if(!strcmp(lex,str[i]))
			return 1;
	return 0;
}

void error(char str1[],struct token t){
	printf("Error in %s",str1);
	printf("Current token: %s\nRow: %d. Column: %d.\n",t.lexeme,t.row,t.col-1);
	exit(0);
}

void Statement_List(FILE *fin,char ca);
void Assign_Stat(FILE *fin, char ca);

void Mul_Op(FILE *fin, char ca){
	//printf("17: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Multiplication Operator"))
		lookahead = getNextToken(fin,ca);
	else
		error("Mul_Op() function: Multiplication Operator expected.\n",lookahead);
}

void Add_Op(FILE *fin, char ca){
	//printf("16: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Addition Operator"))
		lookahead = getNextToken(fin,ca);
	else
		error("Add_Op() function: Addition Operator expected.\n",lookahead);
}

void Rel_Op(FILE *fin, char ca){
	//printf("15: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Relational Operator"))
		lookahead = getNextToken(fin,ca);
	else
		error("Rel_Op() function: Relational Operator expected.\n",lookahead);
}

void Factor(FILE *fin, char ca){
	//printf("14: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier") || !strcmp(lookahead.tok_type,"Number"))
		lookahead = getNextToken(fin,ca);
	else
		error("Factor() function: Identifier or Number type expected.\n",lookahead);
}

void Tprime(FILE *fin, char ca){
	//printf("13: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Multiplication Operator")){
		Mul_Op(fin,ca);
		Factor(fin,ca);
		Tprime(fin,ca);
	}
}

void Term(FILE *fin, char ca){
	//printf("12: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier") || !strcmp(lookahead.tok_type,"Number")){
		Factor(fin,ca);
		Tprime(fin,ca);
	}
	else
		error("Term() function: Identifier or Number type expected.\n",lookahead);		
}

void S_Eprime(FILE *fin, char ca){
	//printf("11: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Addition Operator")){
		Add_Op(fin,ca);
		Term(fin,ca);
		S_Eprime(fin,ca);
	}
}

void Simple_Expn(FILE *fin, char ca){
	//printf("10: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier") || !strcmp(lookahead.tok_type,"Number")){
		Term(fin,ca);
		S_Eprime(fin,ca);
	}
	else
		error("Simple_Expn() function: Identifier or Number type expected.\n",lookahead);
}

void Eprime(FILE *fin, char ca){
	//printf("9: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Relational Operator")){
		Rel_Op(fin,ca);
		Simple_Expn(fin,ca);
	}
}

void Expn(FILE *fin, char ca){
	//printf("8: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier") || !strcmp(lookahead.tok_type,"Number")){
		Simple_Expn(fin,ca);
		Eprime(fin,ca);
	}
	else
		error("Expn() function: Identifier or Number type expected.\n",lookahead);
}


void Looping_Stat(FILE *fin, char ca){
	//printf("20: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.lexeme,"while")){
		lookahead = getNextToken(fin,ca);
		if(!strcmp(lookahead.lexeme,"(")){
			lookahead = getNextToken(fin,ca);
			Expn(fin,ca);
			if(!strcmp(lookahead.lexeme,")")){
				lookahead = getNextToken(fin,ca);
				if(!strcmp(lookahead.lexeme,"{")){
					lookahead = getNextToken(fin,ca);
					Statement_List(fin,ca);
					lookahead = getNextToken(fin,ca);
					if(!strcmp(lookahead.lexeme,"}")){
						lookahead = getNextToken(fin,ca);
					}
					else
						error("Looping_Stat() function: } expected.\n",lookahead);
				}
				else
					error("Looping_Stat() function: { expected.\n",lookahead);
			}
			else
				error("Looping_Stat() function: ) expected.\n",lookahead);
		}
		else
			error("Looping_Stat() function: () expected.\n",lookahead);
	}

	else if(!strcmp(lookahead.lexeme,"for")){
		lookahead = getNextToken(fin,ca);
		//printf("20: %s\n",lookahead.lexeme);
		if(!strcmp(lookahead.lexeme,"(")){
			lookahead = getNextToken(fin,ca);
			//printf("20: %s\n",lookahead.lexeme);
			Assign_Stat(fin,ca);
			if(!strcmp(lookahead.lexeme,";")){
				lookahead = getNextToken(fin,ca);
				//printf("20: %s\n",lookahead.lexeme);
				Expn(fin,ca);
				if(!strcmp(lookahead.lexeme,";")){
					lookahead = getNextToken(fin,ca);
					//printf("20: %s\n",lookahead.lexeme);
					Assign_Stat(fin,ca);
					if(!strcmp(lookahead.lexeme,")")){
						lookahead = getNextToken(fin,ca);
						//printf("20: %s\n",lookahead.lexeme);
						if(!strcmp(lookahead.lexeme,"{")){
							lookahead = getNextToken(fin,ca);
							//printf("20: %s\n",lookahead.lexeme);
							Statement_List(fin,ca);
							lookahead = getNextToken(fin,ca);
							//printf("20: %s\n",lookahead.lexeme);
							if(!strcmp(lookahead.lexeme,"}")){
								lookahead = getNextToken(fin,ca);
								//printf("20: %s\n",lookahead.lexeme);
							}
							else
								error("Looping_Stat() function: } expected.\n",lookahead);
						}
						else
							error("Looping_Stat() function: { expected.\n",lookahead);
					}
					else
						error("Looping_Stat() function: ) expected.\n",lookahead);
				}
				else
					error("Looping_Stat() function: ; expected.\n",lookahead);
			}
			else
				error("Looping_Stat() function: ; expected.\n",lookahead);
		}
		else
			error("Looping_Stat() function: ( expected.\n",lookahead);
	}
	else
		error("Looping_Stat() function: for or while statment expected.\n",lookahead);
}

void Dprime(FILE *fin, char ca){
	//printf("19: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.lexeme,"else")){
		lookahead = getNextToken(fin,ca);
		if(!strcmp(lookahead.lexeme,"{")){
			lookahead = getNextToken(fin,ca);
			Statement_List(fin,ca);
			lookahead = getNextToken(fin,ca);
			if(!strcmp(lookahead.lexeme,"}")){
				lookahead = getNextToken(fin,ca);
			}
			else
				error("Dprime() function: } expected.\n",lookahead);
		}
		error("Dprime() function: { expected.\n",lookahead);
	}
	// else{
	// 	fseek(fin,-(strlen(lookahead.lexeme)),SEEK_CUR);
	// 	lookahead = getNextToken(fin,ca);
	// }
}

void Decision_Stat(FILE *fin, char ca){
	//printf("18: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.lexeme,"if")){
		lookahead = getNextToken(fin,ca);
		if(!strcmp(lookahead.lexeme,"(")){
			lookahead = getNextToken(fin,ca);
			Expn(fin,ca);
			if(!strcmp(lookahead.lexeme,")")){
				lookahead = getNextToken(fin,ca);
				if(!strcmp(lookahead.lexeme,"{")){
					lookahead = getNextToken(fin,ca);
					//printf("18: %s\n",lookahead.lexeme);
					Statement_List(fin,ca);
					lookahead = getNextToken(fin,ca);
					//printf("18: %s\n",lookahead.lexeme);
					if(!strcmp(lookahead.lexeme,"}")){
						lookahead = getNextToken(fin,ca);
						//printf("18: %s\n",lookahead.lexeme);
						Dprime(fin,ca);
					}
					else
						error("Decision_Stat() function: } expected.\n",lookahead);
				}
				else
					error("Decision_Stat() function: { expected.\n",lookahead);
			}
			else
				error("Decision_Stat() function: ) expected.\n",lookahead);
		}
		else
			error("Decision_Stat() function: ( expected.\n",lookahead);
	}
	else
		error("Decision_Stat() function: if statement expected.\n",lookahead);
}

void Assign_Stat(FILE *fin, char ca){
	//printf("7: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier")){
		lookahead = getNextToken(fin,ca);
		if(!strcmp(lookahead.lexeme,"=")){
			lookahead = getNextToken(fin,ca);
			Expn(fin,ca);
		}
		else
			error("Assign_Stat() function: = expected.\n",lookahead);
	}
	else
		error("Assign_Stat() function: Identifier type expected.\n",lookahead);
}

void Statement(FILE *fin, char ca){
	//printf("6: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier")){
		Assign_Stat(fin,ca);
		if(!strcmp(lookahead.lexeme,";"))
			lookahead = getNextToken(fin,ca);
		else
			error("Statement() function: ; expected.\n",lookahead);
	}

	else if(!strcmp(lookahead.lexeme,"if"))
		Decision_Stat(fin,ca);

	else if(!strcmp(lookahead.lexeme,"while") || !strcmp(lookahead.lexeme,"for"))
		Looping_Stat(fin,ca);
	
	else
		error("Statement() function: Identifier or the keywords if and while statements expected.\n",lookahead);
}

void Statement_List(FILE *fin,char ca){
	//printf("5: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier") || !strcmp(lookahead.lexeme,"if") || !strcmp(lookahead.lexeme,"while") || !strcmp(lookahead.lexeme,"for")){ // decision and loop
		Statement(fin,ca);
		Statement_List(fin,ca);
	} 
	else
		fseek(fin,-(strlen(lookahead.lexeme)),SEEK_CUR);
}

void Identifier_List(FILE *fin,char ca){
	//printf("4: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.tok_type,"Identifier")){
		lookahead = getNextToken(fin,ca);
		//printf("4: %s\n",lookahead.lexeme);
		if(!strcmp(lookahead.lexeme,",")){
			lookahead = getNextToken(fin,ca);
			//printf("4: %s\n",lookahead.lexeme);
			Identifier_List(fin,ca);
		}
		else if(!strcmp(lookahead.lexeme,"[")){
			lookahead = getNextToken(fin,ca);
			//printf("4: %s\n",lookahead.lexeme);
			if(!strcmp(lookahead.tok_type,"Number")){
				lookahead = getNextToken(fin,ca);
				//printf("4: %s\n",lookahead.lexeme);
				if(!strcmp(lookahead.lexeme,"]")){
					lookahead = getNextToken(fin,ca);
					//printf("4: %s\n",lookahead.lexeme);
					if(!strcmp(lookahead.lexeme,",")){
						lookahead = getNextToken(fin,ca);
						//printf("4: %s\n",lookahead.lexeme);
						Identifier_List(fin,ca);
					}
				}
				else{
					//printf("emeh\n");
					error("Identifier_List() function: ] expected.\n",lookahead);
				}
			}
			else
				error("Identifier_List() function: Number type expected.\n",lookahead);
		}
	}
	else
		error("Identifier_List() function: Identifier type expected.\n",lookahead);
}

void Data_Type(FILE *fin,char ca){
	//printf("3: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.lexeme,"int") || !strcmp(lookahead.lexeme,"char"))
		lookahead = getNextToken(fin,ca);
	else
		error("Data_Type() function: int or char keyword expected.\n",lookahead);
}

void Declarations(FILE *fin,char ca){
	//printf("2: %s\n",lookahead.lexeme);
	if(search(lookahead.lexeme,first_Decl,3)){
		Data_Type(fin,ca);
		Identifier_List(fin,ca);
		if(!strcmp(lookahead.lexeme,";")){
			lookahead = getNextToken(fin,ca);
			Declarations(fin,ca);
		}
		else
			error("Declarations() function: ; expected.\n",lookahead);
	}
	// else
	// 	int tf = fseek(fin,-(strlen(lookahead.lexeme)),SEEK_CUR);
	// else if(search(lookahead.lexeme.follow_Decl,))
	// 	return;
}

void Program(FILE *fin,char ca){
	//printf("1: %s\n",lookahead.lexeme);
	if(!strcmp(lookahead.lexeme,"main")){
		lookahead = getNextToken(fin,ca);
		//printf("1: %s\n",lookahead.lexeme);
		if(!strcmp(lookahead.lexeme,"(")){
			lookahead = getNextToken(fin,ca);
			//printf("1: %s\n",lookahead.lexeme);
			if(!strcmp(lookahead.lexeme,")")){
				lookahead = getNextToken(fin,ca);
				//printf("1: %s\n",lookahead.lexeme);
				if(!strcmp(lookahead.lexeme,"{")){
					lookahead = getNextToken(fin,ca);
					//printf("1: %s\n",lookahead.lexeme);
					Declarations(fin,ca);
					//printf("1: %s\n",lookahead.lexeme);
					Statement_List(fin,ca);
					//printf("1: %s\n",lookahead.lexeme);
					lookahead = getNextToken(fin,ca);
					//printf("1: %s\n",lookahead.lexeme);
					if(!strcmp(lookahead.lexeme,"}"))
						return;
					else
						error("Program() function: } expected.\n",lookahead);
				}
				else
					error("Program() function: { expected.\n",lookahead);
			}
			else
				error("Program() function: ) expected.\n",lookahead);
		}
		else
			error("Program() function: ( expected.\n",lookahead);
	}
	else
		error("Program() function: main expected.\n",lookahead);
}

void parser(FILE *fin){
	char ca;

	lookahead = getNextToken(fin,ca);
	//printf("0: %s\n",lookahead.lexeme);

	Program(fin,ca);

	lookahead = getNextToken(fin,ca);
	//printf("0: %s\n",lookahead.lexeme);

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