## Synopsis

Assignment of Operating Systems COMP2006

## Contents

ReadMe.txt --------------------- readme file for assignment
matrix.c ----------------------- Contains functions responsible for manipulating matrices in the form of 2d arrays
subtotal.c --------------------- Contains functions responsible for getting subtotals from product matrix
main.c ------------------------- Contains main program for executable pmms
macros.h ----------------------- Header file containing function declarations and Subtotal struct
mat1.txt ----------------------- Contains matrix A from assignment example
mat2.txt ----------------------- Contains matrix B from assignment example
Makefile ----------------------- Used to compile executable
Report.pdf --------------------- Report for assignment
declaration_of_originality.pdf - Declaration of Academic Integrity

## Dependencies

Libraries - stdio.h
----------- stdlib.h
----------- unistd.h
----------- fcntl.h
----------- pthread.h
----------- semaphore.h
----------- sys/wait.h
----------- sys/mman.h

Matrix files required, so input matrices can be read from them

## How to run

$ make
$ ./pmms <matrixFile1> <matrixFile2> <rowsOfMatrix1> <colsOfmatrix1/rowsOfMatrix2> <colsOfMatrix2>

## Version Information

3/NOV/2021 - initial version of Assignment programs
