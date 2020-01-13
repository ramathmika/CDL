#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int rank,size,i,num,s=96;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Status status;
	int *buffer;
	buffer = (int *)malloc(sizeof(int)*200);

	MPI_Buffer_attach(buffer,s);

	int arr[size];

	if(!rank){
		fprintf(stdout,"Process of rank 0.\nEnter the array: ");
		for(i=0;i<size;i++)
			scanf("%d",&arr[i]);

		for(i=1;i<size;i++)
			MPI_Bsend(&arr[i],1,MPI_INT,i,i,MPI_COMM_WORLD);

		fflush(stdout);
	}
	else if(rank%2==0){
		MPI_Recv(&num,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		fprintf(stdout, "\nProcess of rank %d.\nSquare of %d is %d\n",rank,num, num*num);
		fflush(stdout);
	}
	else{
		MPI_Recv(&num,1,MPI_INT,0,rank,MPI_COMM_WORLD,&status);
		fprintf(stdout, "\nProcess of rank %d.\nCube of %d is %d\n",rank,num, num*num*num);
		fflush(stdout);
	}

	MPI_Buffer_detach(&buffer,&s);
	MPI_Finalize();

	return 0;
}