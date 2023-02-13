#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "universe.h"
#include "inttypes.h"

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
    for(uint32_t i = 0; i < u->rows; i++){
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
    //if (r < 0 && c < 0) {return;}
    if (r < u->rows && c < u->cols){
        u->grid[r][c] = true;
    }
}

//ooposite of uv_live_cell(), sets the cell at row r and
//column c to false (deal cell)
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c){
    //if (r < 0 && c < 0) {return;}
    if (r < u->rows && c < u->cols){
        u->grid[r][c] = false;
    }
}

//uses same conditions as uv_live_cell() to get the value
//of the cell. If out of bounds, returns false.
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c){
    if (/*r >= 0 && c >= 0 && */r < u->rows && c < u->cols){
        return u->grid[r][c];
    } else {
        return false;
    }
}

//populates the grid with cells according to data in infile.
//runs until fscanf reads pairs of data. Conditional in the while()
//loop checks if uv_live_cell() worked. Function as a boundary checker,
//since uv_live_cell() will only fail to make a cell if the cell index
//is of bounds
bool uv_populate(Universe *u, FILE *infile){
    uint32_t x, y;
    while (fscanf(infile, "%" SCNd32 "%" SCNd32 "", &x, &y) != EOF){
        uv_live_cell(u, x, y);
        if (uv_get_cell(u, x, y) == false){
            return false;
        }
    }
    return true;
}

//checks cells adjacent to row r and column c to determine the number
//of cells that are alive and dead. If toroidality is enabled, then
//additional checks need to be made for certain edge indexes since
//the universe "wraps around" on itself. To check indexes, I used
//2 for loops that loop over the 8 block area around the cell to be
//censused.
uint32_t uv_census(Universe *u, uint32_t r, uint32_t c){
    uint32_t num_neighbors = 0;
    int32_t row_index;
    int32_t col_index;
    if (u->toroidal){
        for (int32_t i = -1; i <= 1; i++){
            for (int32_t j = -1; j <= 1; j++){
                if (j == 0 && i == 0){continue;} //cell being censused doesn't count

                //since C can't do arithmetic modulo (where -1 mod 5 = 4) we have to account
                //for cases where we have to census cells in row 0 or column 0. This means
                //we have to subtract 1 from the num_rows and num_cols to obtain the index
                //at u->rows-1 that is required to be checked when toroidality is enabled
                if (r == 0 && i == -1){
                    row_index = (int32_t)u->rows - 1;
                } else {
                    row_index = ((int32_t)r+i)%((int32_t)u->rows);
                }

                if (c == 0 && j == -1){
                    col_index = (int32_t)u->cols - 1;
                } else {
                    col_index = ((int32_t)c+j)%((int32_t)u->cols);
                }

                if (uv_get_cell(u, row_index, col_index)){
                    num_neighbors++;
                }
            }
        }
    } else { //normal case
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                if (j == 0 && i == 0){continue;} //cell being censused doesn't count
                if (uv_get_cell(u, ((int32_t)r+i), ((int32_t)c+j))){
                    num_neighbors++;
                }
            }
        }
    }
    return num_neighbors;
}

//prints the grid within universe u cell by cell using fputc
void uv_print(Universe *u, FILE *outfile){
    for (uint32_t r = 0; r < u->rows; r++){
        for (uint32_t c = 0; c < u->cols; c++){
            if(uv_get_cell(u, r, c)){
                fputc(111, outfile); //111 is ASCII for lowercase o
            } else {
                fputc(46, outfile); //46 is ASCII for period
            }
        }
        fputc(10, outfile); //10 is ASCII for newline
    }
    return;
}

