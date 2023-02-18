#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "randstate.h"
#include <math.h>
#include <stdlib.h>

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


void pow_mod(int *o, int a, int d, int n){//mpz_t o, mpz_t a, mpz_t d, mpz_t n){
    int v = 1;
    int p = a;
    while (d > 0){
        if ((d%2) == 1){
            v = (v * p) % n;
        }
        p = (p*p) % n;
        d /= 2;
    }
    *o = v;
    return;
}


bool is_prime(int *n, int iters){//mpz_t n, uint64_t iters){
    int r = (*n-1)/2;
    int s = 1;
    while (r % 2 == 0){
        r /= 2;
        s++;
    }

    int a;
    for (int i = 1; i < iters; i++){
        a = random() % ((*n) - 4);
        a += 2;
        int y = 0;
        int x = 0;
        pow_mod(&y, a, r, *n);
        if (y != 1 && y!= (*n)-1){
            int j = 0;
            while (j <= s-1 && y != (*n)-1){
                x = y;
                pow_mod(&y, x, 2, *n);
                if (y == 1){
                    return false;
                }
                j += 1;
            }
            if (y != (*n)-1){
                return false;
            }
        }
    }
    return true;
}

/*
void make_prime(int *p, int bits, int iters){//mpz_t p, uint64_t bits, uint64_t iters){
    
}
*/
int main(void){
    int g = 0;
    int i = 0;
    int out = 0;
    int n = 740651;
    gcd(&g, 345756848, 76784448);
    printf("the gcd is %d\n", g);
    mod_inverse(&i, 9, 50);
    printf("the mod_inverse is %d\n", i);
    pow_mod(&out, 55, 67, 84);
    printf("the pow_mod is %d\n", out);
    printf(is_prime(&n, 40) ? "true\n" : "false\n");
    return 0;
}

