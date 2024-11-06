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
      matrix[i][j] = 1 + arc4random() % 10;
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
  printf("Array: ");

  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }

  printf("\n");
}

void *matricialProductRoutine(void *args)
{
  int index = *((int *)args);

  int sum = 0;

  pthread_mutex_lock(&matrixMutex);
  for (int i = 0; i < p; i++)
  {
    int productSum = 0;

    for (int j = 0; j < n; j++)
    {
      productSum += firstMatrix[index][j] * secondMatrix[j][i];
    }

    sum += productSum;
  }
  pthread_mutex_unlock(&matrixMutex);

  pthread_mutex_lock(&arrayMutex);
  resultsArray[index] = sum;
  completedRows++;

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

  printf("\n");
  printArray(resultsArray, m);

  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 4)
  {
    printf("Error! Correct usage is: ./<filename> <m value> <n value> <p value>");
    return -1;
  }

  m = atoi(argv[1]);
  n = atoi(argv[2]);
  p = atoi(argv[3]);

  firstMatrix = matrixGeneration(m, n);
  secondMatrix = matrixGeneration(n, p);
  resultsArray = arrayGeneration(m);

  printf("First matrix\n");
  printMatrix(firstMatrix, m, n);

  printf("\nSecond matrix\n");
  printMatrix(secondMatrix, n, p);

  pthread_t *threads = calloc(m, sizeof(pthread_t)), lastThread;

  for (int i = 0; i < m; i++)
  {
    int *args = malloc(sizeof(int));
    *args = i;

    pthread_create(&threads[i], NULL, matricialProductRoutine, args);
  }

  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_create(&lastThread, NULL, printArrayRoutine, NULL);
  pthread_join(lastThread, NULL);

  free(resultsArray);
  matrixDeallocation(firstMatrix, m);
  matrixDeallocation(secondMatrix, n);
  free(threads);

  return 0;
}
