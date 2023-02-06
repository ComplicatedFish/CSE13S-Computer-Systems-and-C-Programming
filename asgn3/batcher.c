#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y]) > 0) {
        swap(stats, &A[x], &A[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0) {
        return;
    }

    //this for loop loops over every bit of n
    //to find the most significant bit and return
    //its index, which corresponds with bit length
    uint32_t t = 0; //will hold bit length
    for (uint32_t i = 0; i < sizeof(uint32_t) * 8; i++) {
        if ((n >> i) & 1) {
            t = i + 1; //length doesn't start at 0, adding 1 to account for that
        } //this conditional returns true if bit index 0 if set to 1
        //after being shifted to the right i times.
    }
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d > 0) {
            for (uint32_t i = 0; i < n - d; i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q = q >> 1;
            r = p;
        }
        p = p >> 1;
    }
}
