################################################################ \
Arnav Nepal \
anepal@ucsc.edu \
Winter 2023 quarter \
CSE13S Long

Assignment 2: A Little Slice of PI \
################################################################

#### Description

This program contains an implementation of several different mathematical functions. \
These functions were built without the use of the C standard math library. functions \
implemented include approximation of PI using 4 different formulas, Approximation of \
Euler's number, and the approximation of square root using the Newton-Raphson method. 

#### Files

e.c : contains code for PI approximation of Euler's number (e) \
madhava.c : contains code for PI approximation using Madhava's series \
euler.c : contains code for PI approximation using Euler's solution to the Basel problem \
bbp.c : PI approximation using the Bailey-Borwein-Plouffe formula \
viete.c : PI approximation using Viete's formula \
newton.c : Approximation of square root using the Newton-Raphson method \
mathlib-test.c : main C file containing main(). Serves as the test harness for the program \

Makefile: compiles and recompiles all necessary files in the working directory

DESIGN.pdf : program design featuring psuedocode for the program \
WRITUP.pdf : writeup explaining and analyzing each of the approximation methods \
README.md : this file, containing the readme

#### Instructions

Run "make clean && make all" to compile all necessary C files. \
run program with "./mathlib-test [-aebmrvnsh]". Within the brackets \
are all the different options for the program: \
  -a   Runs all tests.
  -e   Runs e test.
  -b   Runs BBP pi test.
  -m   Runs Madhava pi test.
  -r   Runs Euler pi test.
  -v   Runs Viete pi test.
  -n   Runs Newton square root tests.
  -s   Print verbose statistics.
  -h   Display program synopsis and usage.

