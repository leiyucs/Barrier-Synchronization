OMPFLAGS = -fopenmp

CC = gcc
CPPFLAGS = -g -Wall $(OMPFLAGS)
LDFLAGS = -g -Wall

#MPICC = /opt/openmpi-1.4.3-gcc44/bin/mpicc
MPICC = mpicc

all: test_omp_sensebarrier test_omp_mcs test_omp_builtin test_mpi_mcs test_mpi_t_barrier test_mpi_builtin test_combinedbarrier

test_omp_sensebarrier: test_omp_sensebarrier.c sensereversal_openmp.c
	$(CC) $(CPPFLAGS) -o $@ $^

test_omp_mcs: test_omp_mcs.c mcs_openmp.c
	$(CC) $(CPPFLAGS) -o $@ $^

test_omp_builtin: test_omp_builtin.c
	$(CC) $(CPPFLAGS) -o $@ $^

test_mpi_mcs: test_mpi_mcs.c mcs_mpi.c
	$(MPICC) -o $@ $^

test_mpi_t_barrier: test_mpi_t_barrier.c tournament_barrier.c
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

test_mpi_builtin: test_mpi_builtin.c
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

test_combinedbarrier: test_combinedbarrier.c combinedbarrier.c mcs_mpi.c sensereversal_openmp.c
	$(MPICC) -o $@ $(CPPFLAGS) $^

clean:
	rm -f *.o test_omp_sensebarrier test_omp_mcs test_omp_builtin test_mpi_mcs test_mpi_t_barrier test_mpi_builtin test_combinedbarrier
