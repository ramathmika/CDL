%{
	int pos_int = 0;
	int neg_int = 0;
	int pos_float = 0;
	int neg_float = 0;
%}

%%

[0-9]+[.][0-9]+ pos_float++;
[-][0-9]+[.][0-9]+ neg_float++;
[0-9]+ pos_int++;
[-][0-9]+ neg_int++;

%%

int yywrap(){
	return 1;
}

int main(){
	yylex();
	printf("Positive integrs: %d\nNegative Integers: %d\nPositive Floating point: %d\nNegative Floating point: %d\n",pos_int,neg_int,pos_float,neg_float);
}