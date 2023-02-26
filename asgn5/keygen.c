#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "ss.h"
#include <sys/stat.h>

#define OPTIONS "b:i:n:d:s:vh"

extern gmp_randstate_t state;

/* NOTE TO GRADERS: I copied and edited my code from last quarter's
 * cse13s class for my main() files (encrypt.c, decrypt.c, and keygen.c)
 * I did not look at my code for other functions/files (my code largely
 * did not work last year + this year is SS encryption, not RSA).
 */

void print_help(void);

int main (int argc, char **argv) {
    int opt = 0; //required for getopt()
    uint64_t iters = 50; //iterations needed for miller rabin
    uint64_t nbits = 1024; //specified number of bits for n
    uint64_t seed = time(NULL); //seed for rand state, defaults to seconds since UNIX epoch
    int v = 0; //to check if verbose input will be enabled
    int h = 0; //help message check
    FILE *pub = fopen("ss.pub", "w");
    FILE *priv = fopen("ss.priv", "w");

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 'b':
                bits = strtoul(optarg, NULL, 10);
                break;
            case 'i':
                iters = strtoul(optarg, NULL, 10);
                break;
            case 'n':
                fclose(pub);
                pub = fopen(optarg, "w");
                break;
            case 'd':
                fclose(priv);
                priv = fopen(optarg, "w");
                break;
            case 's':
                seed = strtoul(optarg, NULL, 10);
                break;
            case 'v':
                v = 1;
                break;
            case 'h':
                h = 1;
                break;
            default:
                h = 1;
                break;
        }
    }

    if (h){
        print_help();
        return 2;
    }

    int pv_no = fileno(priv);

    fchmod(pv_no, 0600);

    randstate_init(seed);

    mpz_t p;    //storage for prime 1
    mpz_t q;    //storage for prime 2
    mpz_t n;    //public modulo n
    mpz_t pq;   //product of primes
    mpz_t d;    //private key d

    mpz_inits(p, q, n, pq, d, NULL);

    ss_make_pub(p, q, n, nbits, iters);
    ss_make_priv(d, pq, p, q);

    char *user = getenv("USER"); //username

    ss_write_priv(pq, d, priv);
    ss_write_pub(n, user, pub);

    //format for this taken from verbose output of provided keygen binary
    if (v == 1) {
        fprintf(stdout, "user = %s\n", user);
        gmp_printf("user signature (%lu bits): %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p  (%lu bits) = %Zd\n", mpz_sizeinbase(prime_p, 2), p);
        gmp_printf("q  (%lu bits) = %Zd\n", mpz_sizeinbase(prime_q, 2), q);
        gmp_printf("n  (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq (%lu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    fclose(pub);
    fclose(priv);

    randstate_clear();

    return 0;
}


//helper function to print program help statement
void print_help(void) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Keygen generated a public-private key pair for use with Schmidt-Samoa\n"
        "   cryptography. \n"
        "\n"
        "USAGE\n"
        "   ./keygen [b:i:n:d:s:vh]\n"
        "\n"
        "OPTIONS\n"
        "   -b nbits        Specifies minimum bits for public modulus n.\n"
        "   -i iters        Specifies number of Miller-Rabin iterations to use to test primality.\n"
        "   -n pbfile       Specifies file to write public key to.\n"
        "   -d pvfile       Specifies file to write private key to.\n"
        "   -s seed         Specifies a seed to use with random number functions.\n"
        "   -v              Enables verbose output of statistics.\n"
        "   -h              Prints help message (this message).\n");
    return;
}
