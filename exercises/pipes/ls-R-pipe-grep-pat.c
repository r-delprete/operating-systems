/**
 * scrivere un programma che esegua il comando di shell
 * “ls -R | grep <pat>” dove <pat> è un pattern inserito dall’utente
 */

#include <stdio.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{
  int fd[2];

  pipe(fd);

  if (fork() == 0)
  {
    // figlio
    close(fd[WRITE]);

    dup2(fd[READ], 0);
    close(fd[READ]);

    execlp("grep", "grep", argv[1], NULL);

    perror("grep error: ");
  }
  else
  {
    // padre
    close(fd[READ]);

    dup2(fd[WRITE], 1);
    close(fd[WRITE]);

    execlp("ls", "ls", "-R", NULL);

    perror("ls error: ");
  }
}