#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

// Funzione per stampare i permessi del proprietario
void print_permissions(mode_t mode) {
    char permissions[4];
    permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = '\0';
    printf("Permissions: %s\n", permissions);
}

int main(int argc, char *argv[]) {
    DIR *dp;
    struct stat file_stat;
    struct dirent *entry;
    char full_path[1024];

    // Verifica se l'utente ha fornito il corretto numero di argomenti
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Apri la directory specificata
    dp = opendir(argv[1]);
    if (dp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Leggi ogni voce nella directory
    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) { // Controlla se è un file regolare
            // Crea il percorso completo al file
            snprintf(full_path, sizeof(full_path), "%s/%s", argv[1], entry->d_name);

            // Ottieni le informazioni sul file
            if (stat(full_path, &file_stat) == -1) {
                perror("stat");
                continue;
            }

            // Controlla se il numero di hard link è maggiore di 1
            if (file_stat.st_nlink > 1) {
                printf("File: %s\n", entry->d_name);
                // Stampa i permessi del proprietario
                print_permissions(file_stat.st_mode);
            }
        }
    }

    // Chiudi la directory
    closedir(dp);
    return 0;
}
