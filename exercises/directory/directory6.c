#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

int main(int argc, char const *argv[]){
DIR* dir;
struct dirent *entry;
struct stat info_file;

char filename[256];

dir=opendir(argv[1]);

printf("file: %s",argv[1]);

while((entry=readdir(dir))!=NULL){
    printf("file: %s\n",entry->d_name);

    sprintf(filename,"%s/%s",argv[1],entry->d_name);

    stat(filename,&info_file);
 
    printf("Grandezza file %ld",info_file.st_size);

    if((info_file.st_mode & S_IRUSR) && (info_file.st_mode & S_IWUSR)){
        printf("\nIl propretario ha diritti di lettura e scrittura\n");
    }else{
        printf("Il propretario NON ha diritti di lettura e scrittura\n");
    }
    if((info_file.st_mode & S_IROTH) && (info_file.st_mode & S_IWOTH)){
        printf("Gli altri hanno diritti di lettura e scrittura\n");
    }else{
        printf("Gli altri NON hanno diritti di lettura e scrittura\n");

    }
    
}
closedir(dir);
return 0;
}