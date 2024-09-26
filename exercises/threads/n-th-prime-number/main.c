/**
 * Calcolo dell'n-esimo numero primo
 */

#include <pthread.h>
#include "../libs/apue.h"

void *getNthPrimeNumber(void *args)
{
  int nThNumber = *((int *)args);
  int factor, isPrime;
  static int candidate = 2;

  while (1)
  {
    isPrime = 1;

    for (factor = 2; factor < candidate; ++factor)
    {
      if (candidate % factor == 0)
      {
        isPrime = 0;
        break;
      }
    }

    if (isPrime)
    {
      if (--nThNumber == 0)
      {
        return ((void *)&candidate);
      }
    }

    ++candidate;
  }
}

int main()
{
  pthread_t threadID;
  int whichPrime = 5;
  void *prime;

  pthread_create(&threadID, NULL, getNthPrimeNumber, (void *)&whichPrime);
  pthread_join(threadID, &prime);

  printf("Prime number = %d\n", *(int *)prime);

  return 0;
}