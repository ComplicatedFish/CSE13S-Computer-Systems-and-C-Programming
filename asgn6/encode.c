#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"
#include "word.h"
#include <string.h>
#include <assert.h>
#include "io.h"
#include "endian.h"
#include <fcntl.h>
#include <unistd.h>

#define OPTIONS "vi:o:h"

void print_help(void);

int main(int argc, char **argv) {
    int input = 0;
    int output = 1;
    int v = 0; //enables compression statistics
    int h = 0; //help messafe print check

    int opt = 0; //required for getopt

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': input = open(optarg, O_RDONLY | O_CREAT); break;
        case 'o': output = open(optarg, O_WRONLY | O_CREAT); break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        default: h = 1; break;
        }
    }
    if (h) {
        print_help();
        return 2;
    }

    
    //verbose output here
    if (v) {

    }

    return 0;
}

//helper function to print program help statement
void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Compresses file using the LZ78 algorithm\n"
                    "\n"
                    "USAGE\n"
                    "   ./encrypt [vi:o:h]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -i input        Specifies input file to compress.\n"
                    "   -o output       Specifies file to place compressed contents into.\n"
                    "   -v              Enables verbose output of statistics.\n"
                    "   -h              Prints help message (this message).\n");
    return;
}


