%{
	int sc = 0;
	int pr = 0;	
%}

%%

"scanf" {sc++; fprintf(yyout,"READ");} 
"printf" {pr++; fprintf(yyout,"WRITE");}

%%

int yywrap(){
	return 1;
}

int main(int argc, char **argv){
	yylex();
	
	if(argc!=3)
		exit(0);
	yyin = fopen(argv[1],"r");
	yyout = fopen(argv[2],"w");

	printf("scanf() count: %d\nprintf() count: %d\n",sc,pr);
}