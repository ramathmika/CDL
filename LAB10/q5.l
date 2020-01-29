%{
	int flag = 0;
%}

%%

"and"|"or"|"but"|"while"|"whereas" flag++;

%%

int yywrap(){
	return 1;
}

int main(){
	yylex();
	if(flag != 0)
		printf("Compound\n");
	else
		printf("Simple\n");
}