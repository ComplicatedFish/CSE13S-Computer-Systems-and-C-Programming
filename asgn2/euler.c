#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t terms;

double pi_euler(void){
    double orig = 1;
    uint_fast64_t k = 1;
    double sum = 0;
    //uint_fast64_t exp = 0;
    //uint_fast64_t add = 1;
    while(absolute(orig) > EPSILON){
        orig = 1.0/(k * k);//(exp + add); //summation equation
        //exp = exp + add;
        //add += 2;
        sum += orig; //adding to sum
        k++; //counter incremenation
        //printf("%16.15f\n", orig);
    }
    terms = k;
    sum = 6.0 * sum;
    sum = sqrt_newton(sum);
    return sum;
}

int pi_euler_terms(void){
    return terms;
}

/*
int main(void){
    double pi = pi_euler();
    printf("result : %16.15f in %d terms\n", pi, terms);
    return 0;
}
*/
