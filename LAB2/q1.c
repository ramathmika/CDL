#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct token{
	char lexemename[100];
	unsigned int row,col;
	char type[100];
};

char keywords[32][100] = {"int","char","float","double","if","while","do","for","else","auto","const","short","struct","unsigned","break","continue","long","signed","switch","void","case","default","enum","goto","register","sizeof","typedef","volatile","extern","return","static","union"};

int row = 1, col = 1;

int find(char cb[]){
	for(int i=0;i<32;i++)
		if(strcmp(keywords[i],cb) == 0)
			return 1;
	return 0;
}

struct token getNextToken(char ca, FILE *fin, char buf[]){
	struct token t;
	int i = 0;
	char cb;
	if(ca == '/'){
		cb = fgetc(fin);
		col++;
		if(cb == '/'){
			while(ca != '\n'){
				ca = fgetc(fin);
			}
			row++;
			col = 0;
		}
		else if(cb == '*'){
			do{
				while(ca != '*'){
					ca = fgetc(fin);
					if(ca == '\n'){
						row++;
						col = 0;
					}
					else
						col++;
				}
				ca = fgetc(fin);
				if(ca == '\n'){
					row++;
					col = 0;
				}
				else
					col++;
			}while(ca != '/');
		}
		t.lexemename[0] = '\0';
		return t;
	}

	else if(ca == '\t' || ca == ' ' || ca == '\n'){
		if(ca == '\n'){
			row++;
			col = 0;
		}
		else if(ca == '\t')
			col+=4;
		else
			col++;
		ca = fgetc(fin);
		while(ca == ' ' || ca == '\t' || ca == '\n'){
			if(ca == '\n'){
				row++;
				col = 0;
			}
			else if(ca == '\t')
				col+=4;
			else
				col++;
			ca = fgetc(fin);
		}
		int tf = fseek(fin,-1,SEEK_CUR);
		t.lexemename[0] = '\0';
		return t;
	}

	else if(ca == '"'){
		col++;
		do{
			buf[i++] = ca;
			ca = fgetc(fin);
			if(ca == '\n'){
				row++;
				col = 0;
			}
			else
				col++;
		}while(ca != '"');
		buf[i++] = '"';
		buf[i] = '\0';
		strcpy(t.type,"String Literal");
	}

	else if(ca == '#'){
		col++;
		ca = fgetc(fin);
		while(ca == ' '){
			col++;
			ca = fgetc(fin);
		}
		if(ca == 'i'){
			while(ca != '\n')
				ca = fgetc(fin);
			row++;
			col = 0;
		}
		t.lexemename[0] = '\0';
		return t;
	}

	else if(isalpha(ca)){
		do{
			col++;
			buf[i++] = ca;
			ca = fgetc(fin);
		}while(isalpha(ca));
		buf[i] = '\0';

		if(find(buf)){
			strcpy(t.type,"Keyword");
		}
		else{
			strcpy(t.type,"Identifier.");
		}
		int tf = fseek(fin,-1,SEEK_CUR);
	}

	else if(isdigit(ca)){
		col++;
		buf[i++] = ca;
		buf[i] = '\0';
		strcpy(t.type,"Numerical Constant.");
	}

	else if(ca == '='){
		int f = -1;
		col++;
		buf[i++] = ca;
		ca = fgetc(fin);
		if(ca == '='){
			f++;
			col++;
			buf[i++] = ca;
			buf[i] = '\0';
			strcpy(t.type,"Relational Operator.");
		}
		else{
			buf[i] = '\0';
			strcpy(t.type,"Assignment Operator.");
		}
		int tf = fseek(fin,f,SEEK_CUR);
	}
	else if(ca == '<' || ca == '>'){
		int f = -1;
		col++;
		buf[i++] = ca;
		ca = fgetc(fin);
		if(ca == '='){
			f++;
			col++;
			buf[i++] = ca;
		}
		buf[i] = '\0';
		strcpy(t.type,"Relational Operator.");
		int tf = fseek(fin,f,SEEK_CUR);
	}
	
	else if(ca == '&'){
		int f = -1;
		col++;
		buf[i++] = ca;
		ca = fgetc(fin);
		if(ca == '&'){
			f++;
			col++;
			buf[i++] = ca;
			buf[i] = '\0';
			strcpy(t.type,"Logical Operator.");
		}
		int tf = fseek(fin,f,SEEK_CUR);
	}

	else if(ca == '|'){
		int f = -1;
		col++;
		buf[i++] = ca;
		ca = fgetc(fin);
		if(ca == '|'){
			f++;
			col++;
			buf[i++] = ca;
			buf[i] = '\0';
			strcpy(t.type,"Logical Operator.");
		}
		int tf = fseek(fin,f,SEEK_CUR);
	}

	else if(ca == '!'){
		int f = -1;
		col++;
		buf[i++] = ca;
		ca = fgetc(fin);
		if(ca == '='){
			f++;
			col++;
			buf[i++] = ca;
			buf[i] = '\0';
			strcpy(t.type,"Relational Operator.");
		}
		buf[i] = '\0';
		strcpy(t.type,"Logical Operator.");
		int tf = fseek(fin,f,SEEK_CUR);
	}

	else if(ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%'){
		col++;
		buf[i++] = ca;
		buf[i] = '\0';
		strcpy(t.type,"Arithmetic Operator.");
	}  // Make exception for pointers

	else{
		col++;
		buf[i++] = ca;
		buf[i] = '\0';
		strcpy(t.type,buf);
	}

	strcpy(t.lexemename,buf);
	t.row = row;
	t.col = col;
	return t;
}

int main(){
	char ca, cb, buf[100];
	// int row=1,col=0,i;

	FILE *fin = fopen("digit.c","r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(0);
	}

	struct token t;

	printf("Printed as <Row,Column,Lexeme,Type>\n");

	ca = fgetc(fin);
	while(ca!=EOF){
		// i=0; 
		buf[0] = '\0';

		// if(ca == '/'){
		// 	cb = fgetc(fin);
		// 	col++;
		// 	if(cb == '/'){
		// 		while(ca != '\n'){
		// 			ca = fgetc(fin);
		// 		}
		// 		row++;
		// 		col = 0;
		// 	}
		// 	else if(cb == '*'){
		// 		do{
		// 			while(ca != '*'){
		// 				ca = fgetc(fin);
		// 				if(ca == '\n'){
		// 					row++;
		// 					col = 0;
		// 				}
		// 				else
		// 					col++;
		// 			}
		// 			ca = fgetc(fin);
		// 			if(ca == '\n'){
		// 				row++;
		// 				col = 0;
		// 			}
		// 			else
		// 				col++;
		// 		}while(ca != '/');
		// 	}
		// }

		// else if(ca == '\t' || ca == ' ' || ca == '\n'){
		// 	if(ca == '\n'){
		// 		row++;
		// 		col = 0;
		// 	}
		// 	else if(ca == '\t')
		// 		col+=4;
		// 	else
		// 		col++;
		// 	ca = fgetc(fin);
		// 	while(ca == ' ' || ca == '\t' || ca == '\n'){
		// 		if(ca == '\n'){
		// 			row++;
		// 			col = 0;
		// 		}
		// 		else if(ca == '\t')
		// 			col+=4;
		// 		else
		// 			col++;
		// 		ca = fgetc(fin);
		// 	}
		// 	continue;
		// }

		// else if(ca == '"'){
		// 	col++;
		// 	do{
		// 		buf[i++] = ca;
		// 		ca = fgetc(fin);
		// 		if(ca == '\n'){
		// 			row++;
		// 			col = 0;
		// 		}
		// 		else
		// 			col++;
		// 	}while(ca != '"');
		// 	buf[i++] = '"';
		// 	buf[i] = '\0';
		// 	printf("\n<%d, %d, %s, String Literal>\n",row,col,buf);
		// }

		// else if(ca == '#'){
		// 	col++;
		// 	ca = fgetc(fin);
		// 	while(ca == ' '){
		// 		col++;
		// 		ca = fgetc(fin);
		// 	}
		// 	if(ca == 'i'){
		// 		while(ca != '\n')
		// 			ca = fgetc(fin);
		// 		row++;
		// 		col = 0;
		// 	}
		// }

		// else if(isalpha(ca)){
		// 	do{
		// 		col++;
		// 		buf[i++] = ca;
		// 		ca = fgetc(fin);
		// 	}while(isalpha(ca));
		// 	buf[i] = '\0';

		// 	if(find(buf)){
		// 		printf("\n<%d, %d, %s, Keyword>\n",row,(col-strlen(buf)+1),buf);
		// 	}
		// 	else{
		// 		printf("\n<%d, %d, %s, Identifier>\n",row,(col-strlen(buf)+1),buf);
		// 	}
		// 	continue;
		// }

		// else if(isdigit(ca)){
		// 	col++;
		// 	printf("\n<%d, %d, %c, Numerical Constant>\n",row,col,ca);
		// }

		// else if(ca == '='){
		// 	col++;
		// 	buf[i++] = ca;
		// 	ca = fgetc(fin);
		// 	if(ca == '='){
		// 		col++;
		// 		buf[i++] = ca;
		// 		buf[i] = '\0';
		// 		printf("\n<%d, %d, %s, Relational Operator>\n",row,col-1,buf);
		// 	}
		// 	else{
		// 		buf[i] = '\0';
		// 		printf("\n<%d, %d, %s, Assignment Operator>\n",row,col,buf);
		// 	}
		// 	continue;
		// }
		// else if(ca == '<' || ca == '>'){
		// 	col++;
		// 	buf[i++] = ca;
		// 	ca = fgetc(fin);
		// 	if(ca == '='){
		// 		col++;
		// 		buf[i++] = ca;
		// 	}
		// 	else if(ca == '\n'){
		// 		row++;
		// 		col = 0;
		// 	}
		// 	buf[i] = '\0';
		// 	printf("\n<%d, %d, %s, Relational Operator>\n",row,col,buf);
		// 	continue;
		// }
		
		// else if(ca == '&'){
		// 	col++;
		// 	buf[i++] = ca;
		// 	ca = fgetc(fin);
		// 	if(ca == '&'){
		// 		col++;
		// 		buf[i++] = ca;
		// 		buf[i] = '\0';
		// 		printf("\n<%d, %d, %s, Logical Operator>\n",row,col-1,buf);
		// 	}
		// 	continue;
		// }

		// else if(ca == '|'){
		// 	col++;
		// 	buf[i++] = ca;
		// 	ca = fgetc(fin);
		// 	if(ca == '|'){
		// 		col++;
		// 		buf[i++] = ca;
		// 		buf[i] = '\0';
		// 		printf("\n<%d, %d, %s, Logical Operator>\n",row,col-1,buf);
		// 	}
		// 	continue;
		// }

		// else if(ca == '!'){
		// 	col++;
		// 	buf[i++] = ca;
		// 	ca = fgetc(fin);
		// 	if(ca == '='){
		// 		col++;
		// 		buf[i++] = ca;
		// 		buf[i] = '\0';
		// 		printf("\n<%d, %d, %s, Relational Operator>\n",row,col-1,buf);
		// 	}
		// 	buf[i] = '\0';
		// 	printf("\n<%d, %d, %s, Logical Operator>\n",row,col,buf);
		// 	continue;
		// }

		// else if(ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%'){
		// 	col++;
		// 	buf[i++] = ca;
		// 	buf[i] = '\0';
		// 	printf("\n<%d, %d, %s, Arithmetic Operator>\n",row,col,buf);
		// }  // Make exception for pointers

		// else{
		// 	col++;
		// 	buf[i++] = ca;
		// 	buf[i] = '\0';
		// 	printf("\n<%d, %d, %s, %s>\n",row,col,buf,buf);
		// }

		t = getNextToken(ca,fin,buf);
		if(t.lexemename[0] != '\0')
			printf("\n<%d, %d, %s, %s>\n",t.row,t.col,t.lexemename,t.type);

		ca = fgetc(fin);
	}
}