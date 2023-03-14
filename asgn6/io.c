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

uint64_t total_syms; // To count the symbols processed. used for flush_words
uint64_t total_bits; // To count the bits processed.

uint8_t read_buffer[BLOCK]; //buffer for read_pair
uint8_t write_buffer[BLOCK]; //buffer for write_pair
uint8_t word_buffer[BLOCK]; //buffer for write_word

//
//this functions and the following function write_bytes() read and write
//bytes from and to a file. They use the functions read and write, both of
//which are used similarly to read from a file into a buffer and write to a
//file from a buffer.
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int r;
    int sum = 0;
    while (sum < to_read) {
        //sum is equal to bytes read, so sum is subtracted from to_read
        //to obtain the number of bytes left to read
        r = read(infile, buf, to_read - sum);
        sum += r;
        if (!r) { //exits loop early if r = 0
            break;
        }
    }
    return sum;
}

//this function writes bytes out to the outfile
//it repeats calls to write() until the specified number
//of bytes is read
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int w;
    int sum = 0;
    while (sum < to_write) {
        w = write(outfile, buf, to_write - sum);
        sum += w;
        if (!w) {
            break;
        }
    }
    return sum;
}

//reads header from infile directly to allocated header struct
void read_header(int infile, FileHeader *header) {
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
}

//writes header directly to outfile from header struct
void write_header(int outfile, FileHeader *header) {
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//read's symbol into buffer, and the reads from buffer into *sym
//uses static buffer
bool read_sym(int infile, uint8_t *sym) {
    static int r; //holds the value returned by read_bytes()
    static int byte_index;
    static uint8_t buffer[BLOCK];
    byte_index = byte_index % BLOCK; //resets index to 0 once 4096 hit
    if (byte_index == 0) { //if index 0, then new buffer required to read
        r = read_bytes(infile, buffer, BLOCK);
        if (r == 0) { //only executes if r = 0 AND there were no previous reads
            return false; //returns false if no more symbols to be read
        }
    }
    if (byte_index < r) {
        *sym = buffer[byte_index];
        byte_index++;
        total_syms++;
        return true;
    } else {
        return false;
    }
}

//global variable for use with write_pair
int write_bit_index;

//writes pairs of bitlen bits of code and the accompanying symbol
//uses global buffer write_buffer
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    if (write_bit_index / 8 == BLOCK) { //buffer full!
        flush_pairs(outfile);
    }
    for (int i = 0; i < bitlen; i++) {
        //this and the accompanying line for writing sym below are very complex. To break it down
        //first code is shifted to the right i times to check if code actually has a bit at that
        //location. If it does not, a harmless logical OR with 0's is performed, doing nothing. If
        //there does exist a bit at index i, then the value of ((code >> i) & 1) = 1, which gets
        //shifted to the left the required number of times as provided by write_bit_index%8. This ensures
        //that the code (and sym below) is written into the buffer correctly
        write_buffer[write_bit_index / 8]
            = write_buffer[write_bit_index / 8] | (((code >> i) & 1) << (write_bit_index % 8));
        write_bit_index++;
        if (write_bit_index / 8 == BLOCK) { //buffer full!
            flush_pairs(outfile);
        }
    }
    for (int i = 0; i < 8; i++) {
        //see above for explanation of this line below
        write_buffer[write_bit_index / 8]
            = write_buffer[write_bit_index / 8] | (((sym >> i) & 1) << (write_bit_index % 8));
        write_bit_index++;
        if (write_bit_index / 8 == BLOCK) { //buffer full!
            flush_pairs(outfile);
        }
    }
}

//flushes write_pair()'s buffer
//writing everythign stored within
//the global buffer write_buffer
void flush_pairs(int outfile) {
    total_bits += write_bit_index;
    if (write_bit_index / 8 == BLOCK) {
        write_bytes(outfile, write_buffer, BLOCK);
    } else {
        write_bytes(outfile, write_buffer, (total_bits + 8 - 1) / 8);
    }
    for (int i = 0; i < BLOCK; i++) {
        write_buffer[i] = 0;
    }
    write_bit_index = 0;
}

//global variable for use with read_pair
int read_bit_index;

//This function read's pair from infile into the global buffer read_buffer
//once the buffer is filled, pairs are gathered from the buffer into
// *code and *sym
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    static int r; //holds result of read_bytes
    static int check; //ensures initialization is only done once in program
    *code = 0;
    *sym = 0;
    if (read_bit_index == 0 && check != 1) { //buffer empty, needs to be filled
        r = read_bytes(infile, read_buffer, BLOCK);
        check = 1; //ensures this initialization never entered again
    }
    if (!r) {
        return false;
    }

    for (int i = 0; i < bitlen; i++) {
        //this is another complex bitwise operation set, mirroring that of write_pair. first, read_buffer
        //at the current byte (obtained by read_bit_index/8) is shifted to the right by the current bit
        //specified by (read_bit_index % 8). This is AND'ed with a 1 to ensure there exists a bit at that
        //location. That bit is then shifted to the right i times (i is from for loop) into the correct
        //position in *code
        *code = *code | ((((read_buffer[read_bit_index / 8] >> (read_bit_index % 8))) & 1) << i);

        read_bit_index++;
        total_bits++;
        if (read_bit_index / 8 == BLOCK) {
            read_bit_index = 0;
            r = read_bytes(infile, read_buffer, BLOCK);
        }
    }
    if (*code == STOP_CODE) {
        total_bits
            += 8; //we can exist program early if *code is STOP_CODE, but we have to account for the pair of code STOP_CODE
            //totalbits incremented 8 bits here to account for STOP_CODE's pair
        return false;
    }
    for (int i = 0; i < 8; i++) {
        //see code copying loop above
        *sym = *sym | ((((read_buffer[read_bit_index / 8] >> (read_bit_index % 8))) & 1) << i);

        read_bit_index++;
        total_bits++;
        if (read_bit_index / 8 == BLOCK) {
            read_bit_index = 0;
            r = read_bytes(infile, read_buffer, BLOCK);
        }
    }
    return true;
}

int word_byte_index;

//writes decoded words to outfile. used in
//decompression
void write_word(int outfile, Word *w) {
    for (uint32_t i = 0; i < w->len; i++) {
        word_buffer[word_byte_index] = w->syms[i];
        word_byte_index++;
        total_syms++;
        if (word_byte_index == BLOCK) {
            flush_words(outfile);
            word_byte_index = 0;
        }
    }
}

//writes out symbols in word_buffer left
void flush_words(int outfile) {
    if (total_syms % BLOCK == 0) {
        write_bytes(outfile, word_buffer, BLOCK);
    } else {
        write_bytes(outfile, word_buffer, word_byte_index);
    }
    for (int i = 0; i < BLOCK; i++) {
        word_buffer[i] = 0;
    }
}
