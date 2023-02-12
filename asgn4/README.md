################################################################ \
Arnav Nepal \
anepal@ucsc.edu \
Winter 2023 quarter \
CSE13S Long

Assignment 4: The Game of Life \
################################################################

#### Description

This program contains the implementation of John Horton Conway's \
*Game of Life.*

#### Files

universe.c : contains the implementation of the universe Abstract Data Type (ADT) \
universe.h : header file for universe.c \
life.c : contains main() and most of the logic for game of life

Makefile: compiles and recompiles all necessary files in the working directory

DESIGN.pdf : program design featuring psuedocode for the program \
WRITUP.pdf : writeup explaining what I learned in this assignment \
README.md : this file, containing the readme

#### Instructions

Run "make clean && make all" to compile all necessary C files. \
run program with "./life [-tsni:o:h]". Within the brackets \
are all the different options for the program: \
   -t              specifies toroidality of the universe. \
   -s              Silences ncurses. Will stop the ncurses window and animation from displaying. \
   -n generations  Specifies the number of generations of evolution to undergo. \
   -i input        Specifies input file to read from. \
   -o output       Specifies output file to write into.

