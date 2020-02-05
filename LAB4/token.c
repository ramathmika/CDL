#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TableLength 30

struct token{
	char lexeme[100],type[100],scope,args[100],return_type[100],tok_type[100];
	int index,size,no_arg;
	unsigned int row,col;
};

struct listElement{
	struct token tok;
	struct listElement *next;
};

struct listElement *Table[TableLength];

void Initialize(){
	for(int i=0;i<TableLength;i++)
		Table[i] = NULL;
}

void Display(){
	struct listElement *temp;
	int i=0;
	printf("Hash\tName\tType\tSize\tScope\tNo_args\tArgs\t\tRet_type\n");
	for(i=0;i<TableLength;i++){
		temp = Table[i];
		while(temp){
			printf("\n%d\t%s\t%s\t%d\t%c\t%d\t%s\t\t%s\n",temp->tok.index,temp->tok.lexeme,temp->tok.type,temp->tok.size,temp->tok.scope,temp->tok.no_arg,temp->tok.args,temp->tok.return_type);
			temp = temp->next;
		}
	}
}

int Hash(char *str){
	int i,val=0;
	for(i=1;i<=strlen(str);i++)
		val+= ((int)str[i-1]*i);
	val = val%TableLength;
	return val;
}

int Search(char *str){
	int i=0;
	struct listElement *temp;
	for(i=0;i<TableLength;i++){
		temp = Table[i];
		while(temp){
			//temp = Table[i];
			if(!strcmp(str,temp->tok.lexeme))
				return 1;
			temp = temp->next;
		}
	}
	
	return 0;
}

void Insert(struct token tok){
	if(Search(tok.lexeme))
		return;
	int val = Hash(tok.lexeme);
	struct listElement *cur = (struct listElement*)malloc(sizeof(struct listElement));
	cur->tok = tok;
	cur->next = NULL;
	if(!Table[val])
		Table[val] = cur;
	else{
		struct listElement *ele = Table[val];
		while(ele->next)
			ele = ele->next;
		ele->next = cur;
	}
}

char keywords[32][100] = {"int","char","float","double","if","while","do","for","else","auto","const","short","struct","unsigned","break","continue","long","signed","switch","void","case","default","enum","goto","register","sizeof","typedef","volatile","extern","return","static","union"};
char types[5][100] = {"int","char","float","double","void"};
char prefun[5][100] = {"printf","scanf","strlen","strcmp","sqrt"};

int size(char str[]){
	if(!strcmp(str,"int"))
		return 4;
	else if(!strcmp(str,"char"))
		return 1;
	else if(!strcmp(str,"float"))
		return 8;
	else if(!strcmp(str,"double"))
		return 16;
	else
		return 0;
}

int row = 1, col = 1, ind = 1,flag=0;
char ty[100],ca,buf[100];

int find(char cb[],char str[][100],int n){
	for(int i=0;i<n;i++)
		if(strcmp(str[i],cb) == 0)
			return 1;
	return 0;
}

struct token getNextToken(FILE *fin,char ca){
	buf[0] = '\0';
	struct token t;
	int i = 0,count=-1,comma=0;
	char cb;

	ca = fgetc(fin);
	if(ca == EOF){
		strcpy(t.lexeme,"lexeme");
		return t;
	}
	if(ca == '/'){
		cb = fgetc(fin);
		col++;
		if(cb == '/'){
			while(ca != '\n'){
				ca = fgetc(fin);
			}
			row++;
			col = 1;
		}
		else if(cb == '*'){
			do{
				while(ca != '*'){
					ca = fgetc(fin);
					if(ca == '\n'){
						row++;
						col = 1;
					}
					else
						col++;
				}
				ca = fgetc(fin);
				if(ca == '\n'){
					row++;
					col = 1;
				}
				else
					col++;
			}while(ca != '/');
		}
		t.lexeme[0] = '\0';
		return t;
	}

	else if(ca == '\t' || ca == ' ' || ca == '\n'){
		if(ca == '\n'){
			row++;
			col = 1;
		}
		else if(ca == '\t')
			col+=4;
		else
			col++;
		ca = fgetc(fin);
		while(ca == ' ' || ca == '\t' || ca == '\n'){
			if(ca == '\n'){
				row++;
				col = 1;
			}
			else if(ca == '\t')
				col+=4;
			else
				col++;
			ca = fgetc(fin);
		}
		int tf = fseek(fin,-1,SEEK_CUR);
		t.lexeme[0] = '\0';
		return t;
	}

	else if(ca == '"'){
		col++;
		do{
			buf[i++] = ca;
			ca = fgetc(fin);
			if(ca == '\n'){
				row++;
				col = 1;
			}
			else
				col++;
		}while(ca != '"');
		buf[i++] = '"';
		buf[i] = '\0';
		strcpy(t.tok_type,"String Literal");
		// t.lexeme[0] = '\0';
		// return t;
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
			col = 1;
		}
		t.lexeme[0] = '\0';
		return t;
	}

	else if(isalpha(ca) || ca == '_'){
		do{
			col++;
			buf[i++] = ca;
			ca = fgetc(fin);
		}while(isalnum(ca) || ca == '_');
		buf[i] = '\0';

		if(find(buf,keywords,32)){
			strcpy(t.tok_type,"Keyword");
			if(find(buf,types,5)){
				strcpy(ty,buf);
			}
			int tf = fseek(fin,-1,SEEK_CUR);
			// t.lexeme[0] = '\0';
			// return t;
		}
		else{
			strcpy(t.tok_type,"Identifier.");
			if(find(buf,prefun,5)){
				t.lexeme[0] = '\0';
				return t;
			}
			else if(ca=='('){
				strcpy(t.type,"FUNC");
				flag = 1;
				t.scope = 'G';
				strcpy(t.return_type,ty);
				ca = fgetc(fin);
				char arg[100];
				int p=0;
				t.args[0] = '\0';
				while(ca != ')'){
					count++;
					if(ca == ',')
						comma++;
					else if(ca == ' '){
						arg[p] = '\0';
						if(find(arg,types,5) == 0){
							sprintf(arg,"<id,%d> ",Hash(arg));
							strcat(t.args,arg);
						}
						p = 0;
						arg[0] = '\0';
					}
					else
						arg[p++] = ca;
					ca = fgetc(fin);
				}
				if(count>0){
					arg[p] = '\0';
					if(find(arg,types,5) == 0){
						sprintf(arg,"<id,%d> ",Hash(arg));
						strcat(t.args,arg);
					}
					arg[0] = '\0';
					
					t.no_arg = comma+1;
					int tf = fseek(fin,-(count+2),SEEK_CUR);
				}
				else
					t.no_arg = 0;
				t.size = 0;
			}
			else{
				int c = 0;
				strcpy(t.type,ty);
				if(ca == '['){
					ca = fgetc(fin);
					while(ca != ']'){
						char ch[1];
						ch[0] = ca;
						ch[1] = '\0';
						c = c*10 + atoi(ch);
						ca = fgetc(fin);
					}
				}
				if(!c)
					c = 1;
				t.size = size(ty)*c;
				if(flag)
					t.scope = 'L';
				else
					t.scope = 'G';
				t.no_arg = 0;
				t.args[0] = '\0';
				t.return_type[0] = '\0';
			}

			int tf = fseek(fin,-1,SEEK_CUR);
		}
		
	}

	else if(isdigit(ca)){
		col++;
		do{
			buf[i++] = ca;
			ca = fgetc(fin);
		}while(isdigit(ca) || ca=='x' || ca == '.');
		
		buf[i] = '\0';
		strcpy(t.tok_type,"Number");
		int tf = fseek(fin,-1,SEEK_CUR);
		// t.lexeme[0] = '\0';
		// return t;
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
			strcpy(t.tok_type,"Relational Operator.");
			// t.lexeme[0] = '\0';
			// return t;
		}
		else{
			buf[i] = '\0';
			strcpy(t.tok_type,"Assignment Operator.");
			// t.lexeme[0] = '\0';
			// return t;
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
		strcpy(t.tok_type,"Relational Operator.");
		int tf = fseek(fin,f,SEEK_CUR);
		// t.lexeme[0] = '\0';
		// return t;
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
			strcpy(t.tok_type,"Logical Operator.");
			
		}
		int tf = fseek(fin,f,SEEK_CUR);
		// t.lexeme[0] = '\0';
		// return t;
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
			strcpy(t.tok_type,"Logical Operator.");
		}
		int tf = fseek(fin,f,SEEK_CUR);
		// t.lexeme[0] = '\0';
		// return t;
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
			strcpy(t.tok_type,"Relational Operator.");
		}
		buf[i] = '\0';
		strcpy(t.tok_type,"Logical Operator.");
		int tf = fseek(fin,f,SEEK_CUR);
		// t.lexeme[0] = '\0';
		// return t;
	}

	else if(ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%'){
		col++;
		buf[i++] = ca;
		buf[i] = '\0';
		strcpy(t.tok_type,"Arithmetic Operator.");
		// t.lexeme[0] = '\0';
		// return t;
	}  // Make exception for pointers

	else{
		col++;
		buf[i++] = ca;
		buf[i] = '\0';
		if(ca=='}')
			flag = 0;
		strcpy(t.tok_type,buf);
		// t.lexeme[0] = '\0';
		// return t;
	}

	strcpy(t.lexeme,buf);
	t.row = row;
	t.col = col;
	t.index = Hash(t.lexeme);
	return t;
}

