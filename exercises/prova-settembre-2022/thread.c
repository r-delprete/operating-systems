/**
 * Si realizzi un programma in Ce Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia k thread per calcolare il prodotto di due matrici di dimensione kxm e mxp.
 * Non appena sarà calcolata la matrice prodotto, un k+1-esimo thread aggiuntivo, che era rimasto in attesa,
 * provvederà a stampare la matrice risultato. Le matrici devono essere allocate dinamicamente.
 * Usare le variabili di condizione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#define NUM_THREADS 8
#define M 5
#define P 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int **firstMatrix, **secondMatrix, **resultMatrix, productsCounter = 0;

int lock(pthread_mutex_t mutex)
{
  return pthread_mutex_lock(&mutex);
}

int unlock(pthread_mutex_t mutex)
{
  return pthread_mutex_unlock(&mutex);
}

int **matrixGeneration(int rows, int cols, bool hasToFill)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  if (hasToFill)
  {
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        matrix[i][j] = 1 + rand() % 10;
      }
    }
  }

  return matrix;
}

void printMatrix(int **matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

void *matricesProductRoutine(void *args)
{
  int threadID = *((int *)args);
  free(args);

  lock(mutex);
  for (int i = 0; i < P; i++)
  {
    for (int j = 0; j < M; j++)
    {
      resultMatrix[threadID][i] += firstMatrix[threadID][j] * secondMatrix[j][i];
      productsCounter++;
    }
  }

  if (productsCounter == (NUM_THREADS * M * P))
  {
    pthread_cond_signal(&condition);
  }

  unlock(mutex);

  return NULL;
}

void *printResultMatrixRoutine(void *args)
{
  lock(mutex);
  while (productsCounter != (NUM_THREADS * M * P))
  {
    pthread_cond_wait(&condition, &mutex);
  }

  printf("Result Matrix\n");
  printMatrix(resultMatrix, NUM_THREADS, P);

  unlock(mutex);

  return NULL;
}

void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  pthread_t *threads = malloc((NUM_THREADS + 1) * sizeof(pthread_t));
  firstMatrix = matrixGeneration(NUM_THREADS, M, true);
  secondMatrix = matrixGeneration(M, P, true);
  resultMatrix = matrixGeneration(NUM_THREADS, P, false);

  printf("First matrix\n");
  printMatrix(firstMatrix, NUM_THREADS, M);
  printf("\n");

  printf("Second matrix\n");
  printMatrix(secondMatrix, M, P);
  printf("\n");

  for (int i = 0; i < NUM_THREADS; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, matricesProductRoutine, threadID);
  }

  pthread_create(&threads[NUM_THREADS], NULL, printResultMatrixRoutine, NULL);

  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_join(threads[NUM_THREADS], NULL);

  matrixDeallocation(firstMatrix, NUM_THREADS);
  matrixDeallocation(secondMatrix, M);
  matrixDeallocation(resultMatrix, NUM_THREADS);
  free(threads);

  return EXIT_SUCCESS;
}