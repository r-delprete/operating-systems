#include "lib.h"

/**
 * Function to generate a matrix
 * @param rows The matrix rows
 * @param cols The matrix columns
 * @param hasToFill Flag to indicate if the matrix has to fill with random values
 * @return The matrix generated
 */
int **matrixGeneration(int rows, int cols, boolean hasToFill)
{
  int **matrix = calloc(rows, sizeof(int *));

  for (int i = 0; i < rows; i++)
  {
    matrix[i] = calloc(cols, sizeof(int));
  }

  // Filling matrix
  if (hasToFill)
  {
    for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
      {
        int randomNumber = 1 + rand() % 26;
        matrix[i][j] = randomNumber;
      }
    }
  }

  return matrix;
}

/**
 * Function to print a matrix
 * @param matrix The matrix to print
 * @param rows The matrix rows
 * @param cols The columns
 */
void printMatrix(int **matrix, int rows, int cols)
{
  printf("Matrix:\n");

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }
}

/**
 * Function to deallocate a matrix
 * @param matrix The matrix to deallocate
 * @param rows The matrix rows
 */
void matrixDeallocation(int **matrix, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    free(matrix[i]);
  }

  free(matrix);
}

/**
 * Function to generate an array
 * @param size The array size
 * @param hasToFill Flag to indicate if the matrix has to fill with random values
 * @return The array generated
 */

int *arrayGeneration(int size, boolean hasToFill)
{
  int *array = calloc(size, sizeof(int));

  // Filling array
  if (hasToFill)
  {
    for (int i = 0; i < size; i++)
    {
      int randomNumber = 1 + rand() % 30;
      array[i] = randomNumber;
    }
  }

  return array;
}

/**
 * Function to print an array
 * @param array The array to print
 * @param size The array size
 */
void printArray(int *array, int size)
{
  printf("Array:\n");

  for (int i = 0; i < size; i++)
  {
    printf("%d\t", array[i]);
  }

  printf("\n");
}

/**
 * Function to get minimum between two number
 * @param a The first number
 * @param b The second number
 * @return The minimum value
 */
int min(int a, int b)
{
  return a < b ? a : b;
}

/**
 * Function to check argument number passed into command line
 * @param argc Argument number passed
 * @param msg Message to show
 * @param numArgumentsNeeded Arguments number needed to execute program
 * Shows a message if there is an error
 */
void checkArgumentsNumber(int argc, char *msg, int numArgumentsNeeded)
{
  if (argc != numArgumentsNeeded)
  {
    printf("Error! Correct usage is: ./<filename> %s\n", msg);
    exit(-1);
  }
}

/**
 * Function to check if a semaphore exists
 * @param semaphore The semaphore
 * @return true if exists, false otherwise
 */
boolean checkSemaphoreExistence(sem_t *semaphore)
{
  if (semaphore == SEM_FAILED)
  {
    return true;
  }

  return false;
}