#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"
#include <unistd.h>

static uint32_t iters;

//this function is built upon and translated from python code provided by prof.Long in
//portable.py
double sqrt_newton(double x){
    iters = 0; //resets iters
    double scale = 1;
    double orig = 1;

    //this loop takes out factors of 4 from
    //
    while(x > 4.0){
        x = (x/4.0);
        scale = scale * 2;
    }
    double guess = 0;
    while(absolute(orig - guess) > EPSILON){
        guess = orig;
        orig = (orig + (x/orig)) / 2;
        iters++;
        //printf("%16.15f\n", orig);
        //sleep(1);
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
