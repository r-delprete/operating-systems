#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "../libs/apue.h"

int main()
{
    int file_descriptor = open("file.txt", O_RDONLY), n_read, position, count = 1;
    char buffer[100];

    if (file_descriptor == -1)
    {
        perror("open error");
        exit(-1);
    }

    if ((lseek(file_descriptor, 0, SEEK_END)) == -1)
    {
        perror("seek error");
        close(file_descriptor);
        exit(-1);
    }

    position = lseek(file_descriptor, 0, SEEK_CUR);
    position = position - count;

    if (lseek(file_descriptor, position, SEEK_SET) == -1)
    {
        perror("errore lseek");
        close(file_descriptor);
        exit(1);
    }

    while ((n_read = read(file_descriptor, buffer, 1)) > 0)
    {
        if ((write(STDOUT_FILENO, buffer, n_read)) == -1)
        {
            close(file_descriptor);
            exit(-1);
        }

        position--;

        if (position >= 0)
        {
            if (lseek(file_descriptor, position, SEEK_SET) == -1)
            {
                perror("seek error");
                close(file_descriptor);
                exit(-1);
            }
        }
        else
        {
            write(STDOUT_FILENO, "\n", 1);
            close(file_descriptor);
            exit(0);
        }
    }
}