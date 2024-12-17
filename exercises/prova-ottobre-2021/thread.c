/**
 * Data una matrice n x n (n pari) di interi generati casualmente, allocata dinamicamente,
 * con n argomento da riga di comando, creare n thread che prelevano casualmente un elemento
 * dalla riga di competenza (thread i-esimo, riga i-esima) e lo inseriscano in un vettore di
 * n2/2 elementi. Un thread n+1-esimo attende il riempimento del vettore per stampare il contenuto
 * dello stesso e per stampare il numero di elementi inserito nel vettore da ciascun thread.
 * Usare semafori Posix basati su nome.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct ThreadInfo
{
  int threadID;
  int row;
  int column;
  int element;
  int elementsInserted;
} ThreadInfo;

ThreadInfo *threadsInfos;
const char *mutexSemName = "/mutex";
const char *completedSemName = "/completed";
int **inputMatrix, *resultArray, completedRows = 0, n;

sem_t *mutexSem, *completedSem;

void *getMatrixElementRoutine(void *args)
{
  int threadID = *((int *)args);
  free(args);

  sem_wait(mutexSem);
  for (int i = 0; i < n; i++)
  {
    resultArray[threadID] = inputMatrix[threadID][i];

    threadsInfos[threadID].threadID = threadID;
    threadsInfos[threadID].row = threadID;
    threadsInfos[threadID].column = i;
    threadsInfos[threadID].element = inputMatrix[threadID][i];
    threadsInfos[threadID].elementsInserted++;
    printf("Thread %d has inserted element %d from position (%d, %d)\n", threadID, threadsInfos[threadID].element, threadID, threadsInfos[threadID].column);
  }

  printf("\n");

  completedRows++;

  if (completedRows == n)
  {
    sem_post(completedSem);
  }
  sem_post(mutexSem);

  return NULL;
}

void *printResultArrayRoutine(void *args)
{
  sem_wait(completedSem);

  sem_wait(mutexSem);
  printf("\nResult array: ");
  for (int i = 0; i < n; i++)
  {
    printf("%d\t", resultArray[i]);
  }
  printf("\n");

  for (int i = 0; i < n; i++)
  {
    printf("\nThread %d has inserted %d elements\n", threadsInfos[i].threadID, threadsInfos[i].elementsInserted);
  }

  sem_post(mutexSem);

  return NULL;
}

int main(int argc, char **argv)
{
  sem_unlink(mutexSemName);
  sem_unlink(completedSemName);

  srand(time(NULL));

  if (argc != 2)
  {
    printf("Error! Correct usage: ./<filename> <n value>\n");
    return EXIT_FAILURE;
  }

  n = atoi(argv[1]);
  if (n % 2 != 0)
  {
    printf("Error! n value must be even\n");
    return EXIT_FAILURE;
  }

  mutexSem = sem_open(mutexSemName, O_CREAT | O_EXCL, S_IRWXU, 1);
  completedSem = sem_open(completedSemName, O_CREAT | O_EXCL, S_IRWXU, 0);

  pthread_t *threads = malloc((n + 1) * sizeof(pthread_t));
  threadsInfos = calloc(n, sizeof(ThreadInfo));
  inputMatrix = calloc(n, sizeof(int *));
  resultArray = calloc(n, sizeof(int));
  for (int i = 0; i < n; i++)
  {
    inputMatrix[i] = calloc(n, sizeof(int));
  }

  for (int i = 0; i < n; i++)
  {
    resultArray[i] = -1;
    for (int j = 0; j < n; j++)
    {
      inputMatrix[i][j] = 1 + rand() % 10;
    }
  }

  printf("Matrix\n");
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      printf("%d\t", inputMatrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  pthread_create(&threads[n], NULL, printResultArrayRoutine, NULL);
  for (int i = 0; i < n; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, getMatrixElementRoutine, threadID);
  }

  pthread_join(threads[n], NULL);
  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  free(resultArray);
  for (int i = 0; i < n; i++)
  {
    free(inputMatrix[i]);
  }
  free(inputMatrix);
  free(threads);
  free(threadsInfos);

  sem_close(mutexSem);
  sem_close(completedSem);

  sem_unlink(mutexSemName);
  sem_unlink(completedSemName);

  return EXIT_SUCCESS;
}
