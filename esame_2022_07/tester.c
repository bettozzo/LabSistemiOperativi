#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/msg.h>

struct mymsg
{
    long mtype;      /* Message type. */
    char mtext[100]; /* Message text. */
} message;

int main(int argc, char *argv[])
{
    mkfifo("./fifo.txt", S_IRUSR | S_IWUSR);

    int fd = open("./fifo.txt", O_WRONLY);
    int n = atoi(argv[1]);
    char buffer[100];

    for (int i = 0; i < n; i++)
    {
        printf("inserito %d\n", i);
        sprintf(buffer, "%d", i);
        write(fd, buffer, strlen(buffer) + 1);
    }
    close(fd);
}