################################################################ \
Arnav Nepal \
anepal@ucsc.edu \
Winter 2023 quarter \
CSE13S Long

Assignment 1: Getting Acquainted with UNIX and C \
################################################################

#### Description

This assignment serves as an introduction to the Bash shell, Bash scripting, and gnuplot. \
The main C program in this assignment, monte-carlo.c, simulates the Monte Carlo method \
for estimating PI . We will be using data from monte-carlo to plot interesting graphs in \
gnuplot.

#### Files

plot.sh : main bash script for this assignment that creates and manipulates files to create plots \
monte-carlo.c : provided file that simulates the monte carlo method for estimating PI. accepts 2 options:
> -r seed : sets seed for random number generator \
> -n iterations: sets number of iterations monte\_carlo will perform \
Makefile: compiles and recompiles all necessary files in the working directory

DESIGN.pdf : program design featuring psuedocode for plot.sh \
WRITUP.pdf : writeup explaining and analyzing each of the plots created \
README.md : this file, containing the readme

#### Instructions

Run "make clean && make all" to compile all necessary C files. \
run plot.sh with "./plot.sh" to generate 3 plots. Can be rerun to generate new plots \
monte\_carlo can be run on its own to display output with "./monte\_carlo -r seed -n iters
>command line options are optional

