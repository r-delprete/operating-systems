#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "../libs/apue.h"

int main()
{
    int file_descriptor = open("alfabeto.txt", O_RDONLY);
    char buffer[100];
    int n_read;
    if (file_descriptor == -1)
    {
        perror("open error");
        exit(-1);
    }

    while ((n_read = read(file_descriptor, buffer, sizeof(buffer))) > 0)
    {
        if ((write(STDOUT_FILENO, buffer, n_read)) == -1)
        {
            close(file_descriptor);
            exit(-1);
        }
    }

    write(STDOUT_FILENO, "\n", 1);
    close(file_descriptor);

    return 0;
}