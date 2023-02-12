#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "universe.h"
#include "inttypes.h"
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#define DELAY 50000

#define OPTIONS "tsn:i:o:"

int main(int argc, char **argv){
    bool t = false; //determines toroidality
    bool s = true; //determines ncurses display
    uint32_t n_generations = 100;
    //uint32_t h = false; //help message eligibility
    FILE *input = stdin;
    FILE *output = stdout;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch(opt) {
            case 't':
                t = true;
                break;
            case 's':
                s = false; //silences the ncurses display
                break;
            case 'n':
                n_generations = strtoul(optarg, NULL, 10);
                break;
            case 'i':
                input = fopen(optarg, "r");
                break;
            case 'o':
                output = fopen(optarg, "w");
                break;
            default:
                //h = true;
                break;

        }
    }

    uint32_t rows;
    uint32_t cols;

    //FILE *fp;
    //fp = fopen(input, "r");
    fscanf(input, "%" SCNd32 "%" SCNd32 "", &rows, &cols);
    Universe *A = uv_create(rows, cols, t);
    Universe *B = uv_create(rows, cols, t);

    uv_populate(A, input);
    fclose(input);

    if(s){
        initscr();
        curs_set(FALSE);
    }

    for (uint32_t n = 0; n < n_generations; n++){
        if(s){
            clear();
            for (uint32_t r = 0; r < uv_rows(A); r++){
                for (uint32_t c = 0; c < uv_cols(A); c++){
                    if (uv_get_cell(A, r, c)){
                        mvprintw(r, c, "o");
                    } else {
                        mvprintw(r, c, " ");
                    }
                }
                mvprintw(r, uv_cols(A), "\n");
            }
            refresh();
            usleep(DELAY);
        }
        for (uint32_t r = 0; r < uv_rows(A); r++){
            for (uint32_t c = 0; c < uv_cols(A); c++){
                if (uv_get_cell(A, r, c)){
                    if (uv_census(A, r, c) == 2 || uv_census(A, r, c) == 3){
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                } else {
                    if (uv_census(A, r, c) == 3){
                        uv_live_cell(B, r, c);
                    } else {
                        uv_dead_cell(B, r, c);
                    }
                }
            }

        }
        Universe *temp;
        temp = A;
        A = B;
        B = temp;
    }
    if(s){
        endwin();
    }

    printf("uv_census row 4 col 0 results: %u\n", uv_census(A, 4, 0));
    printf(uv_get_cell(A, 4, 0) ? "true?????\n" : "false\n");
    uv_print(A, output);

    fclose(output);

    uv_delete(A);
    uv_delete(B);
    return 0;
}

/*
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
*/

