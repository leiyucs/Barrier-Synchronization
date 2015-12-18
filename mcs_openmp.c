#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>
#include "openmpbarrier.h"
#include <stdio.h>
#include <string.h>

/*MCS tree barrier*/

typedef struct mcs_node_omp
{
	union {
		uint8_t unit[4];
		uint32_t pack;
	}childnotready;
	union { 
		uint8_t unit[4];
		uint32_t pack;
	}havechild;
	uint8_t *parentpointer;
	bool *childpointer[2];
	bool parentsense;
	bool dummy;
	bool sense;
	/* data */
}MCSompNode __attribute__((aligned(8)));

static MCSompNode* nodes = NULL;

/*initize MCS barrier*/
void mp_mcs_init(int num_threads)
{
	int i,j;
	nodes = (MCSompNode *)malloc(sizeof(MCSompNode)*num_threads);
	memset(nodes,0,sizeof(MCSompNode)*num_threads);
	for (i=0; i<num_threads; i++) {
		if (i==0) {
			nodes[i].parentpointer = (uint8_t *)&(nodes[i].dummy);
		}
		else {
			nodes[i].parentpointer = &(nodes[(i-1)/4].childnotready.unit[(i-1)%4]);
		}
		for(j=0;j<4;j++) {
			if(i*4+j+1<num_threads) {
				nodes[i].havechild.unit[j]=1;
			}
			else {
				nodes[i].havechild.unit[j]=0;
			}
		}
		nodes[i].childnotready.pack = nodes[i].havechild.pack;

		//wakeup tree
		for(j=0;j<2;j++) {
			if (i*2+j+1 < num_threads)
				nodes[i].childpointer[j] = &(nodes[i*2+j+1].parentsense);
			else
				nodes[i].childpointer[j] = &(nodes[i].dummy);
		}
		nodes[i].parentsense = false;
		nodes[i].sense = true;
	}

}


void mp_mcsbarrier(int num_threads) 
{
	int threadno = omp_get_thread_num();
	while (nodes[threadno].childnotready.pack);
	nodes[threadno].childnotready.pack = nodes[threadno].havechild.pack;
	*nodes[threadno].parentpointer = 0;
	if (threadno!=0)
		while(nodes[threadno].parentsense!=nodes[threadno].sense);
	*nodes[threadno].childpointer[0] = nodes[threadno].sense;
	*nodes[threadno].childpointer[1] = nodes[threadno].sense;
	nodes[threadno].sense = !nodes[threadno].sense;
	
}


void mp_mcsbarrier_finalize()
{
	free(nodes);
	nodes = NULL;
} 