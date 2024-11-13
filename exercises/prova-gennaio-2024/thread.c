/**
 * Data una matrice nxn (n pari) di interi generati casualmente, allocata dinamicamente, con n argomento da riga di comando,
 * creare n thread che prelevano casualmente un elemento dalla riga di competenza (thread i-esimo, riga i-esima)
 * e lo inseriscano concorrentemente in un vettore di (n+1)/2 elementi.
 * Un thread n+1-esimo attende il riempimento del vettore per stampare il contenuto dello stesso
 * e per stampare il numero di elementi inserito nel vettore da ciascun thread.
 * Usare mutex e variabili di condizione
 */

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct ThreadData
{
  /**
   * Number of elements inserted by each thread
   */
  int insertedElements;

  /**
   * Thread ID
   */
  int id;
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int n, arraySize, **inputMatrix, *resultArray, completed = 0;
ThreadData *threadData;

int **matrixAllocation(int rows, int cols)
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

int *arrayAllocation(int size)
{
  return calloc(size, sizeof(int));
}

void printArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");
}

void *getRandomElementFromMatrixRoutine(void *args)
{
  int threadID = *((int *)args);
  free(args);

  pthread_mutex_lock(&mutex);

  threadData[threadID].id = threadID;
  if (resultArray[threadID % arraySize] == 0)
  {
    resultArray[threadID % arraySize] = inputMatrix[threadID][rand() % n];
    threadData[threadID].insertedElements++;
  }
  completed++;

  if (completed == arraySize)
  {
    pthread_cond_signal(&condition);
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void *printArrayRoutine(void *args)
{
  pthread_mutex_lock(&mutex);
  while (completed < arraySize)
  {
    pthread_cond_wait(&condition, &mutex);
  }

  printf("Array: ");
  printArray(resultArray, arraySize);

  for (int i = 0; i < n; i++)
  {
    printf("Thread %d -> Number of elements inserted = %d\n", threadData[i].id, threadData[i].insertedElements);
  }

  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 2)
  {
    printf("Error! Correct usage: ./<filename> <n value>");
    return EXIT_FAILURE;
  }

  n = atoi(argv[1]);
  arraySize = (n + 1) / 2;

  inputMatrix = matrixAllocation(n, n);
  printf("Matrix\n");
  printMatrix(inputMatrix, n, n);
  printf("\n");

  resultArray = arrayAllocation(arraySize);

  pthread_t *threads = malloc((n + 1) * sizeof(pthread_t));
  threadData = calloc(n, sizeof(ThreadData));
  for (int i = 0; i < n; i++)
  {
    threadData[i].insertedElements = 0;
  }

  for (int i = 0; i < n; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, getRandomElementFromMatrixRoutine, threadID);
  }

  pthread_create(&threads[n], NULL, printArrayRoutine, NULL);

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }
  pthread_join(threads[n], NULL);

  free(threads);
  free(resultArray);
  matrixDeallocation(inputMatrix, n);
  free(threadData);

  return EXIT_SUCCESS;
}