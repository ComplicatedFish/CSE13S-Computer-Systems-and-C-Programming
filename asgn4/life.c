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

#define OPTIONS "tsn:i:o:h"

//helper function to print help message
//copied from assignment 3 with edits for this
//program
void print_help(void);

int main(int argc, char **argv){
    bool t = false; //determines toroidality
    bool s = true; //determines ncurses display
    uint32_t n_generations = 100;
    bool h = false; //help message eligibility
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
            case 'h':
                h = true;
                break;
            default:
                h = true;
                break;

        }
    }

    if(h){
        print_help();
        fclose(input);
        fclose(output);
        return 2;
    }

    uint32_t rows;
    uint32_t cols;

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

    uv_print(A, output);

    fclose(output);

    uv_delete(A);
    uv_delete(B);
    return 0;
}

void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   An implementation of John Horton Conway's Game of life.\n"
                    "\n"
                    "USAGE\n"
                    "   ./life [-i:o:n:st]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -t              Specifies a toroidal universe.\n"
                    "   -s              Silences the ncurses animation and window.\n"
                    "   -n generations  Specifies the number of generations to undergo in game of life.\n"
                    "   -i input        Specifies the input file to read from.\n"
                    "   -o output       Specifies the out file to write to.\n");
    return;
}

