#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t terms;

//Approximation of pi using the viete series
double pi_viete(void){
    double orig = sqrt_newton(2.0); //first term
    uint64_t k = 1; //iteration counter
    double product = orig/2.0; //product holder starts with first term added
                               //for ease of use in the loop
    double temp = orig; //to hold numerator temporarily
    double prevproduct = 0; //holding the previous product allows checking
                            //if the next products value is significantly different.
                            //This allows an exit condition to be set

    while(absolute(product - prevproduct) > EPSILON){
        prevproduct = product; //holds the previous product
        temp = temp + 2.0;
        orig = sqrt_newton(temp)/2.0;
        temp = sqrt_newton(temp);
        product = product * orig;
        k++;
        //printf("%16.15f\n", 2.0/product);
    }
    terms = k;
    product = 2.0/product;
    return product;
}

//returns number of iterations pi_viete() went through
int pi_viete_factors(void){
    return terms;
}

