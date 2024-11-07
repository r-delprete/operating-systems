/**
 * Producer and consumer with semaphores
 */

#include "../../libs/lib.h"

#define BUFFER_SIZE 10

const char *matrixMutexSemName = "/mutex";
const char *emptyItemsSemCounterName = "/numItemsEmpty";
const char *storedItemsSemCounterName = "/numItemsStored";

struct
{
  int buffer[BUFFER_SIZE];
  sem_t *mutex, *emptyItemsSemCounter, *storedItemsSemCounter;
} threadInfo;

int numItemsChosen;

void *produce(void *args)
{
  for (int i = 0; i < numItemsChosen; i++)
  {
    sem_wait(threadInfo.emptyItemsSemCounter);

    sem_wait(threadInfo.mutex);
    threadInfo.buffer[i % BUFFER_SIZE] = i;
    sem_post(threadInfo.mutex);

    sem_post(threadInfo.storedItemsSemCounter);

    sleep(1);
  }

  return NULL;
}

void *consume(void *args)
{
  for (int i = 0; i < numItemsChosen; i++)
  {
    sem_wait(threadInfo.storedItemsSemCounter);

    sem_wait(threadInfo.mutex);
    if (threadInfo.buffer[i % BUFFER_SIZE] != i)
    {
      printf("Error! buffer[%d] = %d, index = %d\n", i % BUFFER_SIZE, threadInfo.buffer[i % BUFFER_SIZE], i);
      return NULL;
    }

    printf("Consuming...\n");

    sem_post(threadInfo.mutex);

    sem_post(threadInfo.emptyItemsSemCounter);
  }

  return NULL;
}

int main(int argc, char **argv)
{
  sem_unlink(matrixMutexSemName);
  sem_unlink(emptyItemsSemCounterName);
  sem_unlink(storedItemsSemCounterName);

  checkArgumentsNumber(argc, "<items number>", 2);

  numItemsChosen = atoi(argv[1]);

  pthread_t producer, consumer;

  threadInfo.mutex = sem_open(matrixMutexSemName, O_CREAT | O_EXCL, FILE_MODE, 1);
  threadInfo.emptyItemsSemCounter = sem_open(emptyItemsSemCounterName, O_CREAT | O_EXCL, FILE_MODE, BUFFER_SIZE);
  threadInfo.storedItemsSemCounter = sem_open(storedItemsSemCounterName, O_CREAT | O_EXCL, FILE_MODE, 0);

  if (checkSemaphoreExistence(threadInfo.mutex) || checkSemaphoreExistence(threadInfo.emptyItemsSemCounter) || checkSemaphoreExistence(threadInfo.storedItemsSemCounter))
  {
    if (checkSemaphoreExistence(threadInfo.mutex))
    {
      printf("Error: semaphore with name %s already exists\n", matrixMutexSemName);
    }

    if (checkSemaphoreExistence(threadInfo.emptyItemsSemCounter))
    {
      printf("Error: semaphore with name %s already exists\n", emptyItemsSemCounterName);
    }

    if (checkSemaphoreExistence(threadInfo.storedItemsSemCounter))
    {
      printf("Error: semaphore with name %s already exists\n", storedItemsSemCounterName);
    }

    return EXIT_FAILURE;
  }

  pthread_create(&producer, NULL, produce, NULL);
  pthread_create(&consumer, NULL, consume, NULL);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  sem_close(threadInfo.mutex);
  sem_close(threadInfo.emptyItemsSemCounter);
  sem_close(threadInfo.storedItemsSemCounter);

  sem_unlink(matrixMutexSemName);
  sem_unlink(emptyItemsSemCounterName);
  sem_unlink(storedItemsSemCounterName);

  return 0;
}