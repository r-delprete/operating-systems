#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Per la funzione sleep()
#include <time.h>   // Per generare numeri casuali

int globalNum = 0;

void *routine(void *threadID)
{

  int tid = *((int *)threadID);

  sleep(rand() % 10 + 1);

  globalNum++;
  printf("Thread %d ha incrementato e il valore attuale è : %d \n", tid, globalNum);

  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  srand(time(0));

  int numThreads = atoi(argv[1]);

  pthread_t threads[numThreads];
  int idThreads[numThreads];

  for (int t = 0; t < numThreads; t++)
  {
    idThreads[t] = t + 1;
    printf("Creazione del thread %d\n", idThreads[t]);
    int rc = pthread_create(&threads[t], NULL, routine, (void *)&idThreads[t]);
    if (rc)
    {
      fprintf(stderr, "Errore nella creazione del thread %d, codice di errore: %d\n", t, rc);
      exit(EXIT_FAILURE);
    }
  }

  for (int t = 0; t < numThreads; t++)
  {
    pthread_join(threads[t], NULL);
  }

  printf("Tutti i thread sono terminati. Valore finale del contatore: %d\n", globalNum);
  return 0;
}