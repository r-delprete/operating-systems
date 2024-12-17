/**
 * Si realizzi un programma in C Posix sotto Linux che, con l'ausilio della libreria Pthread,
 * lancia m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm.
 * L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due matrici,
 * inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima locazione libera disponibile.
 * Non appena l'array sarà riempito, un m+1-esimo thread, che era in attesa, provvederà a stamparne il contenuto.
 * Le matrici devono essere allocate dinamicamente.
 * Usare mutex e variabili di condizione.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int **firstMatrix, **secondMatrix, *resultArray, colsCompleted = 0, m;

void *colsProductsRoutine(void *args)
{
  int threadID = *((int *)args), localSum = 0, firstMatrixProduct = 1, secondMatrixProduct = 1;
  free(args);

  for (int i = 0; i < m; i++)
  {
    firstMatrixProduct *= firstMatrix[i][threadID];
    secondMatrixProduct *= secondMatrix[i][threadID];
  }
  localSum = firstMatrixProduct + secondMatrixProduct;

  pthread_mutex_lock(&mutex);
  if (resultArray[threadID] == -1)
  {
    resultArray[threadID] = localSum;
    colsCompleted++;

    if (colsCompleted == m)
    {
      pthread_cond_signal(&condition);
    }
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void *printArrayRoutine(void *args)
{
  pthread_mutex_lock(&mutex);
  while (colsCompleted < m)
  {
    pthread_cond_wait(&condition, &mutex);
  }

  printf("\nResult array: ");
  for (int i = 0; i < m; i++)
  {
    printf("%d\t", resultArray[i]);
  }
  printf("\n");
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  srand(time(NULL));

  if (argc != 2)
  {
    printf("Error! Correct usage: ./<filename> <m value>\n");
    return EXIT_FAILURE;
  }

  m = atoi(argv[1]);

  pthread_t *threads = malloc((m + 1) * sizeof(pthread_t));

  firstMatrix = calloc(m, sizeof(int *));
  secondMatrix = calloc(m, sizeof(int *));
  resultArray = calloc(m, sizeof(int));

  for (int i = 0; i < m; i++)
  {
    resultArray[i] = -1;
  }

  for (int i = 0; i < m; i++)
  {
    firstMatrix[i] = calloc(m, sizeof(int));
    secondMatrix[i] = calloc(m, sizeof(int));
  }

  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < m; j++)
    {
      firstMatrix[i][j] = 1 + rand() % 10;
      secondMatrix[i][j] = 1 + rand() % 10;
    }
  }

  printf("First Matrix\n");
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < m; j++)
    {
      printf("%d\t", firstMatrix[i][j]);
    }
    printf("\n");
  }

  printf("\nSecond Matrix\n");
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < m; j++)
    {
      printf("%d\t", secondMatrix[i][j]);
    }
    printf("\n");
  }

  pthread_create(&threads[m], NULL, printArrayRoutine, NULL);
  for (int i = 0; i < m; i++)
  {
    int *threadID = malloc(sizeof(int));
    *threadID = i;
    pthread_create(&threads[i], NULL, colsProductsRoutine, threadID);
  }

  pthread_join(threads[m], NULL);
  for (int i = 0; i < m; i++)
  {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < m; i++)
  {
    free(firstMatrix[i]);
    free(secondMatrix[i]);
  }
  free(firstMatrix);
  free(secondMatrix);
  free(threads);
  free(resultArray);

  return EXIT_SUCCESS;
}