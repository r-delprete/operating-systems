/**
 * Si realizzi un programma in C Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm.
 * L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due matrici,
 * inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima locazione libera disponibile.
 * Non appena l'array sarà riempito, un m+1-esimo thread, che era in attesa, provvederà a stamparne il contenuto.
 * Le matrici devono essere allocate dinamicamente.
 * Usare mutex e variabili di condizione.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int **firstMatrix, **secondMatrix, *resultsArray, m, completedCols = 0;

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

int *arrayGeneration(int size)
{
  return calloc(size, sizeof(int));
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

void printArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");
}

void *columnsSumProducts(void *args)
{
  int column = *((int *)args), sum = 0;

  pthread_mutex_lock(&matrixMutex);
  for (int i = 0; i < m; i++)
  {
    sum += firstMatrix[i][column] * secondMatrix[i][column];
  }
  pthread_mutex_unlock(&matrixMutex);

  pthread_mutex_lock(&arrayMutex);
  if (resultsArray[column] == 0)
  {
    resultsArray[column] = sum;
    completedCols++;
  }

  if (completedCols == m)
  {
    pthread_cond_signal(&condition);
  }

  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}

void *printArrayRoutine(void *args)
{
  pthread_mutex_lock(&arrayMutex);
  while (completedCols == 0)
  {
    pthread_cond_wait(&condition, &arrayMutex);
  }

  printf("\nArray: ");
  printArray(resultsArray, m);

  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 2)
  {
    printf("Error! Correct usage: ./<filename> <m value>");
    return -1;
  }

  m = atoi(argv[1]);
  pthread_t *threads = malloc(m * sizeof(pthread_t)), lastThread;

  firstMatrix = matrixGeneration(m, m);
  secondMatrix = matrixGeneration(m, m);
  resultsArray = arrayGeneration(m);

  printf("First matrix\n");
  printMatrix(firstMatrix, m, m);

  printf("\nSecond matrix\n");
  printMatrix(secondMatrix, m, m);

  for (int i = 0; i < m; i++)
  {
    int *args = malloc(sizeof(int));
    *args = i;
    pthread_create(&threads[i], NULL, columnsSumProducts, args);
  }

  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_create(&lastThread, NULL, printArrayRoutine, NULL);
  pthread_join(lastThread, NULL);

  free(threads);
  free(resultsArray);
  matrixDeallocation(firstMatrix, m);
  matrixDeallocation(secondMatrix, m);

  return 0;
}
