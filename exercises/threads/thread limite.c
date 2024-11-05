#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

int counter = 0, limit;

void *somma()
{
  for (int i = 0; i < 1000; i++)
  {
    pthread_mutex_lock(&matrixMutex);
    counter += rand() % 50;
    if (counter >= limit)
    {
      pthread_mutex_unlock(&matrixMutex);
      pthread_cond_signal(&condition);
      pthread_exit(NULL);
    }
    else
    {
      pthread_mutex_unlock(&matrixMutex);
    }
  }
  pthread_exit(NULL);
}

void *control()
{
  pthread_mutex_lock(&matrixMutex);
  while (counter < limit)
  {
    pthread_cond_wait(&condition, &matrixMutex);
  }
  pthread_mutex_unlock(&matrixMutex);
  printf("Limite raggiunto!\n");
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("numero argomenti errato!\n");
    return 1;
  }

  pthread_t threads[3];

  limit = atoi(argv[1]);
  srand(time(NULL));

  for (int i = 0; i < 2; i++)
  {
    pthread_create(&threads[i], NULL, &somma, NULL);
  }
  pthread_create(&threads[2], NULL, &control, NULL);

  for (int i = 0; i < 3; i++)
  {
    pthread_join(threads[i], NULL);
  }

  return 0;
}