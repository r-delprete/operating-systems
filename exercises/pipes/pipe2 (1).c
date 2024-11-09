/* Scrivere un programma in ambiente linux che implementi,
stile shell BASH, la seguente pipeline inserita da riga
di comando: Is -IF | wc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>




int main(){
    pid_t pid1,pid2;

    int fd1[2];



    if((pipe(fd1)) == -1){
        perror("Eerrore nella pipe\n");
        exit(EXIT_FAILURE);
    }


    if ((pid1 = fork()) == -1){
        perror("Eerrore nelal fork pid1\n");
        exit(EXIT_FAILURE);
    }

    if(pid1 == 0){
        close (fd1[0]);
        dup2(fd1[1],STDOUT_FILENO);
        close(fd1[1]);

        execlp("ls","ls","-lF",NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);

    }
    

    pid2 = fork();

    if(pid2 == 0){
        close(fd1[1]);
        dup2(fd1[0],STDIN_FILENO);
        close(fd1[0]);

        execlp("wc","wc",NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);

    }

    close(fd1[0]);
    close(fd1[1]);

    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);


    return 0;
}