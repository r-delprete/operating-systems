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


    dir=opendir(argv[1]);

    printf("File:%s\n",argv[1]);

    while((entry=readdir(dir))!=NULL){
        printf("file %s\n",entry->d_name);

        sprintf(filename,"%s/%s",argv[1],entry->d_name);

        stat(entry->d_name,&info_file);
        printf("UID: %d, dimensione: %ld\n",info_file.st_uid,info_file.st_size);
        printf("Permessi:%d\n",info_file.st_mode);
    }        
}
