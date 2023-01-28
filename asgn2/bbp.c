#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t terms;

double pi_bbp(void){
    double orig = 47.0/15.0; //first fraction in bbp series
    uint64_t k = 1;
    double sum = orig; //first term
    double exp = 1.0; //holds exponent
    while(absolute(orig) > EPSILON){
        exp = exp * (1.0/16.0);
        orig = exp * ((k*(120.0*k + 151.0)+47.0)/(k*(k*(k*(512.0*k + 1024.0)+712.0)+194.0)+15.0));
        sum += orig;
        k++;
    }
    terms = k;
    return sum;

}

int pi_bbp_terms(void){
    return terms;
}

/*
int main(void){
    double pi = pi_bbp();
    printf("result : %16.15f in %d terms\n", pi, terms);
    return 0;
}
*/
