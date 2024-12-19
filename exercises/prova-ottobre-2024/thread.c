/**
 * Si realizzi un programma in C Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia m thread per calcolare in modo concorrente il prodotto C di due matrici A e B,
 * di dimensioni mxn e nxd, rispettivamente.
 * Un m+1-esimo thread attende che il calcolo di C sia completo per stamparne il contenuto.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int **matrixA, **matrixB, **matrixC, completedRows = 0, m, n, d;

int **matrixGeneration(int rows, int cols, int hasToFill)
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

void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
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
  printf("\n");
}

void *matricesProductRoutine(void *args)
{
  int threadID = *((int *)args);
  free(args);

  pthread_mutex_lock(&mutex);
  for (int i = 0; i < d; i++)
  {
    for (int j = 0; j < n; j++)
    {
      matrixC[threadID][i] += matrixA[threadID][j] * matrixB[j][i];
    }
  }
  completedRows++;

  if (completedRows == m)
  {
    pthread_cond_signal(&condition);
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void *printResultMatrixRoutine(void *args)
{
  pthread_mutex_lock(&mutex);
  while (completedRows < m)
  {
    pthread_cond_wait(&condition, &mutex);
  }

  printf("Matrix C\n");
  printMatrix(matrixC, m, d);

  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 4)
  {
    printf("Error! Correct usage: ./<filename> <m value> <n value> <d value>\n");
    return EXIT_FAILURE;
  }

  m = atoi(argv[1]);
  n = atoi(argv[2]);
  d = atoi(argv[3]);

  matrixA = matrixGeneration(m, n, 1);
  matrixB = matrixGeneration(n, d, 1);
  matrixC = matrixGeneration(m, d, 0);

  pthread_t *threads = malloc((m + 1) * sizeof(pthread_t));

  printf("Matrix A\n");
  printMatrix(matrixA, m, n);

  printf("Matrix B\n");
  printMatrix(matrixB, n, d);

  pthread_create(&threads[m], NULL, printResultMatrixRoutine, NULL);
  for (int i = 0; i < m; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, matricesProductRoutine, threadID);
  }

  pthread_join(threads[m], NULL);
  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }

  matrixDeallocation(matrixA, m);
  matrixDeallocation(matrixB, n);
  matrixDeallocation(matrixC, m);
  free(threads);

  return EXIT_SUCCESS;
}