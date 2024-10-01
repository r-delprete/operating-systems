#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(){
    pid_t pid;
    int fd[2];
    pipe(fd);
    if((pid=fork())==0){ //figlio
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ls","ls", "-lF",NULL);
        perror("execlp");
        exit(0);
    }else{
        pid=fork();

        if(pid==0){
            close(fd[1]);
            dup2(fd[0],0);
            execlp("wc","wc","-c",NULL);
            perror("execlp");
        exit(0);
        }else{
            close(fd[0]);
            close(fd[1]);

            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}