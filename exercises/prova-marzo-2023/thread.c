/**
 * Si realizzi un programma in C Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia m thread per calcolare la il prodotto righe-colonne di due matrici di dimensioni mxn e nxp.
 * L'i-esimo thread, dopo aver calcolato l'i-esima riga della matrice risultato ne calcola la somma
 * ed inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima locazione libera disponibile.
 * Non appena l'array sarà riempito, un m+1-esimo thread, che era in attesa, provvederà a stamparne il contenuto.
 * Le matrici devono essere allocate dinamicamente. Usare mutex e variabili di condizione.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int **firstMatrix, **secondMatrix, *resultsArray, completedRows = 0;
int m, n, p;

int **matrixGeneration(int rows, int cols)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      matrix[i][j] = 1 + rand() % 10;
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
}

int *arrayGeneration(int size)
{
  int *array = calloc(size, sizeof(int));

  for (int i = 0; i < size; i++)
  {
    array[i] = -1;
  }

  return array;
}

void printArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }

  printf("\n");
}

void *matricesProductRoutine(void *args)
{
  int threadID = *((int *)args), sum = 0;
  free(args);

  pthread_mutex_lock(&matrixMutex);
  for (int i = 0; i < p; i++)
  {
    int productSum = 0;

    for (int j = 0; j < n; j++)
    {
      productSum += firstMatrix[threadID][j] * secondMatrix[j][i];
    }

    sum += productSum;
  }
  pthread_mutex_unlock(&matrixMutex);

  pthread_mutex_lock(&arrayMutex);
  if (resultsArray[threadID] == -1)
  {
    resultsArray[threadID] = sum;
    completedRows++;
  }

  if (completedRows == m)
  {
    pthread_cond_signal(&cond);
  }
  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}

void *printArrayRoutine(void *args)
{
  pthread_mutex_lock(&arrayMutex);
  while (completedRows < m)
  {
    pthread_cond_wait(&cond, &arrayMutex);
  }

  printf("\nResult array: ");
  printArray(resultsArray, m);
  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 4)
  {
    printf("Error! Correct usage: ./<filename> <m value> <n value> <p value>");
    return EXIT_FAILURE;
  }

  m = atoi(argv[1]);
  n = atoi(argv[2]);
  p = atoi(argv[3]);

  firstMatrix = matrixGeneration(m, n);
  secondMatrix = matrixGeneration(n, p);

  printf("First matrix\n");
  printMatrix(firstMatrix, m, n);

  printf("\nSecond matrix\n");
  printMatrix(secondMatrix, n, p);

  resultsArray = arrayGeneration(m);
  pthread_t *threads = malloc((m + 1) * sizeof(pthread_t));

  for (int i = 0; i < m; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, matricesProductRoutine, threadID);
  }
  pthread_create(&threads[m], NULL, printArrayRoutine, NULL);

  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }
  pthread_join(threads[m], NULL);

  free(threads);
  free(resultsArray);
  matrixDeallocation(firstMatrix, m);
  matrixDeallocation(secondMatrix, n);

  return EXIT_SUCCESS;
}