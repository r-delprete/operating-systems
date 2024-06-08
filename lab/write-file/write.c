#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "../lib/apue.h"

int main() {
int file_descriptor, n_write;
char *data = "ABCDEFGHILMNOPQRSTUVZ";

file_descriptor = open("alfabeto.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

if (file_descriptor == -1){
    perror("errore apertura");
    exit(-1);
}

n_write = write(file_descriptor, data, strlen(data));

if (n_write == -1 ) {
    perror("errore scrittura");
    exit(-1);
}

close(file_descriptor);

return 0;
}
