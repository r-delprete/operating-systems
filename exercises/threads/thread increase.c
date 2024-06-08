#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
  int a;
  int b;
} global;

pthread_mutex_t *mutexA;
pthread_mutex_t *mutexB;

global glob;

void increaseRoutine()
{
  for (int i = 0; i < 1000; i++)
  {
    pthread_mutex_lock(mutexA);
    glob.a++;
    pthread_mutex_unlock(mutexA);
    pthread_mutex_lock(mutexB);
    glob.b++;
    pthread_mutex_unlock(mutexB);
  }
  pthread_exit(NULL);
}

int main()
{
  pthread_t threads[2];

  mutexA = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  mutexB = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));

  glob.a = 0;
  glob.b = 0;

  for (int i = 0; i < 2; i++)
  {
    pthread_create(&threads[i], NULL, increaseRoutine, NULL);
  }

  for (int i = 0; i < 2; i++)
  {
    pthread_join(threads[i], NULL);
  }

  printf("glob.a = %d; \tglob.b = %d\n", glob.a, glob.b);

  exit(0);
}