################################################################ \
Arnav Nepal \
anepal@ucsc.edu \
Winter 2023 quarter \
CSE13S Long

Assignment 6: Lempel-Ziv Compression \
################################################################

#### Description

This program contains the implementation of the Lempel-Ziv 78  (LZ78) method \
of file compression and decompression. The program is split into 2 files with \
a main() function: one for compressing (encode.c) and one for decompressing \
(decode.c). 

#### Files
encode.c : contains implementation of LZ78 compression \
decode.c : contains implementation of LZ78 decompression \
trie.c : contains implementation of trie ADT \
trie.h : header file for trie.c \
word.c : contains implementation of WordTable ADT \
word.h : header file for word.c \
io.c : contains the input/output module for this program \
io.h : header file for io.c \
endian.h : contains a small module for checking endianness and swapping them if necessary \
code.h : contains a small module that defines the meaning of "codes" being used in this program

Makefile: compiles and recompiles all necessary files in the working directory

DESIGN.pdf : program design featuring psuedocode for the program \
WRITUP.pdf : writeup explaining what I learned in this assignment \
README.md : this file, containing the readme

#### Instructions

Run "make clean && make all" to compile all necessary C files.

## usage
```
run program with "./encode [vi:o:h]". Within the brackets
are all the different options for the program:
        -i input        Specifies input file to encrypt.
        -o output       Specifies file to place encrypted contents into.
        -n pbfile       Specifies file to read public key from.
        -v              Enables verbose output of statistics.
        -h              Prints help message (this message).

run program with "./decode [vi:o:h]". Within the brackets
are all the different options for the program:
        -i input        Specifies input file to encrypt.
        -o output       Specifies file to place encrypted contents into.
        -n pvfile       Specifies file to read private key from.
        -v              Enables verbose output of statistics.
        -h              Prints help message (this message).
```

