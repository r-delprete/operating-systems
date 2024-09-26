/**
 * Realizzare un programma C e Posix sotto Linux che con l'uso dei
 * semafori Posix sincronizzi un processo padre ed un processo figlio
 * che scrivono e leggono, rispettivamente, un numero intero alla
 * volta (da 1 a n, dove n è passato da riga di comando) nella prima
 * posizione di un file temporaneo opportunamente creato
 */

#include "../libs/lib.h"

const char *parentSemName = "/parent";
const char *childSemName = "/child";
int numbersLimitToWrite;

/**
 * Function to read integer into a file
 * @param fileDescriptor The file descriptor created
 * @return The number read
 */
int readFromFile(int fileDescriptor)
{
  int numberRead;
  int bytesRead = read(fileDescriptor, &numberRead, sizeof(int));

  if (bytesRead == -1)
  {
    printf("Error reading from file...\n");
    exit(EXIT_FAILURE);
  }

  return numberRead;
}

/**
 * Function to write integer into a file
 * @param fileDescriptor The file descriptor created
 * @return The number to write
 */
void writeIntoFile(int fileDescriptor, int number)
{
  int bytesWritten = write(fileDescriptor, &number, sizeof(int));

  if (bytesWritten == -1)
  {
    printf("Error writing from file...\n");
    exit(EXIT_FAILURE);
  }
}

void parentProcess(int numberToWrite)
{
  int fileDescriptor = open("numbers.dat", O_RDWR | O_CREAT, 0644);

  writeIntoFile(fileDescriptor, numberToWrite);
  printf("Parent process wrote number = %d\n", numberToWrite);

  lseek(fileDescriptor, 0, SEEK_SET);

  fsync(fileDescriptor);

  int numberRead = readFromFile(fileDescriptor);
  printf("Parent process read number = %d\n", numberRead);

  close(fileDescriptor);
}

void childProcess(int numberToWrite)
{
  if (numberToWrite > numbersLimitToWrite)
  {
    return;
  }

  int fileDescriptor = open("numbers.dat", O_RDWR | O_CREAT, 0644);

  writeIntoFile(fileDescriptor, numberToWrite);
  printf("Child process wrote number = %d\n", numberToWrite);

  lseek(fileDescriptor, 0, SEEK_SET);

  fsync(fileDescriptor);

  int numberRead = readFromFile(fileDescriptor);
  printf("Child process read number = %d\n", numberRead);

  close(fileDescriptor);
}

int main(int argc, char **argv)
{
  sem_unlink(parentSemName);
  sem_unlink(childSemName);

  sem_t *parentSem = sem_open(parentSemName, O_CREAT | O_EXCL, FILE_MODE, 1);
  sem_t *childSem = sem_open(childSemName, O_CREAT | O_EXCL, FILE_MODE, 0);

  checkArgumentsNumber(argc, "<n-numbers to write into file>", 2);

  numbersLimitToWrite = atoi(argv[1]);

  int processID = fork();

  int i = 0;

  while (i < numbersLimitToWrite)
  {
    if (!processID)
    {
      // Child process
      sem_wait(childSem);
      int number = i + 2;
      childProcess(number);
      i += 2;
      sem_post(parentSem);
    }
    else
    {
      // Parent process
      sem_wait(parentSem);
      i++;
      parentProcess(i);
      sem_post(childSem);
      i++;
    }
  }

  wait(NULL);

  if (processID == 0)
  {
    sem_close(childSem);
    sem_close(parentSem);
  }
  else
  {
    sem_close(childSem);
    sem_close(parentSem);
    sem_unlink(parentSemName);
    sem_unlink(childSemName);
  }

  return 0;
}