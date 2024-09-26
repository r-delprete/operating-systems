/**
 * Modifica di una variabile globale usando un mutex
 */

/**
 * Le sleep (line 24 e line 49) sono necessarie per permettere
 * la stampa alternata di "o" e "."
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int globalVar;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *args)
{
  for (int i = 0; i < 20; i++)
  {
    pthread_mutex_lock(&mutex);
    globalVar++;
    pthread_mutex_unlock(&mutex);
    printf(".");
    fflush(stdout);
    sleep(1);
  }

  return NULL;
}

int main()
{
  pthread_t thread;

  if (pthread_create(&thread, NULL, routine, NULL))
  {
    printf("Error creating thread...\n");
    return -1;
  }

  for (int i = 0; i < 20; i++)
  {
    pthread_mutex_lock(&mutex);
    globalVar++;
    pthread_mutex_unlock(&mutex);
    printf("o");
    fflush(stdout);
    sleep(1);
  }

  if (pthread_join(thread, NULL))
  {
    printf("Error joining thread...\n");
    return -2;
  }

  printf("\nglobalVar value = %d\n", globalVar);

  return 0;
}