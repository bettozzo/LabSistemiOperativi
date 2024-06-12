#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/ipc.h>

void main3args(char *nome, char *azione, char *pid);
void main4args(char *nome, char *azione, char *valore, char *pid);

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
    if (argc != 5 && argc != 4)
    {
        fprintf(stderr, "args\n");
        return 1;
    }

    char nome[32], azione[32], valore[32], pid[32];
    if (argc == 4)
    {
        strcpy(nome, argv[1]);
        strcpy(azione, argv[2]);
        strcpy(pid, argv[3]);
    }
    else
    {
        strcpy(nome, argv[1]);
        strcpy(azione, argv[2]);
        strcpy(valore, argv[3]);
        strcpy(pid, argv[4]);
    }
    main4args(nome, azione, valore, pid);
    return 0;
}

void main4args(char *nome, char *azione, char *valore, char *pid)
{

    int successo = 1;
    int queueId = -1;
    if (strcmp(azione, "new") == 0)
    {
        queueId = getCoda(nome);
        fprintf(stdout, "coda: %d\n", queueId);
    }
    else if (strcmp(azione, "put") == 0)
    {
        queueId = getCoda(nome);
        msg.mtype = 1;
        strcpy(msg.mtext, valore);
        msgsnd(queueId, &msg, sizeof(msg.mtext), 0);
    }
    else if (strcmp(azione, "get") == 0)
    {
        queueId = getCoda(nome);
        msgrcv(queueId, &msg, 100, 0, 0);
        fprintf(stdout, "%s\n", msg.mtext);
    }
    else if (strcmp(azione, "del") == 0)
    {
        queueId = getCoda(nome);
        msgctl(queueId, IPC_RMID, NULL);
        fprintf(stdout, "%s\n", msg.mtext);
    }
    else if (strcmp(azione, "emp") == 0)
    {
        while (1)
        {
            queueId = getCoda(nome);
            int bytes = msgrcv(queueId, &msg, 100, 0, 0);
            fprintf(stdout, "%s\n", msg.mtext);
            if (bytes <= 0)
            {
                break;
            }
        }
    }
    if (successo == 1)
    {
        kill(atoi(pid), SIGUSR1);
    }
    else
    {
        kill(atoi(pid), SIGUSR2);
    }
}
