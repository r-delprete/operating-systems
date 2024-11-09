/**
 * Scrivere un programma C e Posix che prende da riga di comando N interi compresi tra 5 e 20.
 * Il valore di N è costante ed indicato nel sorgente.
 * Il programma avvia N thread che hanno il seguente comportamento:
 *   a) Ogni thread va in sospensione (invocando la funzione sleep()) per un numero di secondi pari al valore del
 *      corrispondente parametro e poi stampa il suo indice.
 *   b) Alla fine del periodo di attesa (sleep), ogni thread stampa un messaggio: "Thread #: svegliato!".
 *   c) Tutti i thread si sincronizzano tra di loro (utilizzando i semafori basati su memoria).
 *   d) Dopo aver atteso il termine di tutti gli altri thread, ciascun thread scrive sullo schermo che ha terminato ("Thread
 *      #: terminato...").
 */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <semaphore.h>

int n, threadsEnded = 0;
sem_t ended, mutex;

void *routine(void *args)
{
  int threadID = *((int *)args);

  printf("Thread %d go to sleep\n", threadID);
  sleep(threadID + 1);
  printf("Thread %d woke up\n", threadID);

  sem_wait(&mutex);
  threadsEnded++;

  if (threadsEnded == n)
  {
    for (int i = 0; i < n; i++)
    {
      sem_post(&ended);
    }
  }
  sem_post(&mutex);

  sem_wait(&ended);
  printf("Thread %d ended\n", threadID);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  n = 5 + rand() % 16;
  printf("Generated n value = %d\n", n);
  pthread_t *threads = malloc(n * sizeof(pthread_t));
  sem_init(&ended, 0, 0);
  sem_init(&mutex, 0, 1);

  for (int i = 0; i < n; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, routine, threadID);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  free(threads);

  sem_destroy(&ended);
  sem_destroy(&mutex);

  return EXIT_SUCCESS;
}