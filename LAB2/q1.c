#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	int rank,i,len;
	char str[100];

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Status status;

	if(rank==0){
		fprintf(stdout, "Process of rank 0.\nEnter the word: " );
		scanf("%s",str);
		len = strlen(str);
		//len++;

		MPI_Ssend(&len,1,MPI_INT,1,0,MPI_COMM_WORLD);
		MPI_Ssend(&str,len,MPI_CHAR,1,1,MPI_COMM_WORLD);

		MPI_Recv(&str,len,MPI_CHAR,1,2,MPI_COMM_WORLD,&status);

		fprintf(stdout, "\nProcess of rank 0.\nToggled string is: %s\n",str);
		fflush(stdout);
	}
	else{
		MPI_Recv(&len,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&str,len,MPI_CHAR,0,1,MPI_COMM_WORLD,&status);

		fprintf(stdout,"\nProcess of rank 1.\nRecieved string is: %s\n",str);
		for(i=0;i<len;i++)
			if(str[i]>='a' && str[i]<='z')
				str[i] -= 32;
			else
				str[i] += 32;

		MPI_Ssend(&str,len,MPI_CHAR,0,2,MPI_COMM_WORLD);
		fflush(stdout);
	}

	MPI_Finalize();
	return 0;
}