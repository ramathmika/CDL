#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	int rank,size,x;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(!rank){
		fprintf(stdout,"Process of Rank 0. Enter value: ");
		scanf("%d",&x);
		MPI_Send(&x,1,MPI_INT,1,0,MPI_COMM_WORLD);
		fflush(stdout);
	}
	else{
		MPI_Recv(&x,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		fprintf(stdout, "Process of Rank 1. Value recieved is: %d\n",x);
		fflush(stdout);
	}
	MPI_Finalize();
	return 0;
}