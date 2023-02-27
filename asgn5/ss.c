#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <assert.h>

extern gmp_randstate_t state;

//computation of the least common multiple of a and b
//result stored in rop
void lcm(mpz_t rop, const mpz_t a, const mpz_t b) {
    mpz_t num; //holds numerator in LCM formula
    mpz_t den; //denominator in LCM fomrula
    mpz_inits(num, den, NULL);
    mpz_mul(num, a, b); //num = a * b. abs() uneeded as we are working with unsigned ints
    gcd(den, a, b); //den = gcd(a, b)
    mpz_fdiv_q(rop, num, den); //rop = num/den = (a*b)/gcd(a, b)
    mpz_clears(num, den, NULL);
}

//production of SS public key
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    uint64_t low = nbits / 5;
    uint64_t rand_bits = random() % low; //generates a random num in range [0, nbits/5)
    rand_bits += low; //new range is [nbits/5, 2nbits/5)
    uint64_t pbits = rand_bits;
    uint64_t qbits = nbits - (pbits * 2);
    printf("bits for p: %lu bits for q = %lu\n", pbits * 2, qbits);
    make_prime(p, pbits, iters);
    make_prime(q, qbits, iters);
    mpz_mul(n, p, p); //n = p^2
    mpz_mul(n, n, q); //n = n * q = p^2 * q

    //the folowing tests whether the output satisfies requirements
    mpz_t temp_p; //will hold p-1
    mpz_t temp_q; //will hold q-1
    mpz_inits(temp_p, temp_q, NULL);
    mpz_sub_ui(temp_p, p, 1);
    mpz_sub_ui(temp_q, q, 1);
    assert(!mpz_divisible_p(temp_q, p)); //checks that p does NOT divide q-1
    assert(!mpz_divisible_p(temp_p, q)); //checks that q does NOT divide p-1
    mpz_clears(temp_p, temp_q, NULL);
}

//produces SS private key
void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t temp_p; //will hold p - 1
    mpz_t temp_q; //will hold q - 1
    mpz_t lcm_pq; //will hold lcm(p-1, q-1)
    mpz_t n; //holds public key within this function
    mpz_inits(temp_p, temp_q, lcm_pq, n, NULL);

    //computation of lambda(pq) = lcm(p-1, q-1)
    mpz_mul(pq, p, q); //pq = p * q
    mpz_sub_ui(temp_p, p, 1); //temp_p = p - 1
    mpz_sub_ui(temp_q, q, 1); //temp_q = q - 1
    lcm(lcm_pq, temp_p, temp_q); //lcm_pq = lcm(temp_p, temp_q) = lcm(p-1, q-1)

    //computation of public key n
    mpz_mul(n, p, p); //n = p * p
    mpz_mul(n, n, q); //n = n * q = p * p * q

    mod_inverse(d, n, lcm_pq);
    mpz_clears(temp_p, temp_q, lcm_pq, n, NULL);
}

//writes SS public key to file provided by pvfile
void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

//writes SS private key to file provided by pvfile
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

//reads SS public key from pvfile, stores key data in vars n and username
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%s\n", n, username);
}

//reads SS private key from pvfile, stores key data in vars pq and d
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

//encrypts message m into ciphertext c
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

//encrypts files with ss_encrypt. In order to keep message length
//less than n, encryption is done in blocks of text
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t k; //holds size of block
    mpz_t m; //holds imported message
    mpz_t c; //holds encrypted message
    mpz_inits(k, m, c, NULL);

    //this is the calculation of block size, which will be stores in K
    mpz_set_ui(k, mpz_sizeinbase(n, 2)); //k = log_2(n)
    mpz_fdiv_q_ui(k, k, 2); //k = k/2 = log_2(n)/2
    mpz_sub_ui(k, k, 1); //k = k-1 = (1/2)log_2(n)-1
    mpz_fdiv_q_ui(k, k, 8); //k = k/8 = [(1/2)log_2(n)-1]/8
    uint64_t block_size = mpz_get_ui(k);

    uint8_t *buffer = (uint8_t *) calloc(block_size, sizeof(uint8_t));
    uint8_t *init;
    init = buffer; //necessary to free buffer
    uint64_t j = 0; //will hold number of bytes read
    while (!feof(infile)) {
        buffer[0] = 0xff;
        j = fread(buffer + 1, sizeof(uint8_t), block_size - 1, infile);
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, buffer);
        ss_encrypt(c, m, n);
        gmp_fprintf(outfile, "%Zx\n", c);
        buffer = init; //resets buffer
    }
    free(init);
    mpz_clears(k, m, c, NULL);
}

//decrypts ciphertext c, stores output in m
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

//decrypts file using ss_decrypt. Reads ciphertext in blocks in order to keep
//message length less than n. This function basically reverses ss_encrypt_file
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t k; //holds size of block
    mpz_t m; //holds imported message
    mpz_t c; //holds encrypted message
    mpz_inits(k, m, c, NULL);

    //this is the calculation of block size, which will be stores in K
    mpz_set_ui(k, mpz_sizeinbase(pq, 2)); //k = log_2(pq)
    mpz_sub_ui(k, k, 1); //k = k-1 = (1/2)log_2(pq)-1
    mpz_fdiv_q_ui(k, k, 8); //k = k/8 = [(1/2)log_2(pq)-1]/8
    uint64_t block_size = mpz_get_ui(k);

    uint8_t *buffer = (uint8_t *) calloc(block_size, sizeof(uint8_t));
    uint8_t *init;
    init = buffer;
    uint64_t j = 0; //will hold number of bytes converted
    while (!feof(infile)) {
        gmp_fscanf(infile, "%Zx\n", c);
        ss_decrypt(m, c, d, pq);
        mpz_export(buffer, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(buffer + 1, sizeof(uint8_t), j - 1, outfile);
    }
    free(init);
    mpz_clears(k, m, c, NULL);
}
