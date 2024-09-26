/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando
 * la libreria Pthread, generi tre thread
 * • I primi due thread sommano 1000 numeri generati casualmente ed ogni
 *   volta incrementano un contatore
 * • Il terzo thread attende che il contatore incrementato dai due thread
 *   raggiunga un valore limite fornito da riga di comando, notifica l’avvenuta
 *   condizione e termina
 * Utilizzare le variabili condizione
 */

#include "../../libs/lib.h"

#define ARRAY_SIZE 1000

int limit;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
struct
{
  pthread_mutex_t mutex;
  int sum;
  int *array;
  int count;
} firstTwoThreadsInfo = {PTHREAD_MUTEX_INITIALIZER, 0};

void *doSum(void *args)
{
  while (true)
  {
    pthread_mutex_lock(&firstTwoThreadsInfo.mutex);
    if (firstTwoThreadsInfo.count >= limit)
    {
      pthread_mutex_unlock(&firstTwoThreadsInfo.mutex);
      pthread_cond_signal(&condition);

      return NULL;
    }

    firstTwoThreadsInfo.sum += firstTwoThreadsInfo.array[firstTwoThreadsInfo.count];
    firstTwoThreadsInfo.count++;
    pthread_mutex_unlock(&firstTwoThreadsInfo.mutex);
  }
}

void *stop(void *args)
{
  pthread_mutex_lock(&firstTwoThreadsInfo.mutex);
  while (firstTwoThreadsInfo.count < limit)
  {
    pthread_cond_wait(&condition, &firstTwoThreadsInfo.mutex);
  }
  pthread_mutex_unlock(&firstTwoThreadsInfo.mutex);

  printf("Limit reached! Sum = %d\n", firstTwoThreadsInfo.sum);
  pthread_exit(0);
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  checkArgumentsNumber(argc, "<limit>", 2);

  limit = min(atoi(argv[1]), ARRAY_SIZE);

  firstTwoThreadsInfo.array = arrayGeneration(ARRAY_SIZE, true);
  printArray(firstTwoThreadsInfo.array, ARRAY_SIZE);

  pthread_t threads[3];

  for (int i = 0; i < 3; i++)
  {
    if (i < 2)
    {
      pthread_create(&threads[i], NULL, doSum, NULL);
    }
    else
    {
      pthread_create(&threads[i], NULL, stop, NULL);
    }
  }

  for (int i = 0; i < 3; i++)
  {
    pthread_join(threads[i], NULL);
  }

  return 0;
}