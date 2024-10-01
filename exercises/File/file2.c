/* Scrivere un programma C che stampi a video Il contenuto di un file a byte alterni 
(ovvero un carattere si ed uno no). Il nome del file viene passato come parametro
 su riga di comando. Esempio di esecuzione:
$-/a.out <nome_file.txt>
Se «nome_file.txt> contiene la scritta "FORZA NAPOLII" allora il programma scriverà.
Output:
FRANPLI */


#include <stdio.h>   // Include per funzioni standard di input/output
#include <stdlib.h>  // Include per funzioni standard di libreria
#include <unistd.h>  // Include per definizioni POSIX, anche se non utilizzate qui
#include <fcntl.h>   // Include per le definizioni di file control, anche se non utilizzate qui

int main(int argc, char *argv[]) {
    // Verifica che il numero di argomenti passati sia esattamente 2 (il nome del programma e il nome del file)
    if (argc != 2) {
        printf("Errore nella lettura da riga di comando %s\n", argv[0]);
        return 1;
    }

    // Tenta di aprire il file in modalità lettura ("r")
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file\n");  // Stampa un messaggio di errore se l'apertura fallisce
        return 1;
    }

    int ch;     // Variabile per memorizzare il carattere letto
    int skip = 0;  // Flag per determinare se saltare il carattere corrente

    // Legge il file un carattere alla volta fino alla fine del file (EOF)
    while ((ch = fgetc(file)) != EOF) {
        if (!skip) {   // Se il flag 'skip' è 0, stampa il carattere
            putchar(ch);
        }
        skip = !skip;  // Inverte il valore del flag 'skip' (alternare tra 0 e 1)
    }

    // Chiude il file
    fclose(file);
    return 0;
}
