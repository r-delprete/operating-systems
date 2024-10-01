/* Creare un link simbolico con il comando ln -s.
Scrivere un programma che selezioni il link nella directory
 e stampi a video il nome del file. */


 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    char cwd[1024]; // Buffer per memorizzare il percorso corrente della directory
    struct stat buf; // Struct per memorizzare le informazioni sul file
    struct dirent *dir; // Struct per memorizzare le informazioni sull'entry della directory
    DIR *dp; // Puntatore alla directory

    // Ottieni la directory corrente
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd error"); // Stampa un messaggio di errore se getcwd fallisce
        exit(EXIT_FAILURE); // Esce dal programma con un codice di errore
    }

    // Apri la directory corrente
    if ((dp = opendir(cwd)) == NULL) {
        perror("opendir error"); // Stampa un messaggio di errore se opendir fallisce
        exit(EXIT_FAILURE); // Esce dal programma con un codice di errore
    }

    // Leggi ogni voce della directory
    while ((dir = readdir(dp)) != NULL) {
        // Ottieni informazioni sul file
        if (lstat(dir->d_name, &buf) < 0) {
            perror("lstat error"); // Stampa un messaggio di errore se lstat fallisce
            continue; // Salta alla prossima voce della directory
        }

        // Controlla se è un link simbolico
        if (S_ISLNK(buf.st_mode)) {
            printf("Trovato il link: %s\n", dir->d_name); // Stampa il nome del link simbolico
        }
    }

    // Chiudi la directory
    closedir(dp); // Chiude la directory aperta
    return 0; // Termina il programma con un codice di successo
}
