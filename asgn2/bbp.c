#include "mathlib.h"

#include <stdint.h>
#include <stdio.h>

static uint32_t terms;

//approximation of pi using the Bailey-Borwein-Plouffe series
double pi_bbp(void) {
    double orig = 47.0 / 15.0; //first fraction in bbp series
    uint64_t k = 1; //counter
    double sum = orig; //sum holder, starts off with term 1 added
    double exp = 1.0; //holds exponent
    while (absolute(orig) > EPSILON) {
        exp = exp * (1.0 / 16.0);
        orig = exp
               * ((k * (120.0 * k + 151.0) + 47.0)
                   / (k * (k * (k * (512.0 * k + 1024.0) + 712.0) + 194.0) + 15.0));
        sum += orig;
        k++;
        //printf("%16.15f\n", sum);
    }
    terms = k;
    return sum;
}

//returns number of terms that pi_bbp() goes through
int pi_bbp_terms(void) {
    return terms;
}
