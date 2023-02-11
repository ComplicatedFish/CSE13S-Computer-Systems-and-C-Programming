#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "universe.h"
#include "inttypes.h"
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define OPTIONS "tsn:i:o:"

int main(int argc, char **argv){
    bool t = false; //determines toroidality
    bool s = false; //determines ncurses display
    uint32_t n_generations = 100;
    uint32_t h = false; //help message eligibility
    char *input = (char *)malloc(PATH_MAX); //PATH_MAX from limits.h stores maximum path length
    char *output = (char *)malloc(PATH_MAX);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch(opt) {
            case 't':
                t = true;
                break;
            case 's':
                s = true;
                break;
            case 'n':
                n_generations = strtoul(optarg, NULL, 10);
                break;
            case 'i':
                input = strcpy(input, optarg);
                break;
            case 'o':
                output = strcpy(output, optarg);
                break;
            default:
                h = 1;
                break;

        }
    }

    uint32_t rows;
    uint32_t cols;

    FILE *fp;
    fp = fopen(input, "r");
    fcanf(fp, "%" SCNd32 "%" SCNd32 "", &rows, &cols);
    Universe *u = uv_create(rows, cols, false);
    printf("%"PRIu32" rows, %"PRIu32" cols\n", uv_rows(u), uv_cols(u));

    uv_live_cell(u, 4, 4);

    uv_dead_cell(u, 2, 3);
    uv_dead_cell(u, 4, 4);

    uv_live_cell(u, 4, 4);
    uv_live_cell(u, 2, 4);
    uv_live_cell(u, 4, 2);
    uv_live_cell(u, 2, 2);
    uint32_t temp = uv_census(u, 3, 3);

    fp = fopen("lists/101.txt", "r");
    tp = fopen("test.txt", "w");
    uv_populate(u, fp);
    uv_print(u, tp);
    fclose(fp);
    fclose(tp);
    printf("census of 3,3 : %u\n", temp);
    printf("PATH_MAX : %d\n", PATH_MAX);
    printf("input : %s\n", input);
    printf("output : %s\n", output);
    printf("help : %d\n", h);
    printf("toroid: %d\n", t);
    printf("n : %u\n", n_generations);
    printf("ncurses: %d\n", s);


    uv_delete(u);
    return 0;
}

