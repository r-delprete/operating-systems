/* Scrivere un programma in ambiente LINUX che implementi, 
stile shell BASH, la seguente pipeline inserita da riga di comando:
ls -lF | sort -R */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1


int main(int argc, char *argv[]){

    pid_t pid1, pid2;
    int fd1[2];

    if(pipe(fd1) < 0){
        perror("pipe 1");
        exit(EXIT_FAILURE);
    }


    if((pid1 = fork()) < 0){
        perror("fork1");
        exit(EXIT_FAILURE);
    }

    if(pid1 == 0){
        close(fd1[READ]);
        dup2(fd1[WRITE], STDOUT_FILENO);
        close(fd1[WRITE]);

        execlp("ls", "ls", "-lF", NULL);
        perror("execlp ls -lF");
        exit(EXIT_FAILURE);
    }

     if((pid2 = fork()) < 0){
        perror("fork2");
        exit(EXIT_FAILURE);
    }

    if(pid2 == 0){
        close(fd1[WRITE]);
        dup2(fd1[READ], STDIN_FILENO);
        close(fd1[READ]);

        execlp("sort", "sort", "-R", NULL);
        perror("execlp sort -R");
        exit(EXIT_FAILURE);
    }

    close(fd1[READ]);
    close(fd1[WRITE]);

    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);


    return 0;
}