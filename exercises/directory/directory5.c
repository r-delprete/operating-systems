/* Si realizzi un programma in C e Posi sotto Linux che
 data una directory da riga di comando, ne elenchi contenuto e,
  individuati i link simbolici, indichi se il file puntato ha i
   diritti di scrittura per il proprietario e per gruppo e stampi la dimensione.*/


   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <dirent.h>
   #include <sys/stat.h>
   #include <string.h>
   #include <sys/types.h>

void print_permissions(const char *filename){
    struct stat sb;

    lstat(filename,&sb);

    printf("Permissions for %s: \n", filename);
    printf("Owner can write: %s\n", (sb.st_mode & S_IWUSR) ? "Yes" : "No");
    printf("Group can write: %s\n", (sb.st_mode & S_IWGRP) ? "Yes" : "No");
    printf("Size: %ld bytes\n", (long) sb.st_size);

}


   int main(int argc, char *argv[]){


    DIR *dir;
    struct stat buf;
    struct dirent *entry;

    if(argc!=2){
        printf("errore durante la ricezione da riga di comando");
        return 1;
    }


    dir = opendir(argv[1]);


    while((entry = readdir(dir)) != NULL){
        printf("%ls", entry->d_name);

        if(lstat(entry->d_name,&buf) < 0){
            perror("lstat error"); // Stampa un messaggio di errore se lstat fallisce
            continue; // Salta alla prossima voce della directory
        }

        // Controlla se è un link simbolico
        if (S_ISLNK(buf.st_mode)) {
            printf("Trovato il link: %s\n", entry->d_name); // Stampa il nome del link simbolico
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);
             print_permissions(path);
        }
    }

closedir(dir);
return 0;

   }