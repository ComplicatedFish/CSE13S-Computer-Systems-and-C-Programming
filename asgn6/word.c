#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"

/*
//reference struct
typedef struct Word {
    uint8_t *syms;
    uint32_t len;
} Word;
*/

//typedef Word * WordTable;

Word *word_create(uint8_t *syms, uint32_t len){
    Word *word = (Word *) calloc(1, sizeof(Word));
    word->len = len;
    word->syms = syms;
    if (word){
        return word;
    } else {
        return NULL;
    }
}

Word *word_append_sym(Word *w, uint8_t sym){
    new_syms[w->len + 2];
    Word *new_word = (Word *) calloc(1, sizeof(Word));
    for (int i = 0; i < w->len; i++){
        new_word->syms[i] = w->syms[i];
    }
    new_word->syms[w->len] = sym;
    new_word->syms[w->len + 1] = '\0';
    return new_word;
}

void word_delete(Word *w){
    free(w);
    w = NULL;
}

WordTable *wt_create(void){
    WordTable *wt = (Word *) calloc(MAX_CODE, sizeof(Word);
}

void wt_reset(WordTable *wt);

void wt_delete(WordTable *wt);

