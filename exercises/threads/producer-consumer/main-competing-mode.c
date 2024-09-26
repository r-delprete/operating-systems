/**
 * Producer and consumer but in competing mode:
 * while a producer producing, if one item is produced a consumer can consume
 */
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ITEMS 10000
#define MAX_THREADS 16

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
} producerInfo = {PTHREAD_MUTEX_INITIALIZER};

struct
{
  /**
   * Mutex var for mutual exclusion
   */
  pthread_mutex_t mutex;

  /**
   * Condition var to wake up/wait a consumer
   */

  pthread_cond_t condition;

  /**
   * Items number ready to consume
   */
  int readyItems;
} consumerInfo = {PTHREAD_MUTEX_INITIALIZER};

void *produce(void *args)
{
  while (1)
  {
    pthread_mutex_lock(&producerInfo.mutex);

    if (producerInfo.position >= numItemsChosen)
    {
      /**
       * In this case, buffer maximum capacity has been reached
       */
      pthread_mutex_unlock(&producerInfo.mutex);
      return NULL;
    }

    // Production
    producerInfo.buffer[producerInfo.position] = producerInfo.currentItemProduced;

    producerInfo.position++;
    producerInfo.currentItemProduced++;

    pthread_mutex_unlock(&producerInfo.mutex);

    pthread_mutex_lock(&consumerInfo.mutex);

    if (consumerInfo.readyItems == 0)
    {
      pthread_cond_signal(&consumerInfo.condition);
    }
    consumerInfo.readyItems++;

    pthread_mutex_unlock(&consumerInfo.mutex);

    *((int *)args) += 1;
  }
}

void *consume(void *args)
{
  for (int i = 0; i < numItemsChosen; i++)
  {
    pthread_mutex_lock(&consumerInfo.mutex);

    while (consumerInfo.readyItems == 0)
    {
      pthread_cond_wait(&consumerInfo.condition, &consumerInfo.mutex);
    }

    // Consume
    consumerInfo.readyItems--;

    pthread_mutex_unlock(&consumerInfo.mutex);

    if (producerInfo.buffer[i] != i)
    {
      printf("Error: buffer[%d] = %d\n", i, producerInfo.buffer[i]);
    }
  }

  return NULL;
}

int min(int a, int b)
{
  return a < b ? a : b;
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

  pthread_create(&consumer, NULL, consume, NULL);

  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(producers[i], NULL);
    printf("Thread %d has produced %d items\n", i + 1, count[i]);
  }

  pthread_join(consumer, NULL);

  return 0;
}
