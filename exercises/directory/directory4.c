#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
    DIR* dir;
    struct dirent *entry;
    struct stat info_file;

    char filename[256];
    int min=1000000;

    dir=opendir(argv[1]);


    printf("file: %s\n",argv[1]);

    while((entry=readdir(dir))!=NULL){
        printf("file:%s\n",entry->d_name);

        sprintf(filename,"%s/%s",argv[1],entry->d_name);

        stat(entry->d_name,&info_file);
        if(info_file.st_size<min){
            min=info_file.st_size;
        }

        if((info_file.st_mode & 00007)==00007){
            printf("Gli altr ahnno tutti i permessi\n");
        }else{
            printf("Gli altri non hanno tutti i permessi");
        }
        printf("File '%s' con grandezza: %ld\n",entry->d_name,info_file.st_size);

    }

    printf("File con grandezza minore: %d\n",min);
    closedir(dir);
}
