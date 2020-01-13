#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]){
	int rank,size,i,sum1,sum2,sum;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;

	if(rank==0){
		sum = sum1 = sum2 = 1;

		for(i=1;i+1<size;i+=2){
			MPI_Ssend(&sum1,1,MPI_INT,i,i,MPI_COMM_WORLD);
			MPI_Recv(&sum1,1,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			MPI_Ssend(&sum2,1,MPI_INT,i+1,i+1,MPI_COMM_WORLD);
			MPI_Recv(&sum2,1,MPI_INT,i+1,i+1,MPI_COMM_WORLD,&status);
			sum += sum1 + sum2;
		}

		fprintf(stdout, "\nFinal sum: %d\n",sum);
		fflush(stdout);
	}
	else if(rank%2==0){
		MPI_Recv(&sum,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		sum += rank;
		MPI_Send(&sum,1,MPI_INT,0,rank,MPI_COMM_WORLD);
	}
	else{
		MPI_Recv(&sum,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		sum *= rank;
		MPI_Send(&sum,1,MPI_INT,0,rank,MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}