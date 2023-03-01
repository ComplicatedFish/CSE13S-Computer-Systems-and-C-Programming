#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
#include "code.h"

//TrieNode struct constructor. Initializes a uint16_t variable
//and an array or pointers to other TrieNode structs. All pointers
//are initialized as NULL
TrieNode *trie_node_create(uint16_t index){
    TrieNode *node = (TrieNode *) calloc(1, sizeof(TrieNode));
    node->code = index;
    for (int i = 0; i < ALPHABET; i++){
        node->children[i] = NULL;
    }
    return node;
}

//TrieNode struct destrcutor
void trie_node_delete(TrieNode *n){
    free(n);
    n = NULL;
}

//creates a root node for the trie data structure
TrieNode *trie_create(void){
    TrieNode *root = trie_node_create(EMPTY_CODE);
    if (root != NULL){
        return root;
    } else {
        return NULL;
    }
}

/*
void trie_reset(TrieNode *root){
    
}


void trie_delete(TrieNode *n);
*/
TrieNode *trie_step(TrieNode *n, uint8_t sym){
    for (int i = 0; i < ALPHABET; i++){
        if (n->children[i]->code == sym){
            return n->children[i];
        }
    }
    return NULL;
}

int main (void){
    TrieNode *A = trie_node_create(16);
    TrieNode *B = trie_create();
    trie_node_delete(A);
    trie_node_delete(B);
    return 0;
}
