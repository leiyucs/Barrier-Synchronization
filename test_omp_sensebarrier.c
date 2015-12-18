#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <sys/time.h>
#include "openmpbarrier.h"

static double threadstime[256];

double timeduration(struct timeval t1, struct timeval t2)
{
	return (t2.tv_sec-t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec);
}


int main(int argc, char **argv)
{
	int numberofthreads = 0 ;
	int iterations;
	int i;
	if (argc >=3) {
		numberofthreads = atoi(argv[1]);
		iterations = atoi(argv[2]);
	}
	else {
		printf("usage: test numberofthreads numberofiterations (numberofthreads no larger than %d)\n",omp_get_max_threads());
		return 0;
	}
	mp_senserevbarrier_init(numberofthreads);
	double averagetime=0.0;
	#pragma omp parallel firstprivate(numberofthreads, iterations) num_threads(numberofthreads) 
		//#pragma omp parallel num_threads(numberofthreads)
	{
		int j;
		struct timeval t1, t2;
		double duration=0.0;
		int thread_no = omp_get_thread_num();
		threadstime[thread_no]=0;
		gettimeofday(&t1,NULL);
		//printf("thread %d is working\n", thread_no);
		for (j=0; j<iterations; j++) {
			//printf("thread %d is working\n", thread_no);
			mp_senserevbarrier(numberofthreads);
		}
		gettimeofday(&t2,NULL);
		duration = timeduration(t1,t2);
		threadstime[thread_no]+=duration;
	}
	mp_senserevbarrier_finalize();
	for(i=0;i<numberofthreads;i++) {
			averagetime += threadstime[i];
	}
	averagetime = averagetime/numberofthreads;
	printf("average time in %d rounds with %d threads is %f\n", iterations, numberofthreads,averagetime);
	return 0;

}