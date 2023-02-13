#include "inttypes.h"
#include "universe.h"

#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DELAY 50000

#define OPTIONS "tsn:i:o:h"

//helper function to print help message
//copied from assignment 3 with edits for this
//program
void print_help(void);

int main(int argc, char **argv) {
    bool t = false; //determines toroidality
    bool s = true; //determines ncurses display
    uint32_t n_generations = 100;
    bool h = false; //help message eligibility
    FILE *input = stdin;
    FILE *output = stdout;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': t = true; break;
        case 's':
            s = false; //silences the ncurses display
            break;
        case 'n': n_generations = strtoul(optarg, NULL, 10); break;
        case 'i': input = fopen(optarg, "r"); break;
        case 'o': output = fopen(optarg, "w"); break;
        case 'h': h = true; break;
        default: h = true; break;
        }
    }

    if (h) {
        print_help();
        fclose(input);
        fclose(output);
        return 2;
    }

    if (input == NULL || output == NULL) {
        perror("ERROR! : ");
        return 2;
    }

    uint32_t rows;
    uint32_t cols;

    fscanf(input, "%" SCNd32 "%" SCNd32 "", &rows, &cols);
    Universe *A = uv_create(rows, cols, t);
    Universe *B = uv_create(rows, cols, t);

    if (uv_populate(A, input) == false) {
        uv_delete(A);
        uv_delete(B);
        fprintf(stderr, "Error, uv_populate failed!!\n");
        return 2;
    }
    fclose(input);

    if (s) {
        initscr();
        curs_set(FALSE);
    }

    for (uint32_t n = 0; n < n_generations; n++) {
        //this conditional sets up the ncurses window
        //which animates the game of life in a text
        //interface through successive generations
        if (s) {
            clear();
            for (uint32_t r = 0; r < uv_rows(A); r++) {
                for (uint32_t c = 0; c < uv_cols(A); c++) {
                    if (uv_get_cell(A, r, c)) {
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
        /*
         * The following is the main logic of the game of life.
         * it loops over every point in the grid using 2 for
         * loops. The first conditional determines wether
         * the cell at a specific point is alive or dead
         * and successive statements bring to life or kill
         * the cell based on the game rules. The comments within
         * mark which rule is being followed on certain checks
         * and actions.
         */
        for (uint32_t r = 0; r < uv_rows(A); r++) {
            for (uint32_t c = 0; c < uv_cols(A); c++) {
                if (uv_get_cell(A, r, c)) {
                    if (uv_census(A, r, c) == 2 || uv_census(A, r, c) == 3) {
                        uv_live_cell(B, r, c); //rule 1
                    } else {
                        uv_dead_cell(B, r, c); //rule 3
                    }
                } else {
                    if (uv_census(A, r, c) == 3) {
                        uv_live_cell(B, r, c); //rule 2
                    } else {
                        uv_dead_cell(B, r, c); //rule 3
                    }
                }
            }
        }
        //swap universes
        Universe *temp;
        temp = A;
        A = B;
        B = temp;
    }
    if (s) {
        endwin();
    }

    uv_print(A, output);

    fclose(output);

    uv_delete(A);
    uv_delete(B);
    return 0;
}

//helper function to print program help statement
void print_help(void) {
    fprintf(stderr,
        "SYNOPSIS\n"
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
