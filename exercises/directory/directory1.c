#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[]){
DIR *dp;
struct dirent *dirp;
struct stat info_file;

if(argc!=2){
    printf("C'e visogno della directory");
    exit(0);
}

if((dp=opendir(argv[1]))== NULL){
    printf("can't open %s",argv[1]);
    exit(0);
}

while((dirp=readdir(dp))!= NULL)
printf("%s",dirp->d_name);

closedir(dp);
exit(0);
}