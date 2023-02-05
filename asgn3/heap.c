#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stats.h"
#include <stdbool.h>

//this block obtains the greater value child from the 2 children of
//index "first". These child indices are given by left = 2*first and
//right = 2*first + 1
uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last){
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && cmp(stats, A[right - 1], A[left -1]) == 1 ){
        printf("max_child reached\n");
        return right;
    }
    printf("left child reached\n");
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last){
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);

    while ((mother <= (last / 2)) && found == false){
        if (A[mother - 1] < A[great - 1]){
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, mother, last);
        } else {
            printf("fix_heap reached, found = true\n");
            found = true;
        }
    }
    return;

}

void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = (last / 2); father > first - 1; father--){
        fix_heap(stats, A, father, last);
    }
    return;
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n){
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last);
    for (uint32_t leaf = last; leaf > first; leaf--){
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf-1);
    }
    return;
}

//main for testing purposes
/*
int main(void){
    Stats stat;
    uint32_t length = 15;
    uint32_t arr[] = {3,4,7,2,1,5,6,14,11,13,12,16,20,9,8};
    uint32_t *A;
    A = (uint32_t *) malloc(length * sizeof(uint32_t));

    for (uint32_t i = 0; i < length; i++){
        A[i] = arr[i];
    }

    heap_sort(&stat, A, length);

    for(uint32_t i = 0; i < length; i++) {
        printf("%u\n", A[i]);
    }
    return 0;
}*/
