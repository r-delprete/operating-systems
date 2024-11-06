/**
 * Scrivere un programma C che con l'uso della libreria Pthread calcoli in modo concorrente l'espressione 4x + 2y^3 + z^2 - sqrt(b).
 * Il valore complessivo dell'espressione deve essere determinato dal thread principale.
 * I valori delle variabili devono essere passati da riga di comando.
 * Ciascun thread, finito di calcolare il proprio termine dell'espressione, inserisce il proprio tid
 * in un array di dimensioni pari al numero di thread, ad indicare il proprio ordine di conclusione delle operazioni.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

int x, y, z, b, firstTerm, secondTerm, thirdTerm, fourthTerm;
int *threadsOrder;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *expressionValueCalcRoutine(void *args)
{
  int threadID = *((int *)args);

  switch (threadID)
  {
  case 0:
    firstTerm = 4 * x;
    break;

  case 1:
    secondTerm = 2 * pow(y, 3);
    break;

  case 2:
    thirdTerm = pow(z, 2);
    break;

  case 3:
    fourthTerm = sqrt(b);
    break;

  default:
    break;
  }

  pthread_mutex_lock(&mutex);
  threadsOrder[threadID] = threadID;
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char **argv)
{
  if (argc != 5)
  {
    printf("Error! Correct usage: ./<filename> <x value> <y value> <z value> <b value>");
    return -1;
  }

  x = atoi(argv[1]);
  y = atoi(argv[2]);
  z = atoi(argv[3]);
  b = atoi(argv[4]);

  pthread_t *threads = calloc(4, sizeof(pthread_t));
  threadsOrder = calloc(4, sizeof(int));

  for (int i = 0; i < 4; i++)
  {
    int *args = malloc(sizeof(int));
    *args = i;

    pthread_create(&threads[i], NULL, expressionValueCalcRoutine, args);
  }

  for (int i = 0; i < 4; i++)
  {
    pthread_join(threads[i], NULL);
  }

  int expressionResult = firstTerm + secondTerm + thirdTerm + fourthTerm;
  printf("\nExpression result = %d\n", expressionResult);

  printf("\nThreads order: ");
  for (int i = 0; i < 4; i++)
  {
    printf("%d\t", threadsOrder[i]);
  }
  printf("\n");

  free(threads);
  free(threadsOrder);

  return 0;
}