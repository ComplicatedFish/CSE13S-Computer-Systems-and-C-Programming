#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "ahbsqr:n:p:H"
/*
 * NOTE FOR GRADERS: I used my sorting.c code from last quarter's CSE13s
 * as a reference for making this main() file. I directly copied the
 * print_elements() and reset_array() functions from my code from last year
 * since I made them on my own as helper functions (They were not required)
 */

//allows for clear and readable usage of sets in main()
enum options { shell, quick, heap, batcher, help };

//helper function to print help message. Below main to avoid clutter
void print_help(void);

//NOTE: THIS IS A DIRECTLY COPIED (with slight edits to make it work on this assignment)
//FUNCTION from assignment 4 sorting.c from last year. This is a helper function that just
//prints the sorts according to specification. Function body is below main() to avoid clutter
void print_elements(uint32_t *arr, uint32_t size, uint32_t elements);

//NOTE: This is another function taken from my own code from last year.
//This is a helper function that resets the array back to the original
//values so that the same numbers can be tested on each sort
void reset_array(uint32_t *arr, uint32_t *orig_arr, uint32_t n) {
    for (uint32_t x = 0; x < n; x++) {
        arr[x] = orig_arr[x];
    }
}

int main(int argc, char **argv) {
    //below declarations are setting default values for options
    int opt = 0; //required for getopt()
    uint32_t seed = 13371453;
    uint32_t size = 100;
    uint32_t elements = 100;

    Set s = set_empty();
    Stats stats;
    reset(&stats);

    s = set_insert(s, help); //defaults to requiring help
        //removed if sort specified

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            s = set_remove(s, help);
            s = set_insert(s, shell);
            s = set_insert(s, heap);
            s = set_insert(s, batcher);
            s = set_insert(s, quick);
            break;
        case 'h':
            s = set_remove(s, help);
            s = set_insert(s, heap);
            break;
        case 'b':
            s = set_remove(s, help);
            s = set_insert(s, batcher);
            break;
        case 's':
            s = set_remove(s, help);
            s = set_insert(s, shell);
            break;
        case 'q':
            s = set_remove(s, help);
            s = set_insert(s, quick);
            break;
        case 'r':
            //s = set_insert(s, seed); unneccesary to track since these just change sort parameters
            //and do not actually tigger specific sorts
            seed = strtoul(optarg, NULL, 10);
            break;
        case 'n':
            //s = set_insert(s, size);
            size = strtoul(optarg, NULL, 10);
            break;
        case 'p':
            //s = set_insert(s, elements);
            elements = strtoul(optarg, NULL, 10);
            break;
        case 'H': s = set_insert(s, help); break;
        default: s = set_insert(s, help); break;
        }
    }

    srandom(seed);
    uint32_t *arr;
    uint32_t *orig_arr; //original array to reset arr
    arr = (uint32_t *) malloc(size * sizeof(uint32_t));
    orig_arr = (uint32_t *) malloc(size * sizeof(uint32_t));

    //populates array with random numbers
    for (uint32_t x = 0; x < size; x++) {
        arr[x] = random() & 0x3FFFFFFF; //0x3FFFFFFF is 30 bits long
    }

    for (uint32_t x = 0; x < size; x++) {
        orig_arr[x] = arr[x];
    }

    if (set_member(s, help)) {
        if (argc == 1) {
            printf("Select at least one sort to perform.\n");
        }
        print_help();
        free(arr);
        free(orig_arr);
        return 2;
    }

    reset(&stats);
    if (set_member(s, shell)) {
        shell_sort(&stats, arr, size);
        printf("Shell Sort, %u elements, %lu moves, %lu compares\n", size, stats.moves,
            stats.compares);
        print_elements(arr, size, elements);
        reset_array(arr, orig_arr, size);
    }

    reset(&stats);
    if (set_member(s, batcher)) {
        batcher_sort(&stats, arr, size);
        printf("Batcher Sort, %u elements, %lu moves, %lu compares\n", size, stats.moves,
            stats.compares);
        print_elements(arr, size, elements);
        reset_array(arr, orig_arr, size);
    }

    reset(&stats);
    if (set_member(s, heap)) {
        heap_sort(&stats, arr, size);
        printf(
            "Heap Sort, %u elements, %lu moves, %lu compares\n", size, stats.moves, stats.compares);
        print_elements(arr, size, elements);
        reset_array(arr, orig_arr, size);
    }

    reset(&stats);
    if (set_member(s, quick)) {
        quick_sort(&stats, arr, size);
        printf("Quick Sort, %u elements, %lu moves, %lu compares\n", size, stats.moves,
            stats.compares);
        print_elements(arr, size, elements);
        reset_array(arr, orig_arr, size);
    }

    free(arr);
    free(orig_arr);

    return 0;
}

//REPOSTED NOTE: THIS IS A DIRECTLY COPIED (with slight edits to make it work on this assignment)
//FUNCTION from assignment 4 sorting.c from last year. This is a helper function that just
//prints the sorts according to specification. Function body is below main() to avoid clutter
void print_elements(uint32_t *arr, uint32_t size, uint32_t elements) {
    uint32_t check; //
    if (elements < size) {
        for (uint32_t i = 0; i < elements; i++) {
            printf("%13" PRIu32, arr[i]);
            check = i;
            if (i % 5 == 4) {
                printf("\n");
            }
        }
        if (check % 5 == 1) {
            return;
        } else {
            printf("");
        }
        return;
    }
    for (uint32_t i = 0; i < size; i++) {
        printf("%13" PRIu32, arr[i]);
        check = i;
        if (i % 5 == 4) {
            printf("\n");
        }
    }
    if (check % 5 == 1) {
        return;
    } else {
        printf("");
    }
}

void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   A small collection of sorting functions. Includes Shell Sort,\n"
                    "   heapsort, quicksort, and batcher (merge) sort\n"
                    "\n"
                    "USAGE\n"
                    "   ./sorting [-ahbsqr:n:p:H]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -a              Runs all tests.\n"
                    "   -h              Runs Heapsort.\n"
                    "   -b              Runs Batcher's Odd-Even Merge Sort.\n"
                    "   -s              Runs Shell Sort.\n"
                    "   -q              Runs Quicksort\n"
                    "   -r seed         Sets the seed for the random number generator that "
                    "populates the array\n"
                    "   -n length       Sets the size of the array to be sorted\n"
                    "   -p elements     Sets the number of elements to be printed for each sort.\n"
                    "   -H              Prints help message (this message).\n");
    return;
}
