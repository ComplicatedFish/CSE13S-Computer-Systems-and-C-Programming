#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "randstate.h"
#include <math.h>
#include <stdlib.h>

//#define uint uint32_t

extern gmp_randstate_t state;

/*In this program, I first implemented all the functions
 * using real C. I have commented out the pure C implementation
 * next to the GMP implementation for code clarity. The comments
 * in the function header acount for the parameters I used
 * for testing
 * */

//sets g to the gcd of a and b. Uses Euclid's
//algorithm to calculate gcd.
void gcd(mpz_t g, const mpz_t a, const mpz_t b){ //int *g, int a, int b
    mpz_t temp;     //temporary variable to hold value of b in loop
    mpz_init(temp);

    mpz_t temp_a;   //holds a within this function
    mpz_t temp_b;   //holds b within this function
    mpz_init(temp_a);
    mpz_init(temp_b);

    mpz_set(temp_a, a);
    mpz_set(temp_b, b);

    while (mpz_cmp_ui(temp_b, 0) != 0){
        mpz_set(temp, temp_b);
        mpz_mod(temp_b, temp_a, temp_b);
        mpz_set(temp_a, temp);
    }
    mpz_set(g, temp_a);
    mpz_clears(temp, temp_a, temp_b, NULL);
    return;

/*
    int t;
    while (b != 0){
        t = b;
        b = a % b;
        a = t;
    }
    *g = a;
    return;
*/
}


void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n){ //int *i, int a, int n
    mpz_t r;
    mpz_t r2;
    mpz_t t;
    mpz_t t2;
    mpz_t q;
    mpz_t temp_t;
    mpz_t temp_r;
    mpz_t temp; //all purpose temp var

    mpz_init(r);
    mpz_init(r2);
    mpz_init(t);
    mpz_init(t2);
    mpz_init(q);
    mpz_init(temp_t);
    mpz_init(temp_r);
    mpz_init(temp);

    mpz_set(r, n);      //int r = n;
    mpz_set(r2, a);     //int r2 = a;
    mpz_set_ui(t, 0);   //int t = 0;
    mpz_set_ui(t2, 1);  //int t2 = 1;
    //int q;
    //int temp_t;
    //int temp_r;

    while (mpz_cmp_ui(r2, 0) != 0){ //while (r2 != 0){
        mpz_fdiv_q(q, r, r2);       //q = r/r2;

        mpz_set(temp_r, r);         //temp_r = r;
        mpz_set(r, r2);             //r = r2;
        mpz_mul(temp, q, r2);       //temp holds (q*r2);
        mpz_sub(r2, temp_r, temp);  //r2 = temp_r - (q*r2);

        mpz_set(temp_t, t);         //temp_t = t;
        mpz_set(t, t2);             //t = t2;
        mpz_mul(temp, q, t2);       //temp holds (q*t2)
        mpz_sub(t2, temp_t, temp);  //t2 = temp_t - (q*t2);
    }
    if (mpz_cmp_ui(r, 1) > 0){  //if (r > 1){
        mpz_set_ui(t, 0);       //*i = 0;
    }
    if (mpz_cmp_ui(t, 0) < 0){  //if (t < 0){
        mpz_add(t, t, n);       //t += n;
    }
    mpz_set(o, t);              //*i = t;
    mpz_clears(r, r2, t, t2, temp_t, temp_r, temp, NULL);
}


void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n){   //int *o, int a, int d, int n
    mpz_t v;
    mpz_t p;
    mpz_t temp_d;
    //mpz_t d_mod2;   //in this conditional below, holds d%2

    mpz_inits(v, p, temp_d, /*d_mod2,*/ NULL);

    mpz_set_ui(v, 1);   //int v = 1;
    mpz_set(p, a);      //int p = a;
    mpz_set(temp_d, d); //int temp_d = d;

    while (mpz_cmp_ui(temp_d, 0) > 0){      //while (d > 0)
        if (mpz_odd_p(temp_d)){;            //if ((d%2) == 1)
            mpz_mul(v, v, p);               //v = v * p
            mpz_mod(v, v, n);               //v = v % n;
        }
        mpz_mul(p, p, p);   //p = p*p
        mpz_mod(p, p, n);   //p = (p*p) % n;
        mpz_fdiv_q_ui(temp_d, temp_d, 2);    //d /= 2

    }
    mpz_set(o, v);  //*o = v;
    return;
}

/*
bool is_prime(int *n, int iters){//mpz_t n, uint64_t iters){
    int r = (*n-1)/2;
    int s = 1;
    while (r % 2 == 0){
        r /= 2;
        s++;
    }
    printf("s = %d, r = %d, RAND_MAX = %d\n", s, r, RAND_MAX);

    int a;
    for (int i = 1; i < iters; i++){
        a = random() % ((*n) - 4);
        a += 2;
        int y = 0;
        int x = 0;
        pow_mod(&y, a, r, *n);
        //printf("%d to the power of %d mod %d == %d\n", a, r, *n, y);
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
*/
/*
void make_prime(int *p, int bits, int iters){//mpz_t p, uint64_t bits, uint64_t iters){
    
}
*/

int main(void){
    mpz_t g;
    mpz_t i;
    mpz_t out;
    mpz_t n;

    mpz_t a;
    mpz_t b;

    mpz_inits(g, i, out, n, a, b, NULL);

    mpz_set_ui(a, 345756848);
    mpz_set_ui(b, 76784448);
    //int g = 0;
    //int i = 0;
    //int out = 0;
    //int n = 521;
    randstate_init(1);
    gcd(g, a, b);
    gmp_printf("the gcd is %Zd\n", g);

    mpz_set_ui(a, 9);
    mpz_set_ui(b, 50);
    mod_inverse(i, a, b);
    gmp_printf("the mod_inverse is %Zd\n", i);

    mpz_set_ui(a, 55);
    mpz_set_ui(b, 67);
    mpz_set_ui(n, 84);
    pow_mod(out, a, b, n);
    gmp_printf("the pow_mod is %Zd\n", out);
    //printf(is_prime(&n, 40) ? "true\n" : "false\n");
    return 0;
}

