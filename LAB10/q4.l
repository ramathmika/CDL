%{
	int sc = 0;
	int pr = 0;	
%}

%%

"scanf" {sc++; strcpy(yytext,"READ");} 
"printf" {pr++; strcpy(yytext,"WRITE");}

%%

int yywrap(){
	return 1;
}

int main(){
	yylex();
	printf("scanf() count: %d\nprintf() count: %d\n"sc,pr);
}