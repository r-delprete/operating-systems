#include "../libs/apue.h"

int main()
{
    int sum = 0, i = 0, value;
    char buffer[1000];

    memset(buffer, 0, sizeof(buffer));

    while (read(STDIN_FILENO, buffer, 4) > 0)
    {
        value = atoi(buffer);
        if (value == -1)
            break;
        else
        {
            i++;
            sum += value;
            if (i == 2)
            {
                sprintf(buffer, "sum = %d", sum);
                buffer[strlen(buffer)] = '\n';
                write(STDOUT_FILENO, buffer, strlen(buffer));
                memset(buffer, 0, sizeof(buffer));
                sum = i = 0;
            }
        }
    }
}