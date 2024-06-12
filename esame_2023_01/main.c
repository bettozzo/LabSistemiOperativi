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

char filePath[255];
void handlerReapeter(int signo, siginfo_t *info, void *empty)
{

    fprintf(stdout, "recieved %d from: %d\n", signo, info->si_pid);
    kill(info->si_pid, SIGUSR1);

    if (fork() == 0)
    {
        sleep(3);
        int fd = open(filePath, O_WRONLY | O_APPEND);
        char buf[255];
        sprintf(buf, "%d-%d\n", info->si_pid, signo);
        write(fd, buf, strlen(buf));
        close(fd);
        kill(getpid(), SIGTERM);
    }
}

void handlerRelay(int signo, siginfo_t *info, void *empty)
{

    if (fork() == 0)
    {
        fprintf(stdout, "recieved %d from: %d. Sending it back from: %d\n", signo, info->si_pid, getpid());
        kill(info->si_pid, SIGUSR2);

        sleep(3);
        int fd = open(filePath, O_WRONLY | O_APPEND);
        char buf[255];
        sprintf(buf, "%d-%d\n", info->si_pid, signo);
        write(fd, buf, strlen(buf));
        close(fd);

        kill(getpid(), SIGTERM);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "passare file\n");
        return 1;
    }
    char str[3] = "./";
    strcat(str, argv[1]);
    strcpy(filePath, str);
    struct sigaction sa1;
    sa1.sa_sigaction = handlerReapeter;
    sa1.sa_flags = SA_SIGINFO;
    sigemptyset(&sa1.sa_mask);
    sigaction(SIGUSR1, &sa1, NULL);

    struct sigaction sa2;
    sa2.sa_sigaction = handlerRelay;
    sa2.sa_flags = SA_SIGINFO;
    sigemptyset(&sa2.sa_mask);
    sigaction(SIGUSR2, &sa2, NULL);

    printf("processo: %d\n", getpid());

    int key = ftok(argv[1], 1);
    int queueId = msgget(key, IPC_CREAT);
    while (1)
    {
        int bytes = msgrcv(queueId, &message, 255, 0, IPC_NOWAIT);
        if (bytes > 0)
        {
            printf("messaggio ricevuto: %s\n", message.mtext);
            kill(atoi(message.mtext), SIGALRM);
        }
    }

    return 0;
}