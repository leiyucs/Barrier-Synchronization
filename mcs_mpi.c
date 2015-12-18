#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "mpibarrier.h"
#include <stdio.h>
#include <string.h>

typedef struct mcs_node_mpi
{
	int arrivalchildpointer[4];
	union { 
		uint8_t unit[4];
		uint32_t pack;
	}havechild;
	int arrivalparentpointer;
	int wakeupparentpointer;
	int wakeupchildpointer[2];
} MCSmpiNode;



static MCSmpiNode *mynode;

void mpi_mcs_init(int num_process, int rank) {
	mynode = (MCSmpiNode *) malloc(sizeof(MCSmpiNode));
	int i;
	for (i=0; i<4; i++) {
		if (4*rank+i+1<num_process) {
			mynode->havechild.unit[i]=1;
			mynode->arrivalchildpointer[i] = 4*rank+i+1;
		}
		else {
			mynode->havechild.unit[i]=0;
			mynode->arrivalchildpointer[i]=-1;
		}

	}
	if (rank!=0) {
		mynode->arrivalparentpointer = (rank-1)/4;
		mynode->wakeupparentpointer = (rank-1)/2;
	}
	else {
		mynode->arrivalparentpointer = -1;
		mynode->wakeupparentpointer = -1;
	}
	for (i=0; i<2; i++)	{
		if (2*rank+i+1<num_process)
			mynode->wakeupchildpointer[i] = 2*rank+i+1;
		else
			mynode->wakeupchildpointer[i] = -1;
	}

}


void mpi_mcsbarrier(int num_process, int rank) {
	int i, tag=1;
	unsigned char msg=1;
	MPI_Status status;
	for (i=0; i<4; i++) {
		if (mynode->havechild.unit[i]==1)
			MPI_Recv(&msg, 1, MPI_BYTE, mynode->arrivalchildpointer[i], tag, MPI_COMM_WORLD, &status);
	}

	if (rank!=0) {
		MPI_Send(&msg, 1, MPI_BYTE, mynode->arrivalparentpointer, tag, MPI_COMM_WORLD);
		MPI_Recv(&msg, 1, MPI_BYTE, mynode->wakeupparentpointer, tag, MPI_COMM_WORLD, &status); 
	}
	
	for (i=0; i<2; i++)	{
		if (mynode->wakeupchildpointer[i]!=-1)
		{
			MPI_Send(&msg, 1, MPI_BYTE, mynode->wakeupchildpointer[i], tag, MPI_COMM_WORLD);
		}
		
	}
}

void mpi_mcsbarrier_finalize() {

	free(mynode);
	
}