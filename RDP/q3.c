/*
	S --> aAcBe

	A --> bA'

	A' --> bA' / e

	B --> d

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ca;
	
void S();
void A();
void Aprime();
void B();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void invalid()
{
	printf("INVALID\n");
	exit(0);
}


char getNextToken(FILE *fp)
{	
	char c = getc(fp);
	if(c != EOF)
		return c;
	invalid();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void S(FILE *fp)
{

	if(ca == 'a')
	{
		ca = getNextToken(fp);
		A(fp);
		if(ca == 'c')
		{
			ca = getNextToken(fp);
			B(fp);
			if(ca == 'e')
			{
				ca = getNextToken(fp);
				return;
			}
			else
				invalid();
		}
		else
			invalid();
	}
	else
		invalid();
}

void A(FILE *fp)
{
	if(ca == 'b')
	{
		ca = getNextToken(fp);
		Aprime(fp);
	}
	else
		invalid();
}

void Aprime(FILE *fp)
{
	if(ca == 'b')
	{
		ca = getNextToken(fp);
		Aprime(fp);
	}
}

void B(FILE *fp)
{
	if(ca == 'd')
	{
		ca = getNextToken(fp);
		return;
	}
	else
		invalid();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	FILE *fp = fopen("s3.txt","r");
	if (fp == NULL)
		exit(0);
	ca = getNextToken(fp);
	S(fp);
	if(ca == '#')
		printf("Sucessfully Parsed\n");
	else
		printf("Error in Parsing\n");
	fclose(fp);
}