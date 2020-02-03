#ifndef _MT19937ARCLASS_H_
#define _MT19937ARCLASS_H_

#include <stdio.h>
/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

class MersenneTwister
{
    private:
    unsigned long mt[N]; /* the array for the state vector  */
    int mti=N+1; /* mti==N+1 means mt[N] is not initialized */


    public:    
    void init_genrand(unsigned long s); /* initializes mt[N] with a seed */
    /* initialize by an array with array-length */
    /* init_key is the array for initializing keys */
    /* key_length is its length */
    /* slight change for C++, 2004/2/26 */
    void init_by_array(unsigned long init_key[], int key_length);

    unsigned long genrand_int32(void); /* generates a random number on [0,0xffffffff]-interval */
    long genrand_int31(void); /* generates a random number on [0,0x7fffffff]-interval */
    double genrand_real1(void); /* generates a random number on [0,1]-real-interval */
    double genrand_real2(void); /* generates a random number on [0,1)-real-interval */
    double genrand_real3(void); /* generates a random number on (0,1)-real-interval */
    double genrand_res53(void);  /* generates a random number on [0,1) with 53-bit resolution*/
    double genrand_real_range(double range_low, double range_high); // generate random between range_low and range_high

};

#endif