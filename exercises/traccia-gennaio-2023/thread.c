/**
 * Si realizzi un programma in C Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia n thread concorrenti per cercare il massimo di ciascuna delle n righe di una matrice mxn
 * di interi e scriverlo in un array di dimensione m nella prima posizione libera disponibile.
 * Un thread m+1-esimo, completato il lavoro dei primi n thread, provvede a cercare il massimo
 * dei massimi e a stamparlo.
 * Usare semafori Posix basati su nome e variabili di condizione.
 * La dimensione della matrice può essere fornita in input al programma in fase di esecuzione
 * o da riga di comando.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;

int **matrix;
int *array;
int m, n;

typedef enum
{
  false,
  true
} boolean;

int **matrixGeneration(int rows, int cols, boolean hasToFill)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  // Filling matrix
  if (hasToFill)
  {
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        int randomNumber = 1 + rand() % 26;
        matrix[i][j] = randomNumber;
      }
    }
  }

  return matrix;
}

void printMatrix(int **matrix, int rows, int cols)
{
  printf("Matrix:\n");

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

void *getMatrixMaxRoutine(void *args)
{
  int index = *((int *)args);

  int max = -1;

  pthread_mutex_lock(&matrixMutex);
  for (int i = 0; i < m; i++)
  {
    if (matrix[i][index] > max)
    {
      max = matrix[i][index];
    }
  }
  pthread_mutex_unlock(&matrixMutex);

  boolean inserted = false;
  int arrayIndex = 0;

  pthread_mutex_lock(&arrayMutex);
  while (inserted == false)
  {
    if (array[arrayIndex] == -1)
    {
      array[arrayIndex] = max;
      inserted = true;
    }

    arrayIndex++;
  }
  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}
void *getArrayMaxRoutine(void *args)
{
  int max = -1;

  pthread_mutex_lock(&arrayMutex);
  for (int i = 0; i < m; i++)
  {
    if (array[i] > max)
    {
      max = array[i];
    }
  }
  pthread_mutex_unlock(&arrayMutex);

  printf("\nMax array value: %d\n", max);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  printf("Insert m value = ");
  scanf("%d", &m);
  printf("\n");

  printf("Insert n value = ");
  scanf("%d", &n);
  printf("\n");

  array = calloc(m, sizeof(int));
  for (int i = 0; i < m; i++)
  {
    array[i] = -1;
  }

  matrix = matrixGeneration(m, n, true);
  printMatrix(matrix, m, n);

  pthread_t threads[n], lastThread;

  for (int i = 0; i < n; i++)
  {
    int *arg = malloc(sizeof(int));
    *arg = i;

    pthread_create(&threads[i], NULL, getMatrixMaxRoutine, arg);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("\nArray: ");
  for (int i = 0; i < m; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");

  pthread_create(&lastThread, NULL, getArrayMaxRoutine, NULL);
  pthread_join(lastThread, NULL);

  matrixDeallocation(matrix, m);
  free(array);

  return 0;
}