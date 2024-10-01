#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Funzione ricorsiva per stampare l'albero dei processi
void stampa_albero(int glob) {
    printf("PID = %d, glob = %d\n", getpid(), glob);
    fflush(stdout);

    int pid = fork();
    if (pid < 0) {
        // Errore nella fork
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Processo figlio
        if (glob > 0) {
            glob--;
            stampa_albero(glob);
        }
        exit(EXIT_SUCCESS);
    } else {
        // Processo padre
        wait(NULL);
    }
}

int main() {
    int glob = 6;

    // Stampa il processo padre iniziale
    printf("Processo padre: PID = %d, glob = %d\n", getpid(), glob);

    // Chiamata alla funzione ricorsiva per stampare l'albero dei processi
    stampa_albero(glob);

    return 0;
}
