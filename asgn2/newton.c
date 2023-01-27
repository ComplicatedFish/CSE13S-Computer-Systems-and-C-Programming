#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t iters = 1;

//this function is built upon and translated from python code provided by prof.Long in
//portable.py
double sqrt_newton(double x){
    double scale = 1;
    double orig = 1;

    //this loop takes out factors of 4 from
    //
    while(x > 4.0){
        x = (x/4.0);
        scale = scale * 2;
    }
    double guess = 0;
    while(absolute(orig - guess)){
        guess = orig;
        orig = (orig + (x/orig)) / 2;
        iters++;
    }
    return scale * orig;
}

int sqrt_newton_iters(void){
    return iters;
}

/*
int main(void){
    double newton = sqrt_newton(12);
    printf("result : %16.15f in %d terms\n", newton, iters);
    return 0;
}
*/
