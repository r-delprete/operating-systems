/**
 * Realizzare un programma C e Posix sotto Linux che, mediante l'ausilio della libreria pthread, decomponga
 * la somma degli elementi di un vettore di interi di dimensione 1000 in quattro somme locali effettuate
 * concorrentemente da altrettanti thread. Si contempli l'uso di una variabile mutex per regolare l'accesso
 * alla variabile globale che conterrà la somma degli elementi del vettore.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000

typedef struct Range
{
  int low;
  int high;
} Range;

Range *threadsRanges;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int *array, sum = 0, lowRange = 0, highRange = 0;

int *arrayAllocation()
{
  int *array = calloc(ARRAY_SIZE, sizeof(int));

  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    array[i] = 1 + rand() % 50;
  }

  return array;
}

void printArray(int *array)
{
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");
}

void *routine(void *args)
{
  int localSum = 0, threadID = *((int *)args);
  free(args);

  pthread_mutex_lock(&mutex);
  for (int i = threadsRanges[threadID].low; i <= threadsRanges[threadID].high; i++)
  {
    localSum += array[i];
  }

  sum += localSum;
  pthread_mutex_unlock(&mutex);

  printf("Thread %d -> local sum = %d\n", threadID, localSum);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  pthread_t *threads = malloc(NUM_THREADS * sizeof(pthread_t));
  array = arrayAllocation();
  threadsRanges = calloc(NUM_THREADS, sizeof(Range));

  int chunkSize = ARRAY_SIZE / NUM_THREADS;
  int remainder = ARRAY_SIZE % NUM_THREADS;

  int startIndex = 0;
  for (int i = 0; i < NUM_THREADS; i++)
  {
    threadsRanges[i].low = startIndex;
    threadsRanges[i].high = startIndex + chunkSize - 1;

    if (i < remainder)
    {
      threadsRanges[i].high++;
    }

    startIndex = threadsRanges[i].high + 1;
  }

  // printf("Array\n");
  // printArray(array);
  // printf("\n");

  printf("Ranges\n");
  for (int i = 0; i < NUM_THREADS; i++)
  {
    printf("Thread %d -> low range = %d\thigh range = %d\t\n", i, threadsRanges[i].low, threadsRanges[i].high);
  }
  printf("\n");

  for (int i = 0; i < NUM_THREADS; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, routine, threadID);
  }

  for (int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("\nGlobal sum = %d\n", sum);

  free(array);
  free(threads);
  free(threadsRanges);

  return EXIT_SUCCESS;
}