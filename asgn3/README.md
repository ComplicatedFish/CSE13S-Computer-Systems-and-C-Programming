################################################################ \
Arnav Nepal \
anepal@ucsc.edu \
Winter 2023 quarter \
CSE13S Long

Assignment 3: Sorting: Putting your affairs in order \
################################################################

#### Description

This programs contains the implementation of a small number of different sorting \
functions and algorithms. The sorts that are included in this program are: quicksort, \
shellsort, heapsort, and batcher sort.

#### Files

batcher.c : main code file for the batcher sort function \
batcher.h : header file for batcher.c \
shell.c : main source file for the shell sort function \
shell.h : header file for shell.c \
gaps.h : contains the gaps array for shell sort, given by gaps.h.py \
heap.c : main source file for the heap sort function \
heap.h : header file for heap.c \
quick.c : main source file for the quick sort function \
quick.h : header file for quick.c \
set.c : contains the set library for creating and usings sets \
set.h : header file for the functions in set.c \
stats.c : given source code for the statistics gathering struct \
stats.h : header file for the functions of stats.c \
sorting.c : main source file for the program, contains main()

Makefile: compiles and recompiles all necessary files in the working directory

DESIGN.pdf : program design featuring psuedocode for the program \
WRITUP.pdf : writeup explaining and analyzing each of the sorting functions \
README.md : this file, containing the readme

#### Instructions

Run "make clean && make all" to compile all necessary C files. \
run program with "./sorting [-ahbsqr:n:p:H]". Within the brackets \
are all the different options for the program: \
   -a              Runs all sorts. \
   -h              Runs Heapsort. \
   -b              Runs Batcher's Odd-Even Merge Sort. \
   -s              Runs Shell Sort. \
   -q              Runs Quicksort. \
   -r seed         Sets the seed for the random number generator that populates the array. \
   -n length       Sets the size of the array to be sorted. \
   -p elements     Sets the number of elements to be printed for each sort. \
   -H              Prints help message (this message).

#### NOTE TO GRADERS (ACADEMIC HONESTY)

In this assignment, I used my own code from last quarters CSE13s class. This is most pertinent to \
shell sort, as it is the only one that is somewhat similar to last year (I did not get quicksort or heapsort \
working correctly last year + there are differences in their implementation this year from last year). I mostly \
followed the psuedocode for shell.c, however I did reference my code from last year in building it. It turned out \
very similar to last years (as the psuedocode was also similar), except for the for-loops because we obtained gaps \
differently this year. Furthermore, I also took code from sorting.c from last year, especially for the \
print\_elements() and reset\_array() functions. \
I am noting this here for posterity.

