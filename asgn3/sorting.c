#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stats.h"
#include <stdbool.h>
#include <inttypes.h>
#include "set.h"

#define OPTIONS "ahbsqr:n:p:H"

//allows for clear and readable usage of sets in main()
enum options {shell, quick, heap, batcher, all, seed, size, elements, help};

int main(int argc, char **argv){
    //below declarations are setting default values for options
    int opt = 0; //required for getopt()
    uint32_t seed = 13371453;
    uint32_t size = 100;
    uint32_t elements = 100;

    Set s = set_empty();
    
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch(opt) {
            case 'a':
                s = set_insert(s, all);
            case 'h':
                s = set_insert(s, heap);
            case 'b':
                s = set_insert(s, batcher);
            case 's':
                s = set_insert(s, shell);
            case 'q':
                s = set_insert(s, quick);
            case 'r':
                s = set_insert(s, seed);
            case 'n':
                s = set_insert(s, size);
            case 'p':
                s = set_insert(s, elements);
            case 'H':
                s = set_insert(s, help);
            default:
                s = set_insert(s, help);

        }
    }
}
