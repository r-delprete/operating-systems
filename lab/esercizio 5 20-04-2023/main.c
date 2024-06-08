#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int fibonacci(int n) {
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n = 12; // Numero di Fibonacci da calcolare
    int status; // Variabile per lo stato di terminazione del processo figlio

    pid_t pid1 = fork(); // Creazione del primo processo figlio

    if (pid1 == 0) { // Codice eseguito dal primo processo figlio
        int result = fibonacci(n); // Calcolo della sequenza di Fibonacci
        exit(result); // Terminazione del processo figlio con il risultato come codice di terminazione
    } else if (pid1 > 0) { // Codice eseguito dal processo padre
        wait(&status); // Attesa della terminazione del primo processo figlio e acquisizione dello stato di terminazione

        if (WIFEXITED(status)) { // Verifica se il processo figlio è terminato correttamente
            int exitStatus = WEXITSTATUS(status); // Acquisizione del codice di terminazione del processo figlio

            if (exitStatus < 50) { // Se il codice di terminazione è minore di 50
                pid_t pid2 = fork(); // Creazione del secondo processo figlio

                if (pid2 == 0) { // Codice eseguito dal secondo processo figlio
                    execl("/bin/ls", "ls", "-al", "a.out", NULL); // Esecuzione del comando ls -al a.out
                    exit(0); // Terminazione del secondo processo figlio
                } else if (pid2 > 0) { // Codice eseguito dal processo padre
                    wait(NULL); // Attesa della terminazione del secondo processo figlio
                    printf("Secondo figlio terminato con successo.\n");
                } else {
                    perror("Errore nella creazione del secondo figlio.\n");
                    exit(1);
                }
            } else {
                printf("Il codice di terminazione del primo figlio è maggiore o uguale a 50.\n");
            }
        } else {
            printf("Il primo figlio non è terminato correttamente.\n");
        }
    } else {
        perror("Errore nella creazione del primo figlio.\n");
        exit(1);
    }

    return 0;
}
