/**
 * "wc","wc","-w" "ls","ls"
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#define read 0
#define write 1

int main(int argc, char **argv)
{
  /**
   * @param dscr descrittore di file; in questo caso, di due file
  */
  int dscr[2];

  /**
   * crea un canale di comunicazione tra processi
   * @param filedes[2] contiene due descrittori di file
   * @param filedes[0] descrittore usato in lettura
   * @param filedes[1] descrittore usato in scrittura
   * @return 0 se va a buon fine
   * @return -1 se c'e' un errore
   */
  pipe(dscr);

  pid_t pid = fork();

  if (pid)
  {
    close(dscr[read]);
    dup2(dscr[write], 1);
    close(dscr[write]);

    execlp("wc", "wc", "-w", NULL);
  }
  else
  {
    close(dscr[write]);
    dup2(dscr[read], 0);
    close(dscr[read]);

    execlp("ls", "ls", NULL);
  }
}

/**
 * Scrivere un programma che esegua il comando di
 * shell “ls- R | grep <pat>” dove <pat> è un pattern inserito
 * dall’utente.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#define read 0
#define write 1

int main(int argc, char **argv)
{
  /**
   * @param dscr[2] descrittore, dove dscr[0] in lettura e dscr[1] in scrittura
   */
  int dscr[2];
  pipe(dscr);

  /**
   * @param pid id del processo
   */
  pid_t pid = fork();

  if (pid)
  {
    close(dscr[read]);
    dup2(dscr[write], 1);
    close(dscr[write]);

    execlp("ls", "ls", "-R", NULL);
  }
  else
  {
    close(dscr[write]);
    dup2(dscr[read], 0);
    close(dscr[read]);

    execlp("grep", "grep", argv[1], NULL);
  }
}

/**
 * Implementare in C un programma che esegua la seguente pipeline:
 * cat file.txt | tr'[[:space:]] [[:punct:]]''\n' | sort | uniq
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define read 0
#define write 1

int main(int argc, char **argv)
{
  /**
   * @param dscr1[2] primo descrittore
   */
  int dscr1[2];
  pipe(dscr1);
  /**
   * @param dscr2[2] secondo descrittore
   */
  int dscr2[2];
  pipe(dscr2);

  /**
   * @param dscr3[2] terzo descrittore
   */
  int dscr3[2];
  pipe(dscr3);

  /**
   * @param pid[3] array con id dei processi
   */
  pid_t pid[3] = {0};
  pid[0] = fork();

  if (pid[0])
  {
    pid[1] = fork();
  }

  if (pid[1])
  {
    pid[2] = fork();
  }

  if (pid[0] && pid[1] && pid[2])
  {
    close(dscr1[read]);
    dup2(dscr1[write], 1);
    close(dscr1[write]);

    execlp("cat", "cat", "file.txt", NULL);
  }
  else if (!pid[0] && !pid[1] && !pid[2])
  {
    close(dscr1[write]);
    dup2(dscr1[read], 0);
    close(dscr1[read]);

    close(dscr2[read]);
    dup2(dscr2[write], 1);
    close(dscr2[write]);

    execlp("tr", "tr", "'[[:space:]] [[:punct:]]'", "\n", NULL);
  }
  else if (pid[0] && !pid[1] && !pid[2])
  {
    close(dscr1[read]);
    close(dscr1[write]);

    close(dscr2[write]);
    dup2(dscr2[read], 0);
    close(dscr2[read]);

    execlp("sort", "sort", NULL);
  }
  else if (pid[0] && pid[1] && !pid[2])
  {
    close(dscr1[read]);
    close(dscr1[write]);

    close(dscr2[read]);
    close(dscr2[write]);

    close(dscr3[write]);
    dup2(dscr3[read], 0);
    close(dscr3[read]);

    execlp("uniq", "uniq", NULL);
  }
}