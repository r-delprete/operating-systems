/**
 * Scrivere un programma C che crea M thread produttori e N thread consumatori (con M=2*N), con M ed N presi come
 * parametro da riga di comando. Tutti i thread condividono una variabile intera. Ogni thread produttore incrementa di due unità
 * la variabile condivisa se e solo se il valore della stessa è inferiore a 100. Ogni thread consumatore decrementa di due unità la
 * variabile condivisa se e solo se il valore della stessa è superiore a 25. Tutti i thread, dopo l’operazione sulla variabile condivisa
 * (di incremento o di decremento) dormono per 2 secondi.
 * Per implementare la sincronizzazione utilizzare i semafori Posix basati su nome.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>

const char *mutexSemName = "/mutex";
const int sleepTime = 2;
int m, n, sharedVar = 0;

sem_t *mutex;

void *increaseVariableRoutine(void *args)
{
  sem_wait(mutex);

  if (sharedVar < 100)
  {
    sharedVar += 2;

    printf("[increaseFunction] New variable value = %d\n", sharedVar);

    sleep(sleepTime);

    sem_post(mutex);
  }
  else
  {
    printf("[increaseFunction] Not changed. Variable value = %d\n", sharedVar);

    sem_post(mutex);
  }

  return NULL;
}

void *decreaseVariableRoutine(void *args)
{
  sem_wait(mutex);

  if (sharedVar > 25)
  {
    sharedVar -= 2;

    printf("[decreaseFunction] New variable value = %d\n", sharedVar);

    sleep(sleepTime);

    sem_post(mutex);
  }
  else
  {
    printf("[decreaseFunction] Not changed. Variable value = %d\n", sharedVar);

    sem_post(mutex);
  }

  return NULL;
}

int main(int argc, const char **argv)
{
  sem_unlink(mutexSemName);

  mutex = sem_open(mutexSemName, O_CREAT | O_EXCL, S_IRWXU, 1);

  if (mutex == SEM_FAILED)
  {
    perror("sem_open error: ");
    return EXIT_FAILURE;
  }

  srand(time(NULL));

  if (argc != 2)
  {
    printf("Error: Correct usage: ./<filename> <n value>\n");
    return -1;
  }

  n = atoi(argv[1]);
  m = 2 * n;

  sharedVar = 1 + rand() % 100;

  printf("Start variable value = %d\n", sharedVar);

  pthread_t *producers = malloc(m * sizeof(pthread_t)), *consumers = malloc(n * sizeof(pthread_t));

  for (int i = 0; i < m; i++)
  {
    pthread_create(&producers[i], NULL, increaseVariableRoutine, NULL);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_create(&consumers[i], NULL, decreaseVariableRoutine, NULL);
  }

  for (int i = 0; i < m; i++)
  {
    pthread_join(producers[i], NULL);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(consumers[i], NULL);
  }

  free(producers);
  free(consumers);

  sem_close(mutex);

  sem_unlink(mutexSemName);

  return 0;
}