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
//algorithm to calculate gcd. See bottom of function for
//pure C implementation of gcd
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

//pure C implementation of gcd below
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
    mpz_clears(r, r2, t, t2, q, temp_t, temp_r, temp, NULL);
    return;
}

//computer the power mod; that is, computes o = a^d mod n
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n){   //int *o, int a, int d, int n
    mpz_t v;
    mpz_t p;
    mpz_t temp_d;
    mpz_inits(v, p, temp_d, NULL);

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
    mpz_clears(v, p, temp_d, NULL);
    return;
}

//Miller-Rabin primality tester
bool is_prime(const mpz_t n, uint64_t iters){ //(int *n, int iters){

    //integers lesser than 4 have to be handled as special
    //cases because of how the mathmatics of this function
    //work (e.g. subtraction is performed on n).
    if (mpz_cmp_ui(n, 4) < 0){
        //0 and 1 are not primes
        //2 and 3 are primes
        //thus we only need to check if n is less than 2
        if (mpz_cmp_ui(n, 2) < 0){
            return false;
        } else {
            return true;
        }
    }
    mpz_t r;
    mpz_t s;
    mpz_t a;
    mpz_t y;
    mpz_t x;
    mpz_t j;
    mpz_t two; //holds 2 for use with pow_mod

    mpz_t temp_n; //these temp variables temporarily hold values
    mpz_t temp_s; //for use within main loop
    mpz_inits(r, s, a, y, x, temp_n, temp_s, j, two, NULL);

    mpz_sub_ui(r, n, 1);        //r holds (n-1)
    mpz_fdiv_q_ui(r, r, 2);     //int r = (n-1)/2;
    mpz_set_ui(s, 1);           //int s = 1;
    while (mpz_even_p(r)){      //while (r % 2 == 0){ //checks if r is even
        mpz_fdiv_q_ui(r, r, 2); //r /= 2;
        mpz_add_ui(s, s, 1);       //s++;
    }

    //main for loop for the miller rabin primality check
    for (uint32_t i = 1; i < iters; i++){
        //sub by 3 here because urandomm finds a random number
        //between 0 and n-1, and we have to search in the range
        //2 to n-2. Since we add the the 2 later (after generation)
        //to prevent sub-2 numbers, we have to search in the range
        //0 to n-4. Since urandomm already searched n-1, we add
        //1 to the upper bound.
        mpz_sub_ui(temp_n, n, 3);

        mpz_urandomm(a, state, temp_n); //a = random() % ((*n) - 4);
        mpz_add_ui(a, a, 2);            //a += 2;
        mpz_set_ui(y, 0);               //int y = 0;
        mpz_set_ui(x, 0);               //int x = 0;
        pow_mod(y, a, r, n);

        mpz_sub_ui(temp_n, n, 1);   //temp_n holds (n-1)
        mpz_sub_ui(temp_s, s, 1);   //temp_s holds (s-1)
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, temp_n) != 0){  //if (y != 1 && y!= *n-1)
            mpz_set_ui(j, 0);               //int j = 0;
            while (mpz_cmp(j, temp_s) <= 0 && mpz_cmp(y, temp_n) != 0){ //while (j <= s-1 && y != (*n)-1)
                mpz_set(x, y);              //x = y;
                mpz_set_ui(two, 2);
                pow_mod(y, x, two, n);
                if (mpz_cmp_ui(y, 1) == 0){ //if (y == 1)
                    mpz_clears(r, s, a, y, x, temp_n, temp_s, j, two, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);        //j += 1;
            }
            if (mpz_cmp(y, temp_n) != 0){   //if (y != (*n)-1)
                mpz_clears(r, s, a, y, x, temp_n, temp_s, j, two, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, s, a, y, x, temp_n, temp_s, j, two, NULL);
    return true;
}

//randomly generates numbers and checks them with is_prime until
//pair of primes are generated
void make_prime(mpz_t p, uint64_t bits, uint64_t iters){ //(int *p, int bits, int iters){
    //for explanation of this see while loop below, this is a copy of the inside
    //for rare cases where the first generated number is a prime
    mpz_urandomb(p, state, bits);
    mpz_setbit(p, 0);
    mpz_setbit(p, bits-1);

    while (is_prime(p, iters) != true){
        mpz_urandomb(p, state, bits);
        mpz_setbit(p, 0); //sets bit index 0 (first bit) which ensures p is not even
        mpz_setbit(p, bits-1); //sets bit at index "bits" to 1 to ensure p is at least "bits" long

    }
}
/*
int m55ain(void){
    mpz_t p;
    mpz_init(p);
    randstate_init(0);
    mpz_set_ui(p, 1);
    printf(is_prime(p, 50) ? "true\n" : "false\n");
    mpz_clear(p);
    randstate_clear();
}*/

