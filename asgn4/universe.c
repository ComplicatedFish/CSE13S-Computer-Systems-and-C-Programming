#include <universe.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
}

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal){
    Universe *uv = (Universe *)calloc(4, sizeof(Universe));
    uv->rows = rows;
    uv->cols = cols;
    bool **grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i++){
        grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    uv->toridal = toroidal;
    uv->grid = grid;
    return uv;
}

void uv_delete(Universe *u){
    for(int i = 0; i < u->cols; i++){
        free(u->grid[i]);
        u->grid[i] = NULL;
    }
    free(u->grid);
    u->grid = NULL;
    free(u);
    u = NULL;
}

/*
uint32_t uv_rows(Universe *u);

uint32_t uv_cols(Universe *u);

void uv_live_cell(Universe *u, uint32_t r, uint32_t c);

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c);

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c);

bool uv_populate(Universe *u, FILE *infile);

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c);

void uv_print(Universe *u, FILE *outfile);
*/

int main(void){
    Universe u = uv_create(5, 5, false);
    printf("%u\n", u->rows);
    return 0;
}
