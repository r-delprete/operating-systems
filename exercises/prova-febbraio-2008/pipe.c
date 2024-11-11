/**
 * Realizzare un programma in C e Posix sotto Linux che, mediante l'uso di pipe,
 * implementi la seguente pipeline:
 * ls -al | sort -k 6 | more -d
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
  int pipe1[2], pipe2[2];

  pipe(pipe1);
  pipe(pipe2);

  pid_t pid1 = fork();

  if (pid1 > 0)
  {
    close(pipe1[READ]);
    dup2(pipe1[WRITE], WRITE);
    close(pipe1[WRITE]);

    if (execlp("ls", "ls", "-al", NULL) == -1)
    {
      perror("'ls -al' error: ");
      exit(EXIT_FAILURE);
    }

    perror("'ls -al' error: ");
  }
  else if (pid1 == 0)
  {
    pid_t pid2 = fork();

    if (pid2 > 0)
    {
      close(pipe1[WRITE]);
      dup2(pipe1[READ], READ);
      close(pipe1[READ]);

      close(pipe2[READ]);
      dup2(pipe2[WRITE], WRITE);
      close(pipe2[WRITE]);

      if (execlp("sort", "sort", "-k", "6", NULL) == -1)
      {
        perror("'sort -k 6' error: ");
        exit(EXIT_FAILURE);
      }
    }
    else if (pid2 == 0)
    {
      close(pipe2[WRITE]);
      dup2(pipe2[READ], READ);
      close(pipe2[READ]);

      if (execlp("more", "more", "-d", NULL) == -1)
      {
        perror("'more -d' error: ");
        exit(EXIT_FAILURE);
      }
    }
  }

  exit(EXIT_SUCCESS);
}