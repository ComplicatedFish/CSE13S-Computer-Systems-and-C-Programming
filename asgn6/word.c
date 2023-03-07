#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"
#include "word.h"
#include <string.h>
#include <assert.h>

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
    uint8_t *new_syms = (uint8_t *) calloc(w->len + 2, sizeof(uint8_t));
    Word *new_word = (Word *) calloc(1, sizeof(Word));
    new_word->syms = new_syms;
    for (uint32_t i = 0; i < w->len; i++){
        new_word->syms[i] = w->syms[i];
    }
    new_word->syms[w->len] = sym;
    new_word->syms[w->len + 1] = '\0';
    new_word->len = w->len + 1;
    free(w); //frees w before returning new_word
    return new_word;
}

void word_delete(Word *w){
    free(w);
    w = NULL;
}

WordTable *wt_create(void){
    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(WordTable));
    uint8_t *syms = NULL;
    wt[EMPTY_CODE] = word_create(syms, 0);
    for (int i = 2; i < MAX_CODE; i++){
        wt[i] = NULL;
    }
    return wt;

}

void wt_reset(WordTable *wt){
    for (int i = 1; i < MAX_CODE; i++){
        if (wt[i] != NULL){
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

void wt_delete(WordTable *wt){
    for (int i = 0; i < MAX_CODE; i++){
        if (wt[i] != NULL){
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
    free(wt);
    wt = NULL;
}

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
    b[5] = word_append_sym(b[5], (uint8_t)'w');
    printf("%s\n", b[5]->syms);

    wt_reset(b);
    wt_delete(b);

    /*
    TrieNode *a = trie_node_create(16);
    TrieNode *b = trie_create();
    b->children[10] = a;

    for (int i = 0; i < 15; i++){
        if (b->children[i] == NULL){
            printf("%d NULL\n", i);
        } else {
            printf("%d %u\n", i, b->children[i]->code);
        }
    }
    trie_reset(b);
    for (int i = 0; i < 15; i++){
        if (b->children[i] == NULL){
            printf("%d NULL\n", i);
        } else {
            printf("before print\n");
            printf("%d %u\n", i, b->children[i]->code);
        }
    }

    trie_delete(b);
    return 0;
    */
}
