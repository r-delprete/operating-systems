/**
 * Si realizzi un programma in C Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia n thread concorrenti per cercare il massimo di ciascuna delle n righe di una matrice mxn
 * di interi e scriverlo in un array di dimensione m nella prima posizione libera disponibile.
 * Un thread m+1-esimo, completato il lavoro dei primi n thread, provvede a cercare il massimo
 * dei massimi e a stamparlo.
 * Usare semafori Posix basati su nome e variabili di condizione.
 * La dimensione della matrice può essere fornita in input al programma in fase di esecuzione
 * o da riga di comando.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

const char *matrixMutexSemName = "/matrix-mutex";
const char *arrayMutexSemName = "/array-mutex";
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;
int n, m, **inputMatrix, *resultArray, insertedMaxes = 0;
sem_t *arraySemMutex, *matrixSemMutex;

int **matrixGeneration(int rows, int cols)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      matrix[i][j] = 1 + rand() % 10;
    }
  }

  return matrix;
}

void printMatrix(int **matrix, int rows, int cols)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

void printArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");
}

void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
}

int *arrayGeneration(int size)
{
  int *array = calloc(size, sizeof(int));

  for (int i = 0; i < size; i++)
  {
    array[i] = -1;
  }

  return array;
}

void *matrixMaxesRoutine(void *args)
{
  int threadID = *((int *)args), localMax = INT_MIN;
  free(args);

  sem_wait(matrixSemMutex);
  for (int i = 0; i < m; i++)
  {
    if (inputMatrix[i][threadID] > localMax)
    {
      localMax = inputMatrix[i][threadID];
    }
  }
  sem_post(matrixSemMutex);

  sem_wait(arraySemMutex);
  resultArray[threadID] = localMax;
  insertedMaxes++;
  if (insertedMaxes == m)
  {
    pthread_cond_signal(&condition);
  }
  sem_post(arraySemMutex);

  return NULL;
}

void *arrayMaxRoutine(void *args)
{
  int localMax = INT_MIN;

  pthread_mutex_lock(&arrayMutex);
  while (insertedMaxes != m)
  {
    pthread_cond_wait(&condition, &arrayMutex);
  }

  sem_wait(arraySemMutex);
  printf("\nMaxes array: ");
  printArray(resultArray, m);

  for (int i = 0; i < m; i++)
  {
    if (resultArray[i] > localMax)
    {
      localMax = resultArray[i];
    }
  }
  sem_post(arraySemMutex);

  printf("Max of maxes: %d\n", localMax);

  return NULL;
}

int main(int argc, char **argv)
{
  sem_unlink(matrixMutexSemName);
  sem_unlink(arrayMutexSemName);

  srand(time(NULL));

  if (argc != 3)
  {
    printf("Error! Correct usage: ./<filename> <m value> <n value>\n");
    return EXIT_FAILURE;
  }

  m = atoi(argv[1]);
  n = atoi(argv[2]);

  inputMatrix = matrixGeneration(m, n);
  printf("Input Matrix\n");
  printMatrix(inputMatrix, m, n);

  resultArray = arrayGeneration(m);
  pthread_t *threads = malloc((m + 1) * sizeof(pthread_t));

  matrixSemMutex = sem_open(matrixMutexSemName, O_CREAT | O_EXCL, S_IRWXU, 1);
  arraySemMutex = sem_open(arrayMutexSemName, O_CREAT | O_EXCL, S_IRWXU, 1);

  pthread_create(&threads[m], NULL, arrayMaxRoutine, NULL);

  for (int i = 0; i < m; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, matrixMaxesRoutine, threadID);
  }

  pthread_join(threads[m], NULL);

  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }

  free(threads);
  free(resultArray);
  matrixDeallocation(inputMatrix, m);

  sem_close(matrixSemMutex);
  sem_close(arraySemMutex);

  sem_unlink(matrixMutexSemName);
  sem_unlink(arrayMutexSemName);

  return EXIT_SUCCESS;
}