/* Scrivere un programma in C e Posix sotto Linux che,
 presa in input un file completo di restituisca lo UID
 del proprietario, la grandezza del file e tutti i permessi associati a tale file.
*/


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


void print_permissions(mode_t mode){
    char permissions[9];

    if(mode & S_IRUSR)
    permissions[0] = 'r';
    if(mode & S_IWUSR)
    permissions[1] = 'w';
    if(mode & S_IXUSR)
    permissions[2] = 'x';


    if(mode & S_IRGRP)
    permissions[3] = 'r';
    if(mode & S_IWGRP)
    permissions[4] = 'w';
    if(mode & S_IXGRP)
    permissions[5] = 'x';


    if(mode & S_IROTH)
    permissions[6] = 'r';
    if(mode & S_IWOTH)
    permissions[7] = 'w';
    if(mode & S_IXOTH)
    permissions[8] = 'x';


    printf("Permessi : %s\n", permissions);
}


int main(int argc, char *argv[]){

    DIR *dir;
    struct stat file_stat;
    struct dirent entry;

    if(argc != 2){
        printf(stderr, "Uso: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   if(stat(argv[1], &file_stat)==-1){
    perror("file stat");
    exit(EXIT_FAILURE);
   }

   printf("UID del propretario %d\n", file_stat.st_uid);
   printf("Dieìmesnione del file: %d bytes\n", file_stat.st_size);

   print_permissions(file_stat.st_mode); 

   return 0;
}