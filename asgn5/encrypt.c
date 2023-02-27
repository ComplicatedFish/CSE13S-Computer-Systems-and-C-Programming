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

#define OPTIONS "i:o:n:vh"

extern gmp_randstate_t state;

/* NOTE TO GRADERS: I copied and edited my code from last quarter's
 * cse13s class for my main() files (encrypt.c, decrypt.c, and keygen.c)
 * I did not look at my code for other functions/files (my code largely
 * did not work last year + this year is SS encryption, not RSA).
 */

void print_help(void);

int main(int argc, char **argv) {
    FILE *input = stdin;
    FILE *output = stdout;
    FILE *pbkey = fopen("ss.pub", "r");
    int v = 0; //verbose output enabler
    int h = 0; //help messafe print check

    int opt = 0; //required for getopt

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': input = fopen(optarg, "r"); break;
        case 'o': output = fopen(optarg, "w"); break;
        case 'n':
            fclose(pbkey);
            pbkey = fopen(optarg, "r");
            break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        default: h = 1; break;
        }
    }
    if (h) {
        print_help();
        return 2;
    }

    mpz_t n; //public modulo n
    mpz_init(n);

    char user[256]; //maximum username size in linux

    ss_read_pub(n, user, pbkey);

    //verbose output here
    if (v) {
        printf("user = %s\n", user);
        gmp_printf("n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    ss_encrypt_file(input, output, n);

    mpz_clear(n);
    fclose(input);
    fclose(output);
    fclose(pbkey);
    return 0;
}

//helper function to print program help statement
void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Encrypts file using the Schmidt-Samao algorithm\n"
                    "\n"
                    "USAGE\n"
                    "   ./encrypt [i:o:n:vh]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -i input        Specifies input file to encrypt.\n"
                    "   -o output       Specifies file to place encrypted contents into.\n"
                    "   -n pbfile       Specifies file to read public key from.\n"
                    "   -v              Enables verbose output of statistics.\n"
                    "   -h              Prints help message (this message).\n");
    return;
}
