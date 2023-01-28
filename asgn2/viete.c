#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"
#include <unistd.h>

static uint32_t terms;

double pi_viete(void){
    double orig = sqrt_newton(2.0);
    uint64_t k = 1;
    double product = orig/2.0; //first term
    double temp = orig; //to hold numerator temporarily
    double prevproduct = 0;

    while(absolute(product - prevproduct) > EPSILON){
        prevproduct = product; //holds the previous product
        temp = temp + 2.0;
        orig = sqrt_newton(temp)/2.0;
        temp = sqrt_newton(temp);
        product = product * orig;
        k++;
        //printf("%16.15f\n", product);
        if (k > 10000){
            break;
        }
        //prevproduct = product;

        //sleep(1);
    }
    terms = k;
    product = 2.0/product;
    return product;
}

int pi_viete_factors(void){
    return terms;
}

int main(void){
    double pi = pi_viete();
    printf("result : %16.15f in %d terms\n", pi, terms);
    return 0;
}
