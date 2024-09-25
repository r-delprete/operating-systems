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
        int randomNumber = 1 + random() % 26;
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