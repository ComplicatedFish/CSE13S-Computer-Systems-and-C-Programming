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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vi:o:h"

void print_help(void);

//gets the bit length of a uint16_t, which in this case
//will be the code. iterates over each bit in the input,
//checking if the bit exists there with AND'ing it with
//a 1 shifted to that bit. If the result is true, the input
//is *at least* that much bits, and the index is set to i + 1
//(since length is 1 indexed)
int bit_len(uint16_t num) {
    int index = 0;
    for (uint16_t i = 0; i < 16; i++) {
        if (num & (1 << i)) {
            index = i + 1;
        }
    }
    return index;
}

int main(int argc, char **argv) {
    int infile = 0;
    int outfile = 1;
    int v = 0; //enables compression statistics
    int h = 0; //help messafe print check

    int opt = 0; //required for getopt

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = open(optarg, O_RDONLY | O_CREAT); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        default: h = 1; break;
        }
    }
    if (h) {
        print_help();
        return 2;
    }

    //printf("the bit length of 110 is %u\n", bit_len(6));

    FileHeader *header = (FileHeader *) calloc(1, sizeof(FileHeader));
    read_header(infile, header);
    if (header->magic != MAGIC) {
        printf("Wrong Magic Number!\n");
        return 2;
    }

    WordTable *table = wt_create();

    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;
    while (read_pair(infile, &curr_code, &curr_sym, bit_len(next_code))) {
        //printf("the current sym is %"PRIu8" and the code is %u\n", curr_sym, curr_code);
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code += 1;
        if (next_code == MAX_CODE) {
            //printf("if statement\n");
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(outfile);

    //verbose output here
    if (v) {
        int total_bytes = (total_bits + 8 - 1)/8 + (int)sizeof(FileHeader); //ceiling division to obtain total bytes in file
        //fileheader size also needs to be added to compressed file size
        printf("Compressed file size: %d bytes\n", total_bytes);
        printf("Uncompressed file size: %lu bytes\n", total_syms);
        printf("Compression ratio: %.2f%%\n", (1 - ((double)total_bytes/(double)total_syms))*100); 
    }

    wt_delete(table);
    close(infile);
    close(outfile);
    free(header);
    return 0;
}

//helper function to print program help statement
void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Decompresses file using the LZ78 algorithm\n"
                    "\n"
                    "USAGE\n"
                    "   ./encrypt [vi:o:h]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -i input        Specifies input file to decompress.\n"
                    "   -o output       Specifies file to place decompressed contents into.\n"
                    "   -v              Enables verbose output of statistics.\n"
                    "   -h              Prints help message (this message).\n");
    return;
}
