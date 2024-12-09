/**
 * Si realizzi un programma in C e Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia k thread per calcolare il prodotto di due matrici di dimensione k × m e m × p.
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

pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int **firstMatrix, **secondMatrix, **resultMatrix, productsDone = 0, k, m, p;

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

void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
}

void *matricesProductRoutine(void *args)
{
  int threadID = *((int *)args);
  free(args);

  pthread_mutex_lock(&mutex);
  for (int j = 0; j < p; j++)
  {
    for (int i = 0; i < m; i++)
    {
      resultMatrix[threadID][j] += firstMatrix[threadID][i] * secondMatrix[i][j];
    }
  }
  productsDone++;

  if (productsDone == k)
  {
    pthread_cond_signal(&condition);
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void *printResultMatrixRoutine(void *args)
{
  pthread_mutex_lock(&mutex);
  while (productsDone < k)
  {
    pthread_cond_wait(&condition, &mutex);
  }

  printf("\nResult matrix\n");
  printMatrix(resultMatrix, k, p);

  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 4)
  {
    printf("Error! Correct usage: ./<filename> <k value> <m value> <p value>\n");
    return EXIT_FAILURE;
  }

  k = atoi(argv[1]);
  m = atoi(argv[2]);
  p = atoi(argv[3]);

  firstMatrix = matrixGeneration(k, m, true);
  secondMatrix = matrixGeneration(m, p, true);
  resultMatrix = matrixGeneration(k, p, false);

  printf("First matrix\n");
  printMatrix(firstMatrix, k, m);

  printf("\nSecond matrix\n");
  printMatrix(secondMatrix, m, p);

  pthread_t *threads = malloc((k + 1) * sizeof(pthread_t));

  pthread_create(&threads[k], NULL, printResultMatrixRoutine, NULL);

  for (int i = 0; i < k; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, matricesProductRoutine, threadID);
  }

  pthread_join(threads[k], NULL);

  for (int i = 0; i < k; i++)
  {
    pthread_join(threads[i], NULL);
  }

  matrixDeallocation(firstMatrix, k);
  matrixDeallocation(secondMatrix, m);
  matrixDeallocation(resultMatrix, k);
  free(threads);

  return EXIT_SUCCESS;
}