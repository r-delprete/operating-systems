/**
 * Scrivere un programma in ambiente linux che implementi, stile shell BASH,
 * la seguente pipeline inserita da riga di comando: ls -lF | wc -c
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
  int *fd = calloc(2, sizeof(int));
  pipe(fd);

  if (fork())
  {
    close(fd[READ]);

    dup2(fd[WRITE], WRITE);
    close(fd[WRITE]);

    execlp("ls", "ls", "-lF", NULL);

    perror("ls -lF error: ");

    exit(EXIT_FAILURE);
  }
  else
  {
    close(fd[WRITE]);

    dup2(fd[READ], READ);
    close(fd[READ]);

    execlp("wc", "wc", "-c", NULL);

    perror("wc -c error: ");

    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}