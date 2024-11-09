#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define read 0
#define write 1

int main(int argc, char const *argv[]){
    pid_t pid;
    int fd[2];
    pipe(fd);
    if((pid=fork())==0){
        close(fd[read]);
        dup2(fd[write],write);
        execlp("ls","ls","-l",NULL);
        perror(execlp);
        exit(0);
    }else{
        pid=fork();

        if(pid==0){
            close(fd[write]);
            dup2(fd[read],read);
            execlp("wc","wc","-c",NULL);
            perror(execlp);
            exit(0);
        }else{
            close(fd[read]);
            close(fd[write]);

            wait(NULL);
            wait(NULL);
        }
    }
return 0;
}