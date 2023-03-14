#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"
#include "word.h"
#include <string.h>
#include <assert.h>

//creates a word object
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *word = (Word *) calloc(1, sizeof(Word));
    word->len = len;
    word->syms = (uint8_t *) calloc(len, sizeof(uint8_t));
    for (uint32_t i = 0; i < len; i++) {
        word->syms[i] = syms[i];
    }
    //word->syms = syms;
    if (word) {
        return word;
    } else {
        printf("NULL\n");
        return NULL;
    }
}

//returns a new word struct after appending
//a sym to the sym of the input word *w
Word *word_append_sym(Word *w, uint8_t sym) {
    //printf("word append entered\n");
    uint8_t new_syms[w->len + 2];
    //printf("check 0\n");
    Word *new_word = word_create(new_syms, w->len + 2); //copies w to new_word
    //printf("check 1\n");
    //new_word->syms = new_syms;
    for (uint32_t i = 0; i < w->len; i++) {
        //printf("i = %u\n", i);
        new_word->syms[i] = w->syms[i];
        //printf("i = %u\n", i);
    }
    new_word->syms[w->len] = sym;
    new_word->syms[w->len + 1] = '\0';
    new_word->len = w->len + 1;
    return new_word;
}

//deletes individual word struct
void word_delete(Word *w) {
    if (w->len != 0 | w->syms != NULL) {
        free(w->syms);
        w->syms = NULL;
    }
    free(w);
    w = NULL;
}

//creates WordTable data type. WordTable is just
//an array of words (or more accurately, an array of
//pointers to words).
WordTable *wt_create(void) {
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(WordTable));
    uint8_t *syms = NULL;
    wt[EMPTY_CODE] = word_create(syms, 0);
    for (int i = START_CODE; i < MAX_CODE; i++) {
        wt[i] = NULL;
    }
    return wt;
}

//resets word table to just the codes at
//index 0 and 1
void wt_reset(WordTable *wt) {
    for (int i = 2; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

//destroys entire WordTable, by looping over
//the wordtable and calling word_delete for each
//word
void wt_delete(WordTable *wt) {
    for (int i = 0; i < MAX_CODE; i++) {
        if (wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
    free(wt);
    wt = NULL;
}

//main used for testing
/*
int main (void){
    //uint8_t *syms = (uint8_t *) calloc(14, sizeof(uint8_t));
    //strcpy(syms, "Hello World!");
    uint8_t *syms = (uint8_t *) "Hello World!\0";
    uint8_t *syms2 = (uint8_t *) "Hello people!\0";
    //Word *a = word_create(syms, 13);
    //word_delete(a);

    WordTable *b = wt_create();
    b[5] = word_create(syms, 13);
    b[78] = word_create(syms2, 14);

    for (int i = 1; i < MAX_CODE; i++){
        if (b[i] != NULL){
            printf("%d index, contains %s of length %u\n", i, b[i]->syms, b[i]->len);
        }
    }
    assert (b[1] != NULL);
    b[6] = word_append_sym(b[5], (uint8_t)'w');
    printf("%s\n", b[6]->syms);

    wt_reset(b);
    wt_delete(b);
}
*/
