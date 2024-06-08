/**
 * Data una matrice nxn (n pari) di interi generata casualmente, allocata dinamicamente, con n argomento da riga di comando,
 * creare n thread che prelevano casualmente un elemento dalla riga di competenza (thread i-esimo, riga i-esima) e lo inseriscano in un vettore
 * di n/2 elementi. Un thread n+1-esimo attende il riempimento del vettore per stampare il contenuto dello stesso e per stampare il numero di elementi
 * inserito nel vettore da ciascun thread. Usare semafori Posix basati su nome.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

/**
 * @brief struct contentente le info dei thread
 * @param counter numero di elementi che ogni thread inserisce nell'array
 * @param threadId id del thread
 */
struct threadInfo
{
  /**
   * @param counter numero di elementi inseriti da un thread
   */
  int counter;

  /**
   * @param id del thread
   */
  pthread_t threadId;
};

/**
 * @brief semaforo per segnalare il riempimento dell'array
 */
sem_t *full;

int rows, n, cols;
int ind = 0;
int *threadIds;
struct threadInfo *insertedElements;
int **matrix;
int *vector;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief funzione per creare una matrice allocata dinamicamente
 * @param rows righe della matrice
 * @param cols colonne della matrice
 * @return la matrice allocata dinamicamente
 */
int **createMatrix()
{
  int **mat = calloc(n, sizeof(int *));
  for (int i = 0; i < cols; i++)
  {
    mat[i] = calloc(n, sizeof(int));
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      mat[i][j] = 1 + rand() % 50;
    }
  }

  return mat;
}

/**
 * @brief funzione per stampare una matrice
 */
void printMatrix()
{
  printf("Matrice: \n");

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

void *routine(void *args)
{
  int rows = *((int *)args);
  int random = rand() % cols;

  pthread_mutex_lock(&mutex);

  if (ind < n / 2)
  {
    vector[ind] = matrix[rows][random];
    insertedElements[ind].threadId = pthread_self();
    insertedElements[ind].counter = 0;
    insertedElements[ind].counter++;
    ind++;
  }
  else
  {
    sem_post(full);
  }
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

void *printElements()
{
  sem_wait(full);

  printf("Vettore: \n");
  for (int i = 0; i < n / 2; i++)
  {
    printf("%d\t", vector[i]);
  }
  printf("\n");

  printf("Numero di elementi inseriti da ogni thread: \n");
  for (int i = 0; i < n / 2; i++)
  {
    printf("Thread id %u ha inserito %d elementi\n", (unsigned)insertedElements[i].threadId, insertedElements[i].counter);
  }
  printf("\n");

  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Numero di argomenti errato!\n");
    exit(1);
  }

  n = atoi(argv[1]);
  if (n % 2 != 0)
  {
    printf("Il numero deve essere pari!\n");
    exit(1);
  }

  rows = cols = n;

  pthread_t threads[n];
  pthread_t threadPrint;

  srand(time(NULL));

  matrix = createMatrix();
  printMatrix();

  threadIds = (int *)malloc(n * sizeof(int));
  insertedElements = (struct threadInfo *)malloc((n / 2) * sizeof(struct threadInfo));
  vector = (int *)malloc(n * sizeof(int));

  full = sem_open("/full", O_CREAT, 0770, 0);

  for (int i = 0; i < n; i++)
  {
    threadIds[i] = i;
    pthread_create(&threads[i], NULL, routine, &threadIds[i]);
  }
  pthread_create(&threadPrint, NULL, printElements, NULL);

  for (int i = 0; i < n; i++)
  {
    pthread_join(threadIds[i], NULL);
  }
  pthread_join(threadPrint, NULL);

  sem_unlink("/full");

  free(threadIds);
  free(vector);
  free(insertedElements);

  exit(0);
}