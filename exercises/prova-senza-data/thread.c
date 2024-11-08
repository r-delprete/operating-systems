/**
 * Si realizzi un programma in C e Posix sotto Linux che, con l'ausilio della libreria Pthread effettui i seguenti task:
 *  - lancia n thread per calcolare la somma degli elementi di ciascuna riga di una matrice n x n di interi generati casualmente in un intervallo [0,255], allocata dinamicamente
 *  - Il calcolo della somma degli elementi di ciascuna riga deve essere effettuato concorrentemente su tutte le righe, secondo la seguente modalità:
 *      i) il thread i-esimo con i pari, calcola la somma degli elementi di indice pari della riga i-esima
 *      ii) il thread con indice i dispari, calcola la somma degli elementi di indice dispari della riga i-esima.
 * Calcolate le somme parziali, si provvederà a ricercarne il minimo ed a stamparlo a video.
 */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <limits.h>

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sumsEnded = PTHREAD_COND_INITIALIZER;
int sumsCounter, *resultsArray, **inputMatrix, n, minimum = INT_MAX;

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
      matrix[i][j] = rand() % 256;
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
  printf("\n");
}

void printArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");
}

void *routine(void *args)
{
  int threadID = *((int *)args), sum = 0;
  free(args);

  pthread_mutex_lock(&matrixMutex);
  if (threadID % 2 == 0)
  {
    for (int i = 0; i < n; i += 2)
    {
      sum += inputMatrix[threadID][i];
    }
  }
  else
  {
    for (int i = 1; i < n; i += 2)
    {
      sum += inputMatrix[threadID][i];
    }
  }
  pthread_mutex_unlock(&matrixMutex);

  resultsArray[threadID] = sum;

  pthread_mutex_lock(&arrayMutex);
  sumsCounter++;

  if (sumsCounter == n)
  {
    printf("\nSums: ");
    for (int i = 0; i < n; i++)
    {
      printf("%d\t", resultsArray[i]);
      if (resultsArray[i] < minimum)
      {
        minimum = resultsArray[i];
      }
    }
    printf("\n");

    pthread_cond_broadcast(&sumsEnded);
  }
  else
  {
    while (sumsCounter < n)
    {
      pthread_cond_wait(&sumsEnded, &arrayMutex);
    }
  }
  pthread_mutex_unlock(&arrayMutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  if (argc != 2)
  {
    printf("Error! Correct usage: ./<filename> <n value>\n");
    return EXIT_FAILURE;
  }

  n = atoi(argv[1]);

  inputMatrix = matrixGeneration(n, n);
  printf("Matrix\n");
  printMatrix(inputMatrix, n, n);

  resultsArray = arrayGeneration(n);

  pthread_t *threads = malloc(n * sizeof(pthread_t));

  for (int i = 0; i < n; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, routine, threadID);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("\nMinimum value = %d\n", minimum);

  free(resultsArray);
  matrixDeallocation(inputMatrix, n);
  free(threads);

  return EXIT_SUCCESS;
}
