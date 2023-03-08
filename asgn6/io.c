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

//MAGIC and BLOCK are defined

//extern uint64_t total_syms; // To count the symbols processed.
//extern uint64_t total_bits; // To count the bits processed.

extern uint8_t *buffer;
//typedef struct FileHeader {
//  uint32_t magic;
//  uint16_t protection;
//} FileHeader;

//this functions and the following function write_bytes() read and write
//bytes from and to a file. They use the functions read and write, both of
//which are used similarly to read from a file into a buffer and write to a
//file from a buffer.
int read_bytes(int infile, uint8_t *buf, int to_read){
    int r;
    int sum = 0;
    while (sum < to_read){
        //sum is equal to bytes read, so sum is subtracted from to_read
        //to obtain the number of bytes left to read
        r = read(infile, buf, to_read - sum);
        sum += r;
        if (!r){//exits loop early if r = 0
            break;
        }
    }
    return sum;
}

int write_bytes(int outfile, uint8_t *buf, int to_write){
    int w;
    int sum = 0;
    while (sum < to_write){
        w = write(outfile, buf, to_write - sum);
        sum += w;
        if (!w){
            break;
        }
    }
}

void read_header(int infile, FileHeader *header){
    read_bytes(infile, header, sizeof(FileHeader));
    if (big_endian()){
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
}

void write_header(int outfile, FileHeader *header){
    if (big_endian()){
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    write_bytes(outfile, header, sizeof(FileHeader));
}

bool read_sym(int infile, uint8_t *sym){
    int r;
    static int byte_index;
    byte_index = byte_index % BLOCK; //resets index to 0 once 4096 hit
    if (byte_index == 0){ //if index 0, then new buffer required to read
        r = read_bytes(infile, buffer, BLOCK);
        if (r == 0){ //only executes if r = 0 AND there were no previous reads
            return false; //returns false if no more symbols to be read
        }
    }
    *sym = buffer[byte_index];
    //deal with the fact that r will not always go up to 4096
    //you may need to declare r as static var, and then compare


}

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen){
    
}

void flush_pairs(int outfile){

}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen){

}

void write_word(int outfile, Word *w){

}

void flush_words(int outfile){

}

int main (void){

    return 0;
}

