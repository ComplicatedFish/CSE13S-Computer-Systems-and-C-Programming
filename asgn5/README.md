################################################################ \
Arnav Nepal \
anepal@ucsc.edu \
Winter 2023 quarter \
CSE13S Long

Assignment 5: Public Key Cryptography \
################################################################

#### Description

This program contains the implementation of the Schmidt-Samoa (SS) method \
of cryptographic encryption and decryption. The program is split into \
3 files, each which fullfil a specific purpose. Keygen generated the \
public private key pair, Encrypt encrypts a provided file using keys \
made my keygen, and Decrypt decrypts the provided file in a similar manner \

#### Files
decrypt.c : contains main() code for the decryption of files \
encrypt.c : contains main() code for the encryption of files \
keygen.c : contains main() code for the generation of public-private key pairs \
numtheory.c : library of number theoretic functions necessary for SS cryptography \
numtheory.h : header file for the functions in numtheory.c \
randstate.c : Small module for setting global random state for GMP and C library functions \
randstate.h : header file for the functions in randstate.c \
ss.c : library for actual cryptographic logic of SS \
ss.h : header file for the functions in ss.c

Makefile: compiles and recompiles all necessary files in the working directory

DESIGN.pdf : program design featuring psuedocode for the program \
WRITUP.pdf : writeup explaining what I learned in this assignment \
README.md : this file, containing the readme

#### Instructions

Run "make clean && make all" to compile all necessary C files. \

## Keygen
```
run program with "./keygen [-b:i:n:d:s:vh]". Within the brackets
are all the different options for the program:
        -b nbits         Specifies minimum number of bits required for public modulus n.
        -i iters         Specifies number of iterations to check primality of a number with Miller-Rabin.
        -n pbfile        Specifies file to write public key to.
        -i pvfile        Specifies file to write private key to.
        -s seed          Specifies seed for global random state.
        -v               Enables verbose output of statistics and data pertaining to program
        -h               Enables printing the help message

run program with "./encrypt [-i:o:n:vh]". Within the brackets
are all the different options for the program:
        -i input        Specifies input file to encrypt.
        -o output       Specifies file to place encrypted contents into.
        -n pbfile       Specifies file to read public key from.
        -v              Enables verbose output of statistics.
        -h              Prints help message (this message).

run program with "./decrypt [-i:o:n:vh]". Within the brackets
are all the different options for the program:
        -i input        Specifies input file to encrypt.
        -o output       Specifies file to place encrypted contents into.
        -n pvfile       Specifies file to read private key from.
        -v              Enables verbose output of statistics.
        -h              Prints help message (this message).
```
