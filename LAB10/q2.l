%{
	int chars = 0;
	int words = 0;
	int lines = 0;
	int blanks = 0;
%}

%%

[a-zA-Z]+ {words++;chars += strlen(yytext);}
\n {chars++;lines++;}
[ ] {blanks++;}
. {chars++;}

%%

int yywrap()
{
	return 1;
}

main(int argc,char **argv){
	yylex();

	printf("Lines:%8d\nWords:%8d\nCharacters:%8d\nBlanks:%8d\n",lines,words,chars,blanks);
}