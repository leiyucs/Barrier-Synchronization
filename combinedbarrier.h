#ifndef COMBINED_BARRIER_H
#define COMBINED_BARRIER_H

/*Combined barrier*/
void combinedbarrier_init(int num_process, int num_threads, int rank);
void combinedbarrier(int num_process, int num_threads, int rank);
void combinedbarrier_finalize();



#endif
