#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t terms;

//Approximation of pi using Euler Series
double pi_euler(void){
    double orig = 1; //term 0. Term 0 is 1 because Euler requires division, and dividing by 0 if undefined
    uint_fast64_t k = 1; //counter
    double sum = 0; //holds sum
    while(absolute(orig) > EPSILON){
        orig = 1.0/(k * k); //summation equation
        sum += orig; //adding to sum
        k++; //counter incremenation
        //printf("%16.15f\n", orig); //used only for plot.sh
    }
    terms = k;
    sum = 6.0 * sum;
    sum = sqrt_newton(sum);
    return sum;
}

//returns number of terms pi_viete() goes through
int pi_euler_terms(void){
    return terms;
}

