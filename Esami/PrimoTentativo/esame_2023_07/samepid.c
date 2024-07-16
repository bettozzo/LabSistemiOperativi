#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    printf("PID: %d\n", getpid());
    while (1)
    {
    }
    return 0;
}