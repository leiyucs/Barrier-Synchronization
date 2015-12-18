#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <sys/time.h>
#include <math.h>

double timeduration(struct timeval t1, struct timeval t2)
{
    return (t2.tv_sec-t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec);
}

int main(int argc, char **argv)
{
    int iterations, rank;
    int numberofprocess;
    MPI_Init(&argc, &argv);
    if (argc >=2) {
        iterations = atoi(argv[1]);
    }
    else {
        printf("usage: test numberofiterations");
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberofprocess);
    
    int j;
    struct timeval t1, t2;
    double duration=0.0;
    //printf("process %d is working\n", rank);
    gettimeofday(&t1,NULL);
    for (j=0; j<iterations; j++) {
        MPI_Barrier(MPI_COMM_WORLD);
    }
    gettimeofday(&t2,NULL);


    duration = timeduration(t1,t2);
    printf("average time in %d rounds on %d process is %f\n", iterations, rank, duration);
    
    MPI_Finalize();
    return 0;
}