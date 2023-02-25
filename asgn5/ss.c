#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <assert.h>

void lcm(mpz_t rop, const mpz_t a, const mpz_t b){
    mpz_t num;  //holds numerator in LCM formula
    mpz_t den;  //denominator in LCM fomrula
    mpz_inits(num, den, NULL);
    mpz_mul(num, a, b); //num = a * b. abs() uneeded as we are working with unsigned ints
    gcd(den, a, b); //den = gcd(a, b)
    mpz_fdiv_q(rop, num, den); //rop = num/den = (a*b)/gcd(a, b)
    mpz_clears(num, den, NULL);

}

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters){
    uint64_t low = nbits/5;
    uint64_t rand_bits = random() % low;    //generates a random num in range [0, nbits/5)
    rand_bits += low;   //new range is [nbits/5, 2nbits/5)
    uint64_t pbits = rand_bits;
    uint64_t qbits = nbits - (pbits * 2);
    printf("bits for p: %lu bits for q = %lu\n", pbits * 2, qbits);
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_mul(n, p, p);   //n = p^2
    mpz_mul(n, n, q);   //n = n * q = p^2 * q

    //the folowing tests wether the output satisfies requirements
    mpz_t temp_p; //will hold p-1
    mpz_t temp_q; //will hold q-1
    mpz_inits(temp_p, temp_q, NULL);
    mpz_sub_ui(temp_p, p, 1);
    mpz_sub_ui(temp_q, q, 1);
    assert(!mpz_divisible_p(temp_q, p)); //checks that p does NOT divide q-1
    assert(!mpz_divisible_p(temp_p, q)); //checks that q does NOT divide p-1
    mpz_clears(temp_p, temp_q, NULL);
}


void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q){
    mpz_t temp_p; //will hold p - 1
    mpz_t temp_q; //will hold q - 1
    mpz_t lcm_pq; //will hold lcm(p-1, q-1)
    mpz_t n;      //holds public key within this function
    mpz_inits(temp_p, temp_q, lcm_pq, n, NULL);

    //computation of lambda(pq) = lcm(p-1, q-1)
    mpz_mul(pq, p, q);              //pq = p * q
    mpz_sub_ui(temp_p, p, 1);       //temp_p = p - 1
    mpz_sub_ui(temp_q, q, 1);       //temp_q = q - 1
    lcm(lcm_pq, temp_p, temp_q);    //lcm_pq = lcm(temp_p, temp_q) = lcm(p-1, q-1)

    //computation of public key n
    mpz_mul(n, p, p);   //n = p * p
    mpz_mul(n, n, q);   //n = n * q = p * p * q

    mod_inverse(d, n, lcm_pq);
    mpz_clears(temp_p, temp_q, lcm_pq, n, NULL);

}


void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile){
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile){
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}


void ss_read_pub(mpz_t n, char username[], FILE *pbfile){
    gmp_fscanf(pbfile, "%Qd\n%s\n", n, username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile){
    gmp_fscanf(pvfile, "%Qd\n%Qd\n", pq, d);
}

/*
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n);

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n);

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq);

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq);
*/


int main(void){
    mpz_t p;
    mpz_t q;
    mpz_t n;
    mpz_t d;
    mpz_t pq;
    mpz_t f;
    mpz_inits(p, q, n, d, pq, f, NULL);
    randstate_init(5);

    ss_make_pub(p, q, n, 128, 40);
    gmp_printf("prime p = %Zd, prime q = %Zd, modulus n = %Zd\n", p, q, n);

    ss_make_priv(d, pq, p, q);
    gmp_printf("prime product pq = %Zd, modulus d = %Zd\n", pq, d);

    char *user = getenv("USER");
    printf("\n");
    ss_write_pub(n, user, stdout);
    printf("\n");
    ss_write_priv(pq, d, stdout);

    return 0;
}

/*
    mpz_inits(g, i, out, n, a, b, NULL);

    mpz_set_ui(a, 345756848);
    mpz_set_ui(b, 76784448);
    randstate_init(5);
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
    mpz_set_ui(n, 1436453715);
    printf(is_prime(n, 40) ? "true\n" : "false\n");
    make_prime(n, 32, 40);
    gmp_printf("%Zd\n", n);
    return 0;
}*/

