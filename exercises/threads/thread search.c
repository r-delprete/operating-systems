#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int coord[2] = {-1, -1};
int key, rows, cols;
int **matrix;
pthread_t *threads;
pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief function per allocare dinamicamente una matrice
 * @return matrice allocata
 */
void createMatrix()
{
  matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < cols; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }
  srand(time(NULL));

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      matrix[i][j] = 1 + rand() % 20;
    }
  }
}

/**
 * @brief function per stampare una matrice
 * @return matrice stampata
 */
void printMatrix()
{
  printf("Matrice: \n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

/**
 * @brief thread routine di ricerca
 * @param threadId id del thread che esegue la ricerca
 * @return coordinate aggiornate
 */
void searchRoutine(void *threadId)
{
  pthread_t *myThreadId = (pthread_t *)threadId;

  printf("Thread %d is searching...\n");

  for (int j = 0; j < cols; j++)
  {
    if (matrix[(int)*myThreadId][j] == key)
    {
      pthread_mutex_lock(&matrixMutex);
      coord[0] = *myThreadId;
      coord[1] = j;

      for (int i = 0; i < rows; i++)
      {
        if (i != (int)*myThreadId)
        {
          printf("Thread %d cancelled. Key found in thread %d!\n", i, (*(int *)myThreadId));
          pthread_cancel(threads[i]);
        }
      }
      pthread_mutex_unlock(&matrixMutex);
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Errore dati in ingresso");
    exit(1);
  }

  pthread_t *threadIds;

  rows = cols = atoi(argv[1]);

  createMatrix();
  printMatrix();

  printf("Key: ");
  scanf("%d", &key);
  printf("\nElement to search: %d\n ", key);

  threadIds = calloc(rows, sizeof(pthread_t));
  threads = calloc(rows, sizeof(pthread_t));

  for (int i = 0; i < rows; i++)
  {
    threadIds[i] = i;
    pthread_create(&threads[i], NULL, searchRoutine, (void *)&threadIds[i]);
  }

  for (int i = 0; i < rows; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("Coordinates: (%d; %d)\n", coord[0], coord[1]);

  exit(0);
}