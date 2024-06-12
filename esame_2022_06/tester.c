#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/ipc.h>

struct messageStruct
{
    long mtype;
    char mtext[100];
} msg;

int getCoda(char *nome)
{
    int key = ftok(nome, 1);
    if (key == -1)
    {
        open(nome, O_WRONLY | O_CREAT);
        key = ftok(nome, 1);
    }
    return msgget(key, IPC_CREAT);
}

int main(int argc, char *argv[])
{
    int queueId = getCoda("coda.txt");
    fprintf(stdout, "coda: %d\n", queueId);

    // case "put":
    for (int i = 0; i < 100; i++)
    {
        msg.mtype = 1;
        sprintf(msg.mtext, "%d", i);
        msgsnd(queueId, &msg, sizeof(msg.mtext), 0);
    }
    // case "get":
    // msgrcv(queueId, &msg, 100, 0, 0);
    // fprintf(stdout, "%s\n", msg.mtext);

    return 0;
}