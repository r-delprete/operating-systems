/**
 * Si realizzi un programma in C e Posix sotto Linux che, utilizzando la
 * libreria Pthread
 * • lancia n thread per cercare un elemento in una matrice nxn di caratteri
 * • Ognuno dei thread cerca l'elemento in una delle righe della matrice
 * • Non appena un thread ha trovato l'elemento cercato, rende note agli altri
 * thread le coordinate dell'elemento e tutti i thread terminano (sono
 * cancellati)
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREADS 16
#define MATRIX_ROWS 5
#define MATRIX_COLS 5

struct
{
  pthread_mutex_t mutex;
  int isFound;
  pthread_t *threads;
  int **matrix;
  int numberToSearch;
} threadInfo = {PTHREAD_MUTEX_INITIALIZER};

int **matrixGeneration(int rows, int cols)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  // Filling matrix
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      int randomNumber = 1 + random() % 26;
      matrix[i][j] = randomNumber;
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

void *search(void *args)
{
  int index = *((int *)args);

  pthread_mutex_lock(&threadInfo.mutex);
  for (int i = 0; i < MATRIX_ROWS; i++)
  {
    if (threadInfo.isFound)
    {
      printf("Element already found, thread %d exiting...\n", index);
      pthread_mutex_unlock(&threadInfo.mutex);

      pthread_exit(0);
    }
  }
  pthread_mutex_unlock(&threadInfo.mutex);

  for (int i = 0; i < MATRIX_ROWS; i++)
  {
    if (threadInfo.numberToSearch == threadInfo.matrix[index][i])
    {
      pthread_mutex_lock(&threadInfo.mutex);

      if (!threadInfo.isFound)
      {
        printf("Element found by thread %d at row %d, col %d\n", index, index, i);
        threadInfo.isFound = 1;
      }

      pthread_mutex_unlock(&threadInfo.mutex);
      return NULL;
    }
  }

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  threadInfo.matrix = matrixGeneration(MATRIX_ROWS, MATRIX_COLS);
  printMatrix(threadInfo.matrix, MATRIX_ROWS, MATRIX_COLS);

  if (argc != 3)
  {
    printf("Error! Correct usage is: ./<filename> <threads number> <character to search>\n");
    return -1;
  }

  threadInfo.numberToSearch = atoi(argv[2]);
  printf("\nElement to search = %d\n", threadInfo.numberToSearch);

  int numThreads = atoi(argv[1]) < MAX_THREADS ? atoi(argv[1]) : MAX_THREADS;
  threadInfo.threads = calloc(numThreads, sizeof(pthread_t));

  for (int i = 0; i < numThreads; i++)
  {
    pthread_create(&threadInfo.threads[i], NULL, search, (void *)&i);
  }

  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(threadInfo.threads[i], NULL);
  }

  matrixDeallocation(threadInfo.matrix, MATRIX_ROWS);

  return 0;
}
