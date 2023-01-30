#include "mathlib.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

static uint32_t iters;

//this function is built upon and translated from python code provided by prof.Long in
//portable.py
double sqrt_newton(double x) {
    iters = 0; //resets iters
    double scale = 1;
    double orig = 1;

    //this loop takes out factors of 4 from x.
    //Each 4 taken out means 2 is multiplied to scale
    //(since 4 is a perfect square and sqrt(4) = 2)
    //scale will be multiplied later into the final result
    while (x > 4.0) {
        x = (x / 4.0);
        scale = scale * 2;
    }

    double guess = 0; //guess will hold the previous value of orig
        //and will be used to check if orig has reached
        //a degree of precision that is < EPSILON
    while (absolute(orig - guess) > EPSILON) {
        guess = orig;
        orig = (orig + (x / orig)) / 2;
        iters++;
    }
    return scale * orig;
}

//returns number of iterations sqrt_newton() took
int sqrt_newton_iters(void) {
    return iters;
}
