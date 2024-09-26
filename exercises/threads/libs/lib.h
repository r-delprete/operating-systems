#ifndef lib_h
#define lib_h

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

typedef enum
{
  false,
  true
} boolean;

int **matrixGeneration(int, int, boolean);
void printMatrix(int **, int, int);
void matrixDeallocation(int **, int);
int *arrayGeneration(int, boolean);
void printArray(int *, int);
int min(int, int);
void checkArgumentsNumber(int, char *, int);

#endif
