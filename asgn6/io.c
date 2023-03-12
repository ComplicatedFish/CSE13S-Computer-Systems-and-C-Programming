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

//MAGIC and BLOCK are defined

//extern uint64_t total_syms; // To count the symbols processed.
extern uint64_t total_bits; // To count the bits processed.

/*extern*/ uint8_t read_buffer[BLOCK]; //buffer for read_pair
/*extern*/ uint8_t write_buffer[BLOCK]; //buffer for write_pair
/*extern*/ uint8_t word_buffer[BLOCK]; //buffer for write_word

//typedef struct FileHeader {
//  uint32_t magic;
//  uint16_t protection;
//} FileHeader;

//
//this functions and the following function write_bytes() read and write
//bytes from and to a file. They use the functions read and write, both of
//which are used similarly to read from a file into a buffer and write to a
//file from a buffer.
//
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
    return sum;
}

void read_header(int infile, FileHeader *header){
    read_bytes(infile, (uint8_t *)header, sizeof(FileHeader));
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
    write_bytes(outfile, (uint8_t *)header, sizeof(FileHeader));
}

//read's symbol into buffer, and the reads from buffer into *sym
//uses static buffer
bool read_sym(int infile, uint8_t *sym){
    static int r; //holds the value returned by read_bytes()
    static int byte_index;
    static uint8_t buffer[BLOCK];
    byte_index = byte_index % BLOCK; //resets index to 0 once 4096 hit
    if (byte_index == 0){ //if index 0, then new buffer required to read
        r = read_bytes(infile, buffer, BLOCK);
        if (r == 0){ //only executes if r = 0 AND there were no previous reads
            return false; //returns false if no more symbols to be read
        }
    }
    if (r > 0){
        *sym = buffer[byte_index];
        return true;
    } else {
        return false;
    }

}

//writes pairs of bitlen bits of code and the accompanying symbol
//uses global buffer write_buffer
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen){
    //remember sets.c from asgn 3
    static int bit_index;
    int byte_index = bit_index/8;
    if (byte_index == BLOCK){ //buffer full!
        flush_pairs(outfile);
        byte_index = 0;
        bit_index = 0;
    }
    for (int i = 0; i < bitlen; i++){
        write_buffer[byte_index] = write_buffer[byte_index] | (code & (1 << (bit_index % 8)));
        bit_index++;
        byte_index = bit_index/8;
        if (byte_index == BLOCK){ //buffer full!
            flush_pairs(outfile);
            byte_index = 0;
            bit_index = 0;
        }
    }
    for (int i = 0; i < 8; i++){
        write_buffer[byte_index] = write_buffer[byte_index] | (sym & (1 << (bit_index % 8)));
        bit_index++;
        byte_index = bit_index/8;
        if (byte_index == BLOCK){ //buffer full!
            flush_pairs(outfile);
            byte_index = 0;
            bit_index = 0;
        }
    }
}


//flushes write_pair()'s buffer
//writing everythign stored within
//the global buffer write_buffer
void flush_pairs(int outfile){
    write_bytes(outfile, write_buffer, BLOCK);
}

//This function read's pair from infile into the global buffer read_buffer
//once the buffer is filled, pairs are gathered from the buffer into
//*code and *sym
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen){
    static int bit_index;
    int byte_index = bit_index/8;
    int r; //holds result of read_bytes
    *code = 0;
    *sym = 0;
    if (byte_index == BLOCK){
        bit_index = 0;
        byte_index = 0;
    }
    if (bit_index == 0){ //buffer empty, needs to be filled
        r = read_bytes(infile, read_buffer, BLOCK);
        r++;//just to disable warning
    }
    for (int i = 0; i < bitlen; i++){
        *code = *code | (read_buffer[byte_index] & (1 << (bit_index % 8)));
        bit_index++;
        byte_index = bit_index/8;
        //read_buffer[byte_index] = read_buffer[byte_index] | (code & (1 << (bit_index % 8)));
        if (byte_index == BLOCK){
            bit_index = 0;
            byte_index = 0;
            r = read_bytes(infile, read_buffer, BLOCK);
        }
    }
    if (*code == STOP_CODE){
        return false;
    }
    for (int i = 0; i < 8; i++){
        *sym = *sym | (read_buffer[byte_index] & (1 << (bit_index % 8)));
        bit_index++;
        byte_index = bit_index/8;
        if (byte_index == BLOCK){
            bit_index = 0;
            byte_index = 0;
            r = read_bytes(infile, read_buffer, BLOCK);
        }

    }
    return true;
}

 
void write_word(int outfile, Word *w){
    static int byte_index;
    for (uint32_t i = 0; i < w->len; i++){
        word_buffer[byte_index] = w->syms[i];
        byte_index++;
        if (byte_index == BLOCK){
            flush_words(outfile);
            byte_index = 0;
        }
    }
}

//writes out symbols in word_buffer left
void flush_words(int outfile){
    write_bytes(outfile, word_buffer, BLOCK);
}
/*
int main (void){
    int fd = open("input.txt", O_RDONLY | O_CREAT);
    uint8_t *buffer = (uint8_t *) calloc(1000, sizeof(uint8_t));

    //read_buffer = (uint8_t *) calloc(BLOCK, sizeof(uint8_t));
    //write_buffer = (uint8_t *) calloc(BLOCK, sizeof(uint8_t));

    read_bytes(fd, buffer, 1000);
    write_bytes(1, buffer, 1000);
    close(fd);
    free(buffer);
    
    return 0;
}
*/
