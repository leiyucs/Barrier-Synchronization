#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>
#include <omp.h>
#include <sys/time.h>
#include "combinedbarrier.h"

static double threadstime[256];

double timeduration(struct timeval t1, struct timeval t2)
{
	return (t2.tv_sec-t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec);
}

int main(int argc, char **argv)
{
	int iterations, rank;
	int i, numberofprocess, numberofthreads;
	double averagetime = 0;
	MPI_Init(&argc, &argv);
	if (argc >=2) {
		numberofthreads = atoi(argv[1]);
		iterations = atoi(argv[2]);
	}
	else {
		printf("usage: test numberofthreads numberofiterations");
		return 0;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numberofprocess);

	combinedbarrier_init(numberofprocess, numberofthreads, rank);
	#pragma omp parallel num_threads(numberofthreads)
	{
		int j;
		struct timeval t1, t2;
		int thread_no = omp_get_thread_num();
		//printf("thread %d:%d is working\n", rank, thread_no);
		gettimeofday(&t1,NULL);
		for (j=0; j<iterations; j++) {
			combinedbarrier(numberofprocess,numberofthreads,rank);
		}
		gettimeofday(&t2,NULL);
		threadstime[thread_no] = timeduration(t1,t2);
	}

	combinedbarrier_finalize();
	for(i=0; i<numberofthreads; i++) {
		averagetime += threadstime[i];
	}

	printf("average time in %d rounds on %d process is %f\n", iterations, rank, averagetime/numberofthreads);
	
	MPI_Finalize();
	return 0;
}
