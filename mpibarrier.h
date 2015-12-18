#ifndef MPIBARRIER_H
#define MPIBARRIER_H

/*MCS tree barrier*/
void mpi_mcs_init(int num_threads, int rank);
void mpi_mcsbarrier(int num_threads, int rank);
void mpi_mcsbarrier_finalize();

/*Tournament barrier*/
void tournament_barrier(int rank, int numprocs, int max_round);

#endif