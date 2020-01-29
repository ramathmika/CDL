%{
	int vc = 0;
	int cc = 0;	
%}

%%

[aeiou] {vc++;}
[a-zA-Z] cc++;

%%

int yywrap(){
	return 1;
}

int main(){
	yylex();

	printf("Vowels: %d, Consonants: %d\n",vc,cc);
}