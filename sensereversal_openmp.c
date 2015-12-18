#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>
#include "openmpbarrier.h"
#include <stdio.h>

static int count;
static bool sense;

typedef union localsense_t {
	bool lsense;
	long whole;
}localsense_t;

static localsense_t *localsense_list=NULL;

void mp_senserevbarrier_init (int num_threads)	{
	int i;
	sense = true;
	count = num_threads;
	if (localsense_list==NULL) {
		localsense_list = (localsense_t *)malloc(sizeof(localsense_t)*num_threads);
	}
	for (i=0; i<num_threads; i++) {
		localsense_list[i].lsense=true;
	}
}


void mp_senserevbarrier(int num_threads) {
	int threadno = omp_get_thread_num();
	localsense_list[threadno].lsense = !localsense_list[threadno].lsense;

	if (__sync_fetch_and_sub (&count, 1) == 1) {
		count = num_threads;
		sense = localsense_list[threadno].lsense;
	}
	else {
		while(sense != localsense_list[threadno].lsense){};
	}
}

void mp_senserevbarrier_finalize(int num_threads) {
	free(localsense_list);
	localsense_list=NULL;
}