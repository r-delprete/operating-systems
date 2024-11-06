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

struct Position
{
  int row;
  int column;
};

struct ThreadData
{
  int id;
  int element;
  struct Position position;
  int elementsCounter;
};

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int **matrix;
int *array;
int n;
struct ThreadData *threadData;
int elementsInserted = 0;

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

void *getValuesFromMatrixRoutine(void *args)
{
  int competenceRow = *((int *)args), competenceColumn = rand() % n;

  pthread_mutex_lock(&matrixMutex);
  array[competenceRow % ((n + 1) / 2)] = matrix[competenceRow][competenceColumn];

  threadData[competenceRow].id = competenceRow;
  threadData[competenceRow].element = matrix[competenceRow][competenceColumn];
  threadData[competenceRow].position.row = competenceRow;
  threadData[competenceRow].position.column = competenceColumn;
  threadData[competenceRow].elementsCounter++;

  elementsInserted++;

  if (elementsInserted == (n + 1) / 2)
  {
    pthread_cond_signal(&cond);
  }

  pthread_mutex_unlock(&matrixMutex);

  return NULL;
}

void *printArrayRoutine(void *args)
{
  pthread_mutex_lock(&matrixMutex);

  while (elementsInserted < (n + 1) / 2)
  {
    pthread_cond_wait(&cond, &matrixMutex);
  }

  printf("\nArray: ");
  for (int i = 0; i < (n + 1) / 2; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");

  for (int i = 0; i < n; i++)
  {
    printf("\nThread %d ha inserito %d elementi.", threadData[i].id, threadData[i].elementsCounter);
    printf("\nThread %d ha inserito elemento %d preso da posizione (%d, %d).\n", threadData[i].id, threadData[i].element, threadData[i].position.row, threadData[i].position.column);
  }

  pthread_mutex_unlock(&matrixMutex);

  printf("\n");

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  n = atoi(argv[1]);

  int arraySize = (n + 1) / 2;
  array = calloc(arraySize, sizeof(int));

  threadData = calloc(n, sizeof(struct ThreadData));

  pthread_t threads[n], lastThread;
  int rows = n, cols = n;

  matrix = matrixGeneration(rows, cols, true);
  printMatrix(matrix, rows, cols);
  printf("\n");

  for (int i = 0; i < n; i++)
  {
    int *arg = malloc(sizeof(int));
    *arg = i;
    pthread_create(&threads[i], NULL, getValuesFromMatrixRoutine, arg);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_create(&lastThread, NULL, printArrayRoutine, NULL);
  pthread_join(lastThread, NULL);

  matrixDeallocation(matrix, rows);
  free(array);
  free(threadData);

  return 0;
}
