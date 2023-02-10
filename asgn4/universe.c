#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "universe.h"

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

//A constructor function for the Universe struct. Takes input rows and
//cols for the grid dimensions of the grid that simulates the game
//universe. toroidal determines if edges wrap around. This function
//first allocates memory for struct elements and then allocates memory
//for the 2-D grid for the game. It then sets the grid pointer within
//the struct to the array that has been initialized. Returns pointer
//to Universe struct

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal){
    Universe *uv = (Universe *)calloc(4, sizeof(Universe));
    uv->rows = rows;
    uv->cols = cols;
    bool **grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++){
        grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    uv->toroidal = toroidal;
    uv->grid = grid;
    return uv;
}

//Deletes (free()'s) all memory allocated during construction of a
//particular Univese struct. Loops over grid to free() each pointer's
//memory within (since matrices are arrays of pointers). Then free()'s
//the grid memory. Also frees the struct memory.

void uv_delete(Universe *u){
    for(uint32_t i = 0; i < u->cols; i++){
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

//simple function, returns rows cotained within
//Universe u
uint32_t uv_rows(Universe *u){
    return u->rows;
}

//returns columns contained within Universe u
uint32_t uv_cols(Universe *u){
    return u->cols;
}

//sets the cell in the u->grid at row r and column c
//to true (to emulate a live cell in the game of life).
//returns 
void uv_live_cell(Universe *u, uint32_t r, uint32_t c){
    if (r < 0 && c < 0) {return;}
    if (r < u->rows && c < u->cols){
        u->grid[r][c] = true;
    }
}

//ooposite of uv_live_cell(), sets the cell at row r and
//column c to false (deal cell)
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c){
    if (r < 0 && c < 0) {return;}
    if (r < u->rows && c < u->cols){
        u->grid[r][c] = false;
    }
}

//uses same conditions as uv_live_cell() to get the value
//of the cell. If out of bounds, returns false.
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c){
    if (r >= 0 && c >= 0 && r < u->rows && c < u->cols){
        return u->grid[r][c];
    } else {
        return false;
    }
}


bool uv_populate(Universe *u, FILE *infile){
    uint32_t x, y;
    while
}

/*
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c);

void uv_print(Universe *u, FILE *outfile);
*/

int main(void){
    Universe *u = uv_create(5, 5, false);
    printf("%u\n", uv_rows(u));
    uv_live_cell(u, 4, 4);
    printf(uv_get_cell(u, 4, 4) ? "true\n" : "false\n");
    printf(uv_get_cell(u, 5, 4) ? "true\n" : "false\n");
    printf(uv_get_cell(u, 1, 2) ? "true\n" : "false\n");
    printf(uv_get_cell(u, 2, 3) ? "true\n" : "false\n");
    uv_live_cell(u, 2, 3);
    printf(uv_get_cell(u, 2, 3) ? "true\n" : "false\n");
    uv_delete(u);
    return 0;
}
