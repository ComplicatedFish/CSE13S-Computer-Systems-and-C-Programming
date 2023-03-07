#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"
#include "word.h"
#include <string.h>
#include <assert.h>

//extern uint64_t total_syms; // To count the symbols processed.
//extern uint64_t total_bits; // To count the bits processed.

//typedef struct FileHeader {
//  uint32_t magic;
//  uint16_t protection;
//} FileHeader;

//near the end of the file, read might not read all the bytes
//you request. Read will try to read as many bytes as possible now
//rather than later
//
int read_bytes(int infile, uint8_t *buf, int to_read);

int write_bytes(int outfile, uint8_t *buf, int to_write);

void read_header(int infile, FileHeader *header);

void write_header(int outfile, FileHeader *header);

bool read_sym(int infile, uint8_t *sym);

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen);

void flush_pairs(int outfile);

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen);

void write_word(int outfile, Word *w);

void flush_words(int outfile);


