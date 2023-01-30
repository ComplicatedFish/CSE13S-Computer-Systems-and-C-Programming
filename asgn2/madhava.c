#include "mathlib.h"

#include <stdint.h>
#include <stdio.h>

static uint32_t terms;

//approximation of PI using the madhava series
double pi_madhava(void) {
    double orig = 1; //first term (when k = 0)
    unsigned int k = 1; //counter for iterations
    double sum = 1; //holds sum that is added to each loop
    double exp = 1; //holds exponent (-3)^k each loop. -3^0 = 1
    while (absolute(orig) > EPSILON) {
        exp = exp * (1.0 / -3.0);
        orig = (1.0 / (2.0 * k + 1.0)) * exp;
        sum += orig;
        k++;
        //printf("%16.15f\n", sqrt_newton(12) * sum); //here for plot.sh, left in for posterity
    }
    terms = k;
    sum = sum * sqrt_newton(12);
    return sum;
}

//returns number of terms pi_madhava() took
int pi_madhava_terms(void) {
    return terms;
}
