#include <stdio.h>
#include <stdint.h>
#include "mathlib.h"

static uint32_t terms; //unsigned so value of int can be greater

//Approximation of Euler's number (e)
//This code it built and translated from the python code provided by Prof, long
//in portable.py
double e(void){
    double orig = 1; //1 is the first term in the series (orig = origin), also acts as a temp var
    double sum = 1; //holds sum for series, includes term 1 from above
    int k = 1; //counter for terms, set to 1 since term 1 is calculated above
    while(absolute(orig) > EPSILON){
        orig = orig * (1.0/k);
        sum += orig;
        k++;
    }
    terms = k;
    return sum;
}

//returns nmber of terme e() went through
int e_terms(void){
    return terms;
}

