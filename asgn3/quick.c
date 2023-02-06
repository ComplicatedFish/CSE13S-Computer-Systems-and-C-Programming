#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//function that swaps elements to either before or after the pivot
//The pivot is the point at which quicksort partitions the array.
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

//recursively quicksorts by first partioning the array into 2, then calling
//quick sort again on thoes 2 sub arrays
void quick_sorter(Stats *stats, uint32_t *A, uint32_t low, uint32_t high) {
    if (low < high) {
        uint32_t p = partition(stats, A, low, high); //partition array into < pivot and > pivot
        quick_sorter(stats, A, low, p - 1); //less than pivot subarray
        quick_sorter(stats, A, p + 1, high); //greater than pivot subarray
    }
}
//function to be called. Most of the logic of quick sort is in the
//other functions because of the need for more arguments
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
