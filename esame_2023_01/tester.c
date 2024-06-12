#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

struct msg_buffer
{
    long mtype;
    char mtext[255];
} message;

int main(int argc, char *argv[])
{
    int pid = atoi(argv[1]);
    printf("sending singal to %d... \n", pid);
    // if (kill(pid, SIGUSR2) == -1)
    // {
    //     printf("errore mandazione. PID non trovato\n");
    // }
    int key = ftok("log.txt", 1);
    int queueId = msgget(key, IPC_CREAT);
    message.mtype = 1;
    sprintf(message.mtext, "%d", getpid());
    msgsnd(queueId, &message, 255, 0);

    while (1)
    {
    }
    return 0;
}