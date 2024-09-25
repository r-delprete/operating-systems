#include <stdio.h>
#include <fcntl.h>
#include "../libs/apue.h"

int main()
{
    int file_descriptor = open("alfabeto.txt", O_RDONLY);
    char buffer[100];
    int n_read, position = 0;
    if (file_descriptor == -1)
    {
        perror("open error");
        exit(-1);
    }

    while ((n_read = read(file_descriptor, buffer, 2)) > 0)
    {
        if ((write(STDOUT_FILENO, buffer, n_read)) == -1)
        {
            close(file_descriptor);
            exit(-1);
        }

        position = position + 5;

        if (lseek(file_descriptor, position, SEEK_SET) == -1)
        {
            perror("seek error");
            close(file_descriptor);
            exit(-1);
        }
    }

    write(STDOUT_FILENO, "\n", 1);
    close(file_descriptor);

    return 0;
}