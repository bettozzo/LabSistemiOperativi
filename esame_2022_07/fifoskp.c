#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/msg.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "args: path n\n");
        return 2;
    }

    mkfifo(argv[1], S_IRUSR | S_IWUSR);
    int fd = open(argv[1], O_RDONLY);
    int n = atoi(argv[2]);
    char buffer[100];

    int i = 0;
    int atLeastN = 0;
    for (i = 0; i < 10; i++)
    {
        int byte = read(fd, buffer, 2);
        if (byte <= 0)
        {
            break;
        }

        if (i == n)
        {
            atLeastN = 1;
        }
        else
        {
            fprintf(stdout, "%s\n", buffer);
        }
    }
    close(fd);

    if (atLeastN == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}