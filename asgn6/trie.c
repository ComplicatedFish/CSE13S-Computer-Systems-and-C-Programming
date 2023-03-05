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
    //printf("free called\n");
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

//from the root, checks if each child is null
//if not, then calls trie_delete on them, which
//kills the entire subtree held by that child
void trie_reset(TrieNode *root){
    for (int i = 0; i < ALPHABET; i++){
        if (root->children[i] == NULL){
            continue;
        } else {
            printf("at index %d trie_delete\n", i);
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

//deletes each child of a trienode and then
//itself. Works recursively
void trie_delete(TrieNode *n){
    for (int i = 0; i < ALPHABET; i++){
        if (n->children[i] == NULL){
            continue;
        } else {
            trie_delete(n->children[i]);
            n->children[i] = NULL;
        }
    }
    trie_node_delete(n);
    //n = NULL;
}

TrieNode *trie_step(TrieNode *n, uint8_t sym){
    for (int i = 0; i < ALPHABET; i++){
        if (n->children[i]->code == sym){
            return n->children[i];
        }
    }
    return NULL;
}

int main (void){
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
}
