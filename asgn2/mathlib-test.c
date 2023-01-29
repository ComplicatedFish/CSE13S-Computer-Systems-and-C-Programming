#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mathlib.h"
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

void print_help(void);

int main (int argc, char **argv) {
    int opt = 0;
    int a,e_var,b,m,r,v,n,s,h = 0; //to keep track of arguments, all set 0 by default
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 'a':
                a = 1;
                break;
            case 'e':
                e_var = 1;
                break;
            case 'b':
                b = 1;
                break;
            case 'm':
                m = 1;
                break;
            case 'r':
                r = 1;
                break;
            case 'v':
                v = 1;
                break;
            case 'n':
                n = 1;
                break;
            case 's':
                s = 1;
                break;
            case 'h':
                h = 1;
                break;
            default:
                h = 1;
                break;
        }
    }
    if (h) { //error printing in seperate function to preserve space within main
        print_help();
        return 2; //non_zero return means error
    }
    //Euler's number test
    if (a == 1 || e_var == 1) {
        printf("e() = %16.15f, M_E = %16.15f, diff = %16.15f\n", e(), M_E, e() - M_E);
        if (s == 1) {
            printf("e terms = %d\n", e_terms());
        }
    }
    //Euler's PI series test
    if (a == 1 || r == 1) {
        printf("pi_euler() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", pi_euler(), M_PI, absolute(pi_euler() - M_PI));
        if (s == 1) {
            printf("pi_euler() terms = %d\n", pi_euler_terms());
        }
    }
    // BBP test
    if (a == 1 || b == 1) {
        printf("pi_bbp() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", pi_bbp(), M_PI, pi_bbp() - M_PI);
        if (s == 1) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
        }
    }
    //Madhava test
    if (a == 1 || m == 1) {
        printf("pi_madhava() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", pi_madhava(), M_PI, pi_madhava() - M_PI);
        if (s == 1) {
            printf("pi_madhava() terms = %d\n", pi_madhava_terms());
        }
    }
    //Viete test
    if (a == 1 || v == 1) {
        printf("pi_viete() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", pi_viete(), M_PI, absolute(pi_viete() - M_PI));
        if (s == 1) {
            printf("pi_viete() terms = %d\n", pi_viete_factors());
        }
    }
    //Newton-Raphson square root test
    if (a == 1 || n == 1) {
        for (double d = 0.0; d <= 10.0; d+=0.1){
            printf("sqrt_newton(%f) = %16.15f, M_E = %16.15f, diff = %16.15f\n", d,  sqrt_newton(d), sqrt(d), absolute(sqrt_newton(d) - sqrt(d)));
            if (s == 1) {
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
            }
        }
    }
    return 0;
}

void print_help(void){
    fprintf(stderr,
            "SYNOPSIS\n"
            "   A testing program to test approximations of PI using various taylor series.\n"
            "   Also includes approximations of Euler's number (e) and square root\n"
            "\n"
            "USAGE\n"
            "   ./mathlib-test [-aebmrvnsh]\n"
            "\n"
            "OPTIONS\n"
            "   -a  Runs all tests.\n"
            "   -e  Runs Euler's number (e) test.\n"
            "   -b  Runs Bailer-Bourwein-Plouffe formula pi test.\n"
            "   -m  Runs Madhava series pi test.\n"
            "   -r  Runs Euler series pi test.\n"
            "   -v  Runs Viete series pi test.\n"
            "   -n  Runs Newton-Raphson's square root test.\n"
            "   -s  Prints statistics on number of iterations for each test.\n"
            "   -h  Prints help message (this message).\n"
            );
    return;
}
