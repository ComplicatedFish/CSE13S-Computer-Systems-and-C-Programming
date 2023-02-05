#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stats.h"
#include <stdbool.h>

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y){
    if (cmp(stats, A[x], A[y]) > 0){
        swap(stats, &A[x], &A[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n){
    if (n == 0){
        return;
    }

    //this for loop loops over every bit of n
    //to find the most significant bit and return
    //its index, which corresponds with bit length
    uint32_t t = 0; //will hold bit length
    for (uint32_t i = 0; i < sizeof(n)*8; i++){
        if ((n >> i) & 1){
            t = i;
        }//this conditional returns true if bit index 0 if set to 1
         //after being shifted to the right i times.
    }
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d > 0){
            for (uint32_t i = 0; i < n-d; i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
        }
    }

}

int main(void){
    Stats stat;
    //reset(&stat);
    uint32_t length = 15;
    uint32_t arr[] = {3,4,7,2,1,5,6,14,11,13,12,16,20,9,8};
    uint32_t *A;
    A = (uint32_t *) malloc(length * sizeof(uint32_t));

    for (uint32_t i = 0; i < length; i++){
        A[i] = arr[i];
    }

    quick_sort(&stat, A, length);

    for(uint32_t i = 0; i < length; i++) {
        printf("%u\n", A[i]);
    }
    free(A);
    return 0;
}

