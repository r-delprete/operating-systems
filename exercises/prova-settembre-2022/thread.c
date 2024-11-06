/**
 * Si realizzi un programma in Ce Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia k thread per calcolare il prodotto di due matrici di dimensione kxm e mxp.
 * Non appena sarà calcolata la matrice prodotto, un k+1-esimo thread aggiuntivo, che era rimasto in attesa,
 * provvederà a stampare la matrice risultato. Le matrici devono essere allocate dinamicamente.
 * Usare le variabili di condizione.
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int **firstMatrix, **secondMatrix, **resultMatrix, k, m, p;
int counter = 0;

int **matrixGeneration(int rows, int cols, int hasToFill)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  if (hasToFill == 1)
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
  printf("\n");
}

void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
}

void *matricesSumRoutine(void *args)
{
  int row = *((int *)args);
  free(args);

  pthread_mutex_lock(&mutex);
  for (int i = 0; i < p; i++)
  {
    for (int j = 0; j < m; j++)
    {
      resultMatrix[row][i] += firstMatrix[row][j] * secondMatrix[j][i];
    }
  }

  counter++;

  if (counter == k)
  {
    pthread_cond_signal(&condition);
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void *printMatrixRoutine(void *args)
{
  pthread_mutex_lock(&mutex);
  while (counter != k)
  {
    pthread_cond_wait(&condition, &mutex);
  }

  printf("Result matrix\n");
  printMatrix(resultMatrix, k, p);

  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 4)
  {
    printf("Error! Correct usage: ./<filename> <k value> <m value> <p value>");
    return -1;
  }

  k = atoi(argv[1]);
  m = atoi(argv[2]);
  p = atoi(argv[3]);

  firstMatrix = matrixGeneration(k, m, 1);
  secondMatrix = matrixGeneration(m, p, 1);
  resultMatrix = matrixGeneration(k, p, 0);

  printf("\nFirst matrix\n");
  printMatrix(firstMatrix, k, m);

  printf("\nSecond matrix\n");
  printMatrix(secondMatrix, m, p);

  pthread_t *threads = malloc((k + 1) * sizeof(pthread_t));

  for (int i = 0; i < k; i++)
  {
    int *args = malloc(sizeof(int));
    *args = i;
    pthread_create(&threads[i], NULL, matricesSumRoutine, args);
  }

  pthread_create(&threads[k], NULL, printMatrixRoutine, NULL);

  for (int i = 0; i < k; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_join(threads[k], NULL);

  free(threads);
  matrixDeallocation(firstMatrix, k);
  matrixDeallocation(secondMatrix, m);
  matrixDeallocation(resultMatrix, k);

  return 0;
}