#ifndef lib_h
#define lib_h

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum
{
  false,
  true
} boolean;

int **matrixGeneration(int rows, int cols, boolean hasToFill);
void printMatrix(int **, int, int);
void matrixDeallocation(int **, int);

#endif
