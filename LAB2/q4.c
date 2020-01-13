#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	int rank,size,num;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	
	if(!rank){
		fprintf(stdout, "Process of rank 0.\nEnter the number: ");
		scanf("%d",&num);

		MPI_Send(&num,1,MPI_INT,1,0,MPI_COMM_WORLD);

		MPI_Recv(&num,1,MPI_INT,size-1,size-1,MPI_COMM_WORLD,&status);
		fprintf(stdout, "\nProcess of rank 0.\nFinal recieved number: %d\n",num);
		fflush(stdout);
	}
	else{
		MPI_Recv(&num,1,MPI_INT,rank-1,rank-1,MPI_COMM_WORLD,&status);
		fprintf(stdout, "\nProcess of rank %d.\nRecieved number: %d\n",rank,num);
		num++;

		if(rank==size-1)
			MPI_Send(&num,1,MPI_INT,0,rank,MPI_COMM_WORLD);
		else
			MPI_Send(&num,1,MPI_INT,rank+1,rank,MPI_COMM_WORLD);

		fflush(stdout);
	}

	MPI_Finalize();

	return 0;

}