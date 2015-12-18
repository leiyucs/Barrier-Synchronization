#ifndef OPENMPBARRIER_H
#define OPENMPBARRIER_H
#include <stdint.h>

/*sense reversing barrier*/
void mp_senserevbarrier_init (int num_threads);
void mp_senserevbarrier(int num_threads) ;
void mp_senserevbarrier_finalize();


/*MCS tree barrier*/
void mp_mcs_init(int num_threads);
void mp_mcsbarrier(int num_threads);
void mp_mcsbarrier_finalize();

#endif