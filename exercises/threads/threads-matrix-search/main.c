/**
 * Si realizzi un programma in C e Posix sotto Linux che, utilizzando la
 * libreria Pthread:
 *  • lancia n thread per cercare un elemento in una matrice nxn di caratteri
 *  • Ognuno dei thread cerca l'elemento in una delle righe della matrice
 *  • Non appena un thread ha trovato l'elemento cercato, rende note agli altri
 *    thread le coordinate dell'elemento e tutti i thread terminano (sono
 *    cancellati)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct Position
{
  int row, column;
} Position;

typedef struct SearchParams
{
  int elementToSearch;
  Position elementPosition;
} SearchParams;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int **inputMatrix, n;
bool found = false;
SearchParams searchParams;

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

void *searchRoutine(void *args)
{
  int threadID = *((int *)args);
  free(args);

  pthread_mutex_lock(&mutex);
  if (found)
  {
    printf("Thread %d -> Element already found\n", threadID);
    pthread_mutex_unlock(&mutex);
    pthread_cancel(pthread_self());
  }
  else
  {
    for (int j = 0; j < n && !found; j++)
    {
      if (searchParams.elementToSearch == inputMatrix[threadID][j])
      {
        found = true;
        searchParams.elementPosition.row = threadID;
        searchParams.elementPosition.column = j;

        printf("Thread %d -> Element %d found in position (%d, %d)\n",
               threadID,
               searchParams.elementToSearch,
               searchParams.elementPosition.row,
               searchParams.elementPosition.column);
      }
    }

    if (!found)
    {
      printf("Thread %d -> Element %d not found\n", threadID, searchParams.elementToSearch);
    }
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

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
  printf("\n");
  searchParams.elementToSearch = 1 + rand() % 10;
  printf("Value to search = %d\n", searchParams.elementToSearch);

  pthread_t *threads = malloc(n * sizeof(pthread_t));

  for (int i = 0; i < n; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, searchRoutine, threadID);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  free(threads);
  matrixDeallocation(inputMatrix, n);

  return EXIT_SUCCESS;
}