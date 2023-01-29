#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mathlib.h"

#define OPTIONS "aebmrvnsh"

int main (int argc, char **argv) {
    printf("e() = %16.15f, M_E = %16.15f, diff = %16.15f\n", e(), M_E, e() - M_E);
    printf("e terms = %d\n", e_terms());
    return 0;
}
