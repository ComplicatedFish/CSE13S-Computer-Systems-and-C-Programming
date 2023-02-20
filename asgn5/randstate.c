#include <stdio.h>
#include <stdint.h>
#include <gmp.h>
#include <stdlib.h>

gmp_randstate_t state;

void randstate_init(uint64_t seed){
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
    srandom(seed);
    return;
}

void randstate_clear(void){
    gmp_randclear(state);
    return;
}
