#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	int rank,size,num,i;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		fprintf(stdout, "Process of rank 0.\nEnter a number: ");
		scanf("%d",&num);
		fflush(stdout);

		for(i=1;i<size;i++)
			MPI_Send(&num,1,MPI_INT,i,i,MPI_COMM_WORLD);

		
	}
	else{
		MPI_Recv(&num,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		fprintf(stdout,"\nProcess of rank %d.\nRecieved number is: %d\n",rank,num);
		fflush(stdout);
	}

	MPI_Finalize();

	return 0;
}