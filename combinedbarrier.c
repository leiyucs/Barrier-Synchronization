#include "mpibarrier.h"
#include "openmpbarrier.h"
#include <stdio.h>
#include <omp.h>

/*Combined barrier*/
void combinedbarrier_init(int num_process, int num_threads, int rank)
{
	mpi_mcs_init(num_process, rank); 
	mp_senserevbarrier_init(num_threads);
}


void combinedbarrier(int num_process, int num_threads, int rank)
{
	mp_senserevbarrier(num_threads);
	if (omp_get_thread_num()==0)
	{
		mpi_mcsbarrier(num_process,rank);
	}
	mp_senserevbarrier(num_threads);
}

void combinedbarrier_finalize(){
	mp_senserevbarrier_finalize();
	mpi_mcsbarrier_finalize();
}

