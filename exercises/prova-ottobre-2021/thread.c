/**
 * Data una matrice n x n (n pari) di interi generati casualmente, allocata dinamicamente,
 * con n argomento da riga di comando, creare n thread che prelevano casualmente un elemento
 * dalla riga di competenza (thread i-esimo, riga i-esima) e lo inseriscano in un vettore di
 * n2/2 elementi. Un thread n+1-esimo attende il riempimento del vettore per stampare il contenuto
 * dello stesso e per stampare il numero di elementi inserito nel vettore da ciascun thread.
 * Usare semafori Posix basati su nome.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct Position
{
  int row;
  int column;
} Position;

typedef struct ThreadInfo
{
  Position elementPosition;
  int insertedElements;
  int threadID;
  int element;
} ThreadInfo;

ThreadInfo *threadsInfo;
int n, resultsArraySize, *resultsArray, **inputMatrix, insertedElementsCounter;
const char *completedSemName = "/completed";
const char *matrixMutexSemName = "/matrix-mutex";
const char *arrayMutexSemName = "/array-mutex";
sem_t *matrixMutex, *completed, *arrayMutex;

/**
 * Function to create a semaphore if not exists
 * @param semName The semaphore name
 * @param initialValue The smaphore initial value
 * @return The semaphore open if semaphore not exists
 * or it will exit with EXIT_FAILURE
 */
sem_t *createSemaphoreIfNotExists(const char *semName, unsigned int initialValue)
{
  sem_t *sem = sem_open(semName, O_CREAT | O_EXCL, S_IRWXU, initialValue);
  if (sem == SEM_FAILED)
  {
    if (errno == EEXIST)
    {
      // Il semaforo esiste già
      printf("Error: semaphore '%s' already exists.\n", semName);

      exit(EXIT_FAILURE);
    }
    else
    {
      // Altro errore nell'apertura del semaforo
      perror("sem_open error: ");
      exit(EXIT_FAILURE);
    }
  }
  return sem;
}

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

void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
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

int *arrayAllocation(int size)
{
  return calloc(size, sizeof(int));
}

void printArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }
  printf("\n");
}

void *getMatrixElementRoutine(void *args)
{
  Position coordinates = *((Position *)args);

  int element = 0, inserted = 0;
  free(args);

  sem_wait(matrixMutex);
  element = inputMatrix[coordinates.row][coordinates.column];
  sem_post(matrixMutex);

  sem_wait(arrayMutex);
  for (int i = 0; i < resultsArraySize && !inserted; i++)
  {
    if (resultsArray[i] == 0)
    {
      resultsArray[i] = element;
      insertedElementsCounter++;

      threadsInfo[coordinates.row].threadID = coordinates.row;
      threadsInfo[coordinates.row].element = element;
      threadsInfo[coordinates.row].insertedElements++;
      threadsInfo[coordinates.row].elementPosition.row = coordinates.row;
      threadsInfo[coordinates.row].elementPosition.column = coordinates.column;

      inserted = 1;
    }
  }

  if (insertedElementsCounter == resultsArraySize || insertedElementsCounter == n)
  {
    sem_post(completed);
  }
  sem_post(arrayMutex);

  return NULL;
}

void *printInfoRoutine(void *args)
{
  sem_wait(completed);

  sem_wait(arrayMutex);

  printf("\nArray: ");
  printArray(resultsArray, resultsArraySize);
  printf("\n");

  for (int i = 0; i < n; i++)
  {
    printf(
        "Thread %d has inserted %d elements (element %d) got from (%d, %d)\n",
        threadsInfo[i].threadID,
        threadsInfo[i].insertedElements,
        threadsInfo[i].element,
        threadsInfo[i].elementPosition.row,
        threadsInfo[i].elementPosition.column);
  }

  sem_post(arrayMutex);

  return NULL;
}

int main(int argc, char **argv)
{
  sem_unlink(completedSemName);
  sem_unlink(matrixMutexSemName);
  sem_unlink(arrayMutexSemName);

  completed = createSemaphoreIfNotExists(completedSemName, 0);
  matrixMutex = createSemaphoreIfNotExists(matrixMutexSemName, 1);
  arrayMutex = createSemaphoreIfNotExists(arrayMutexSemName, 1);

  srand(time(NULL));

  if (argc != 2)
  {
    printf("Error! Correct usage_: ./<filename> <n value>\n");
    return -1;
  }

  n = atoi(argv[1]);

  if (n % 2 != 0)
  {
    printf("n value must be even\n");
    return -2;
  }

  resultsArraySize = pow(n, 2) / 2;

  inputMatrix = matrixGeneration(n, n);
  resultsArray = arrayAllocation(resultsArraySize);
  threadsInfo = malloc(n * sizeof(struct ThreadInfo));
  pthread_t *threads = malloc((n + 1) * sizeof(pthread_t));

  printf("\nMatrix\n");
  printMatrix(inputMatrix, n, n);

  for (int i = 0; i < n; i++)
  {
    Position *coordinates = malloc(sizeof(Position));

    coordinates->row = i;
    coordinates->column = rand() % n;

    pthread_create(&threads[i], NULL, getMatrixElementRoutine, coordinates);
  }

  pthread_create(&threads[n], NULL, printInfoRoutine, NULL);

  for (int i = 0; i < n; i++)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_join(threads[n], NULL);

  free(threads);
  free(resultsArray);
  matrixDeallocation(inputMatrix, n);

  sem_close(arrayMutex);
  sem_close(completed);
  sem_close(matrixMutex);

  if (completed != SEM_FAILED)
  {
    sem_unlink(completedSemName);
  }

  if (matrixMutex != SEM_FAILED)
  {
    sem_unlink(matrixMutexSemName);
  }

  if (arrayMutex != SEM_FAILED)
  {
    sem_unlink(arrayMutexSemName);
  }

  return 0;
}