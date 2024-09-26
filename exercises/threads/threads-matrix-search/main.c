/**
 * Si realizzi un programma in C e Posix sotto Linux che, utilizzando la
 * libreria Pthread
 * • lancia n thread per cercare un elemento in una matrice nxn di caratteri
 * • Ognuno dei thread cerca l'elemento in una delle righe della matrice
 * • Non appena un thread ha trovato l'elemento cercato, rende note agli altri
 * thread le coordinate dell'elemento e tutti i thread terminano (sono
 * cancellati)
 */

#include "../../libs/lib.h"

#define MAX_THREADS 16
#define MATRIX_ROWS 5
#define MATRIX_COLS 5

struct
{
  pthread_mutex_t mutex;
  boolean isFound;
  pthread_t *threads;
  int **matrix;
  int numberToSearch;
  int threadsCompleted;
} threadInfo = {PTHREAD_MUTEX_INITIALIZER};

void *search(void *args)
{
  int index = *((int *)args);
  free(args);

  pthread_mutex_lock(&threadInfo.mutex);
  if (threadInfo.isFound)
  {
    printf("Element already found, thread %d exiting...\n", index);
    pthread_mutex_unlock(&threadInfo.mutex);

    pthread_exit(0);
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
        threadInfo.isFound = true;
      }
      pthread_mutex_unlock(&threadInfo.mutex);

      return NULL;
    }
  }

  pthread_mutex_lock(&threadInfo.mutex);
  threadInfo.threadsCompleted++;
  pthread_mutex_unlock(&threadInfo.mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  threadInfo.matrix = matrixGeneration(MATRIX_ROWS, MATRIX_COLS, true);
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
    int *index = malloc(sizeof(int));
    *index = i;
    pthread_create(&threadInfo.threads[i], NULL, search, (void *)index);
  }

  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(threadInfo.threads[i], NULL);
  }

  if (threadInfo.threadsCompleted == numThreads)
  {
    printf("Element %d not found\n", threadInfo.numberToSearch);
  }

  matrixDeallocation(threadInfo.matrix, MATRIX_ROWS);
  free(threadInfo.threads);

  return 0;
}
