#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "randstate.h"
#include <math.h>

#define uint uint32_t

extern gmp_randstate_t state;

//sets g to the gcd of a and b. Uses Euclid's
//algorithm to calculate gcd.
void gcd(int *g, int a, int b){//*mpz_t g, mpz_t a, mpz_t b){
    int t;
    while (b != 0){
        t = b;
        b = a % b;
        a = t;
    }
    *g = a;
    return;
}


void mod_inverse(int *i, int a, int n){//mpz_t o, mpz_t a, mpz_t n){
    int r = n;
    int r2 = a;
    int t = 0;
    int t2 = 1;
    int q;
    int temp_t;
    int temp_r;

    while (r2 != 0){
        q = r/r2;

        temp_r = r;
        r = r2;
        r2 = temp_r - (q*r2);

        temp_t = t;
        t = t2;
        t2 = temp_t - (q*t2);
    }
    if (r > 1){
        *i = 0;
    }
    if (t < 0){
        t += n;
    }
    *i = t;
}

/*
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n){

}

bool is_prime(mpz_t n, uint64_t iters){

}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters){

}*/

int main(void){
    int g = 0;
    int i = 0;
    gcd(&g, 345756848, 76784448);
    printf("the gcd is %d\n", g);
    mod_inverse(&i, 9, 50);
    printf("the mod_inverse is %d\n", i);
    return 0;
}
