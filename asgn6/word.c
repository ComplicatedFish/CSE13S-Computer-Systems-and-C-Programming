#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"
#include "word.h"
#include <string.h>

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
    uint8_t new_syms[w->len + 2];
    Word *new_word = (Word *) calloc(1, sizeof(Word));
    new_word->syms = new_syms;
    for (uint32_t i = 0; i < w->len; i++){
        new_word->syms[i] = w->syms[i];
    }
    new_word->syms[w->len] = sym;
    new_word->syms[w->len + 1] = '\0';
    new_word->len = w->len + 1;
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
    for (int i = 1; i < MAX_CODE; i++){
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
    uint8_t *syms = (uint8_t *) calloc(14, sizeof(uint8_t));
    strcpy(syms, (uint8_t*)"Hello World!");
    Word *a = word_create(syms, 13);
    word_delete(a);
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
