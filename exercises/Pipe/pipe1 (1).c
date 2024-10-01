/*Implementare la seguente pipeline di comandi in ambiente Unix/Linux,
 nello stile della shell
• ls-al|sort–R|wc-w */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    int fd1[2], fd2[2];

    // Creazione delle pipe
    if (pipe(fd1) < 0) {
        perror("Pipe1");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd2) < 0) {
        perror("Pipe2");
        exit(EXIT_FAILURE);
    }

    // Primo figlio - esegue `ls -al`
    if ((pid1 = fork()) < 0) {
        perror("Error fork1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        close(fd1[0]);            // Chiude lettura di pipe1
        dup2(fd1[1], STDOUT_FILENO); // Redirige stdout a scrittura di pipe1
        close(fd1[1]);            // Chiude scrittura di pipe1

        // Chiude pipe2
        close(fd2[0]);
        close(fd2[1]);

        execlp("ls", "ls", "-al", NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    // Secondo figlio - esegue `sort -R`
    if ((pid2 = fork()) < 0) {
        perror("Error fork2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        close(fd1[1]);            // Chiude scrittura di pipe1
        dup2(fd1[0], STDIN_FILENO); // Redirige stdin a lettura di pipe1
        close(fd1[0]);            // Chiude lettura di pipe1

        close(fd2[0]);            // Chiude lettura di pipe2
        dup2(fd2[1], STDOUT_FILENO); // Redirige stdout a scrittura di pipe2
        close(fd2[1]);            // Chiude scrittura di pipe2

        execlp("sort", "sort", "-R", NULL);
        perror("execlp sort");
        exit(EXIT_FAILURE);
    }

    // Terzo figlio - esegue `wc -w`
    if ((pid3 = fork()) < 0) {
        perror("Error fork3");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0) {
        close(fd1[0]); // Chiude pipe1
        close(fd1[1]);

        close(fd2[1]);            // Chiude scrittura di pipe2
        dup2(fd2[0], STDIN_FILENO); // Redirige stdin a lettura di pipe2
        close(fd2[0]);            // Chiude lettura di pipe2

        execlp("wc", "wc", "-w", NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    // Chiusura delle pipe nei processi padre
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    // Attesa che tutti i processi figli terminino
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}
