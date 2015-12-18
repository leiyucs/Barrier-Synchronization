#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

#define NUM_BARRIERS 10

int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

void tournament_barrier( int rank, int numprocs, int max_round) {
    int tag = 1;
    int finished = 1;
    int self = 0;
    int curr_round;
    
    for(curr_round = 1; curr_round <= max_round; curr_round ++){
        if(rank % ipow(2, curr_round) == 0){
            if(rank + ipow(2, curr_round - 1) < numprocs){
                MPI_Recv( &self, 1, MPI_INT, rank + ipow(2, curr_round - 1), 
                    tag, MPI_COMM_WORLD, NULL);
            }
        }
        else {
            MPI_Send( &finished, 1, MPI_INT, rank - ipow(2, curr_round - 1), tag, MPI_COMM_WORLD);
            MPI_Recv( &self, 1, MPI_INT, rank - ipow(2, curr_round - 1), 
                tag, MPI_COMM_WORLD, NULL);
            break;
        }
    }

    for(curr_round = max_round; curr_round >= 1; curr_round --){
        if(rank % ipow(2, curr_round) == 0){
            if(rank + ipow(2, curr_round - 1) < numprocs){
                MPI_Send( &finished, 1, MPI_INT, rank + ipow(2, curr_round - 1), tag, MPI_COMM_WORLD);
            }
        }
    }
}
