#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

static int iterations;
static double threadstime[256];

double timeduration(struct timeval t1, struct timeval t2)
{
    return (t2.tv_sec-t1.tv_sec)*1000000+(t2.tv_usec-t1.tv_usec);
}


int main(int argc, char **argv)
{
    int thread_no; 
    int numberofthreads = 0 ;
    int i;
    if (argc >=3) {
        numberofthreads = atoi(argv[1]);
        iterations = atoi(argv[2]);
    }
    else {
        printf("usage: test numberofthreads numberofiterations (numberofthreads no larger than %d)\n",omp_get_max_threads());
        return 0;
    }
    double averagetime=0.0;
    #pragma omp parallel private(thread_no) num_threads(numberofthreads)
    //#pragma omp parallel num_threads(numberofthreads)
    {
        struct timeval t1, t2;
        double duration=0.0;
        thread_no = omp_get_thread_num();
        int i;
        //printf("thread %d is working\n", thread_no);
        threadstime[thread_no]=0;
        gettimeofday(&t1,NULL);
        for (i=0; i<iterations; i++){
            #pragma omp barrier
            //printf("thread %d is reached barrier %d\n", thread_no, i);
        }
        gettimeofday(&t2,NULL);
        duration = timeduration(t1,t2);
        threadstime[thread_no]+=duration;
    }
    
    for(i=0;i<numberofthreads;i++) {
        averagetime += threadstime[i];
    }
    averagetime = averagetime/numberofthreads;
    //printf("%f\n", averagetime);
    printf("average time in %d rounds with %d threads is %f\n", iterations, numberofthreads,averagetime);
    return 0;
}
