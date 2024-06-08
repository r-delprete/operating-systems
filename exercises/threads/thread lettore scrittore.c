/**
 * Facendo uso della libreria Pthread, realizzare un programma in cui un thread scrittore, dato un intero N da riga di comando (dove 10 < N <= 15),
 * scrive in un file nella prima posizione, uno alla volta ed ogni ½ secondo, la sequenza di Fibonacci di ordine N,
 * alternandosi con un thread lettore che legge, uno alla volta dalla prima posizione del file, i numeri scritti dal thread scrittore.
 * Un terzo thread attende la lettura dell’ N-esimo intero, quindi stampa a video il messaggio “Operazioni concluse, arrivederci dal thread: tid”,
 * attende 5 secondi e termina.
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>

/**
 * @param writeSem semaforo per scrittore
 */
sem_t writeSem;

/**
 * @param readSem semaforo per lettore
 */
sem_t readSem;

/**
 * @param mutex variabile per gestione di mutua esclusione
 */
pthread_mutex_t mutex;

/**
 * @param condition variabile di condizione per gestione thread
 */
pthread_cond_t condition;

int n, filePointer, counter;

/**
 * Thread routine per scrivere la sequenza di fibonacci in un file
 */
void *writer();

/**
 * Thread routine per leggere la sequenza di fibonacci da un file scritta dal thread writer
 */
void *reader();

/**
 * Thread routine per stampare a video la sequenza di fibonacci letti dal thread reader
 */
void *print();

int main(int argc, char **argv)
{
  /**
   * @param threadWriter thread scrittore
   */
  pthread_t threadWriter;

  /**
   * @param threadReader thread lettore
   */
  pthread_t threadReader;

  /**
   * @param threadPrint thread stampa
   */
  pthread_t threadPrint;

  sem_init(&writeSem, 0, 1);
  sem_init(&readSem, 0, 0);

  pthread_mutex_init(&mutex, NULL);

  pthread_cond_init(&condition, NULL);

  if (argc != 2)
  {
    printf("Numero di comandi errato");
    exit(1);
  }

  n = atoi(argv[1]);

  if (n <= 10 || n > 15)
  {
    printf("Numero non valido");
    exit(1);
  }

  filePointer = open("es1.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);

  pthread_create(&threadWriter, NULL, writer, NULL);
  pthread_create(&threadReader, NULL, reader, NULL);
  pthread_create(&threadPrint, NULL, print, NULL);

  pthread_join(threadWriter, NULL);
  pthread_join(threadReader, NULL);
  pthread_join(threadPrint, NULL);

  close(filePointer);

  sem_destroy(&readSem);
  sem_destroy(&writeSem);

  pthread_mutex_destroy(&mutex);

  pthread_cond_destroy(&condition);

  return 0;
}

void *writer()
{
  int a = 0, b = 1, c;
  c = a + b;
  a = b;
  b = c;
  lseek(filePointer, 0, SEEK_SET);
  for (int i = 0; i < n; i++)
  {
    sem_wait(&writeSem);
    write(filePointer, &c, sizeof(c));
    usleep(500000);
    sem_post(&readSem);
  }
}

void *reader()
{
  int buff;

  while (1)
  {
    sem_wait(&readSem);
    read(filePointer, &buff, sizeof(buff));
    sem_post(&writeSem);
    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == n)
    {
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mutex);
      pthread_exit(NULL);
    }
    else
    {
      pthread_mutex_unlock(&mutex);
    }
  }
}

void *print()
{
  pthread_mutex_lock(&mutex);

  while (counter < n)
  {
    pthread_cond_wait(&condition, &mutex);
  }

  pthread_mutex_unlock(&mutex);

  printf("Operazioni concluse! Arrivederci dal thread %u", (unsigned int)pthread_self());

  sleep(5);

  pthread_exit(NULL);
}