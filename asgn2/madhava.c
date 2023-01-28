#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t terms;

double pi_madhava(void){
    double orig = 1; //first term (when k = 0)
    unsigned int k = 1;
    double sum = 1;
    double exp = 1; //holds exponent (-3)^k each loop. -3^0 = 1
    while(absolute(orig) > EPSILON){
        exp = exp * (1.0/-3.0);
        orig = (1.0/(2.0*k+1.0))*exp; //simplified: 1/(-3(2k+1))
        sum += orig;
        k++;
        //printf("%16.15f\n", sqrt_newton(12) * sum);
        //if (k > 100){
        //    return sum;
        //}
    }
    terms = k;
    sum = sum * sqrt_newton(12);
    return sum;

}

int pi_madhava_terms(void){
    return terms;
}

/*
int main(void){
    double pi = pi_madhava();
    printf("result : %16.15f in %d terms\n", pi, terms);
    return 0;
}
*/
