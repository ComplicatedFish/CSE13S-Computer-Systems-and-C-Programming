#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t partition(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {
    uint32_t i = low - 1;
    for (uint32_t j = low; j < high; j++) {
        if (cmp(stats, A[j - 1], A[high - 1]) < 0) {
            i++;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[high - 1]);
    return i + 1;
}

void quick_sorter(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {
    if (low < high) {
        uint32_t p = partition(stats, A, low, high);
        //printf("partition p = %u, low = %u, high = %u\n", p, low, high);
        quick_sorter(stats, A, low, p - 1);
        //quick_sorter(stats, A, p + 1, high);
        //for(uint32_t i = 0; i < 15; i++) {
        //    printf("%u\n", A[i]);
        //}
        quick_sorter(stats, A, p + 1, high);
    }
}
//function to be called. Most of the logic of quick sort is in the
//other functions because of the need for more arguments
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}

/*
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
}*/
