/**
 * scrivere un programma che esegua il comando di shell
 * “ls -R | sort | grep <pat>” dove <pat> è un pattern inserito dall’utente
 */

#include <stdio.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
  int pipe1[2];
  int pipe2[2];

  pipe(pipe1);
  pipe(pipe2);

  if (fork() != 0)
  {
    // padre
    close(pipe1[READ]);

    dup2(pipe1[WRITE], 1);
    close(pipe1[WRITE]);

    execlp("ls", "ls", "-R", NULL);

    perror("ls error: ");
  }
  else if (fork() != 0)
  {
    // primo figlio
    close(pipe1[WRITE]);

    dup2(pipe1[READ], 0);
    close(pipe1[READ]);

    close(pipe2[READ]);

    dup2(pipe2[WRITE], 1);
    close(pipe2[WRITE]);

    execlp("sort", "sort", NULL);

    perror("sort error: ");
  }
  else
  {
    // secondo figlio
    close(pipe1[READ]);
    close(pipe1[WRITE]);

    close(pipe2[WRITE]);

    dup2(pipe2[READ], 0);
    close(pipe2[READ]);

    execlp("grep", "grep", argv[1], NULL);

    perror("grep error: ");
  }
}