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
int bit_len(uint16_t num){
    int index = 0;
    for (uint16_t i = 0; i < 16; i++){
        if (num & (1 << i)){
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
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT); break;
        case 'v': v = 1; break;
        case 'h': h = 1; break;
        default: h = 1; break;
        }
    }
    if (h) {
        print_help();
        return 2;
    }

    //this block handles header
    FileHeader *header = (FileHeader *) calloc(1, sizeof(FileHeader));
    struct stat file_info;
    fstat(infile, &file_info);
    header->magic = 0xBAADBAAC;
    header->protection = file_info.st_mode; //CHANGE THE PROTECTION FOR OUTFILE LATER

    write_header(outfile, header);

    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    TrieNode *next_node;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;
    while (read_sym(infile, &curr_sym)){
        next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL){
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bit_len(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code += 1;
        }
        if (next_code == MAX_CODE){
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
        printf("\n%u\n", next_code);
    }
    if (curr_node != root){
        write_pair(outfile, prev_node->code, prev_sym, bit_len(next_code));
        next_code = (next_code + 1)%MAX_CODE;
    }
    write_pair(outfile, STOP_CODE, 0, bit_len(next_code));
    flush_pairs(outfile);


    //verbose output here
    if (v) {
        
    }

    fchmod(outfile, 0600/*file_info.st_mode*/);

    close(infile);
    close(outfile);
    trie_delete(root);
    free(header);
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


