/**
 * Si realizzi un programma che lancia m thread per calcolare la somma dei prodotti
 * delle colonne di due matrici mxm, l'i-esimo thread, che dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due matrici,
 * inserisce il risultato in un array m-dimensionale in modo concorrente nella prima locazione disponibile.
 * Non appena l'array sarà riempito, un m+1-esimo thread provvederà a stamparne il contenuto.
 * Le matrici devono essere allocate dinamicamente.
 * Usare mutex e variabili di condizione
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int **firstMatrix, **secondMatrix;
int m, resultIndex = 0;
int *resultVector;

pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_t *threads, printThread;

int **createMatrix()
{
  int **matrix = calloc(m, sizeof(int *));

  for (int i = 0; i < m; i++)
  {
    matrix[i] = calloc(m, sizeof(int));
  }

  srand(time(NULL));
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < m; j++)
    {
      matrix[i][j] = 1 + rand() % 20;
    }
  }
  return matrix;
}

void printMatrix(int **matrix)
{
  printf("\n");
  for (int i = 0; i < m; i++)
  {
    printf("|");
    for (int j = 0; j < m; j++)
    {
      printf("%d ", matrix[i][j]);
    }
    printf("|\n");
  }
}

void *sumProdRoutine(void *args)
{
  int sum = 0, cols = *((int *)args);

  for (int i = 0; i < m; i++)
  {
    sum += firstMatrix[i][cols] * secondMatrix[i][cols];
  }

  pthread_mutex_lock(&matrixMutex);
  resultVector[resultIndex] = sum;
  resultIndex++;

  if (resultIndex == m)
  {
    pthread_cond_signal(&cond);
  }

  pthread_mutex_unlock(&matrixMutex);

  pthread_exit(NULL);
}

void *printVectorRoutine()
{
  pthread_mutex_lock(&matrixMutex);

  while (resultIndex < m)
  {
    pthread_cond_wait(&cond, &matrixMutex);
  }

  printf("Result vector: \n");
  printf("|");
  for (int i = 0; i < m; i++)
  {
    printf("|%d| ", resultVector[i]);
  }
  printf("|\n");

  pthread_mutex_unlock(&matrixMutex);
  pthread_cond_broadcast(&cond);

  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Invalid arguments number!\n");
    exit(1);
  }

  m = atoi(argv[1]);

  firstMatrix = createMatrix();
  printMatrix(firstMatrix);

  secondMatrix = createMatrix();
  printMatrix(secondMatrix);

  resultVector = calloc(m, sizeof(int));
  threads = calloc(m, sizeof(pthread_t));

  for (int i = 0; i < m; i++)
  {
    int *col = calloc(1, sizeof(int));
    col = i;

    pthread_create(&threads[i], NULL, sumProdRoutine, (void *)col);
  }

  pthread_create(&printThread, NULL, printVectorRoutine, NULL);

  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }
  pthread_join(printThread, NULL);

  for (int i = 0; i < m; i++)
  {
    free(firstMatrix[i]);
    free(secondMatrix[i]);
  }

  free(firstMatrix);
  free(secondMatrix);
  free(resultVector);
  free(threads);

  exit(0);
}