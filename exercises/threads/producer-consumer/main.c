/**
 * Producer Consumer algorithm
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/apue.h"

#define MAX_ITEMS 10000
#define MAX_THREADS 12

/**
 * Item number you would produce
 */
int numItemsChosen;

struct
{
  /**
   * Mutex var for mutual exclusion
   */
  pthread_mutex_t mutex;

  /**
   * i-th item position to place into buffer
   */
  int position;

  /**
   * i-th item produced
   */
  int currentItemProduced;

  /**
   * Buffer with all items
   */
  int buffer[MAX_ITEMS];
} items = {PTHREAD_MUTEX_INITIALIZER};

void *produce(void *args)
{
  while (1)
  {
    pthread_mutex_lock(&items.mutex);
    if (items.position >= numItemsChosen)
    {
      pthread_mutex_unlock(&items.mutex);
      return NULL;
    }
    items.buffer[items.position] = items.currentItemProduced;
    items.position++;
    items.currentItemProduced++;
    pthread_mutex_unlock(&items.mutex);
    *((int *)args) += 1;
  }
}

void *consumes(void *args)
{
  for (int i = 0; i < numItemsChosen; i++)
  {
    if (items.buffer[i] != i)
    {
      printf("Error: buffer[%d] = %d\n", i, items.buffer[i]);
    }
  }

  return NULL;
}

int main(int argc, char **argv)
{

  int numThreads;

  /**
   * Check each thread how many items produces
   */
  int count[MAX_THREADS];

  pthread_t producers[MAX_THREADS], consumer;

  if (argc != 3)
  {
    printf("Error! Usage is <items number to create> <threads number to create>");
    return -1;
  }

  numItemsChosen = min(atoi(argv[1]), MAX_ITEMS);
  numThreads = min(atoi(argv[2]), MAX_THREADS);

  for (int i = 0; i < numThreads; i++)
  {
    count[i] = 0;
    pthread_create(&producers[i], NULL, produce, &count[i]);
  }

  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(producers[i], NULL);
    printf("Thread %d has produced %d items\n", i + 1, count[i]);
  }

  pthread_create(&consumer, NULL, consumes, NULL);

  pthread_join(consumer, NULL);

  return 0;
}
