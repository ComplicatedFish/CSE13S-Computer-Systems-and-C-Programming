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


uint32_t uv_census(Universe *u, uint32_t r, uint32_t c){
    uint32_t num_neighbors = 0;
    if (u->toroidal){
        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                if (j == 0 && i == 0){continue;} //cell being censused doesn't count
                if (uv_get_cell(u, ((int32_t)r+i)%(u->rows), ((int32_t)c+j)%(u->cols))){
                    num_neighbors++;
                }
            }
        }
    } else {
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
        //fprintf(stderr, "row %u col %u\n", r, c);
    }
    return;
}

/*
int main(void){
    Universe *u = uv_create(50, 50, false);
    printf("%"PRIu32" rows, %"PRIu32" cols\n", uv_rows(u), uv_cols(u));
    uv_live_cell(u, 4, 4);

    FILE *fp;
    FILE *tp;
    fp = fopen("test.txt", "w");
    uv_print(u, fp);
    fclose(fp);

    uv_dead_cell(u, 2, 3);
    uv_dead_cell(u, 4, 4);

    uv_live_cell(u, 4, 4);
    uv_live_cell(u, 2, 4);
    uv_live_cell(u, 4, 2);
    uv_live_cell(u, 2, 2);
    uint32_t temp = uv_census(u, 3, 3);

    fp = fopen("lists/101.txt", "r");
    tp = fopen("test.txt", "w");
    //uv_populate(u, fp);
    uv_print(u, tp);
    fclose(fp);
    fclose(tp);
    printf("%u\n", temp);

    uv_delete(u);
    return 0;
}*/

