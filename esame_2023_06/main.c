#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

struct queueMsgStruct
{
    long category;
    char word[255];
} queueMsg;

int canContinue = 1;

void handler(int signo, siginfo_t *info, void *empty)
{
    canContinue = 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "dare file come arg\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");

    int key = ftok(argv[1], 1);
    int queueId = msgget(key, 0777 | IPC_CREAT);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigaction(SIGUSR1, &sa, NULL);

    printf("pid:%d\n", getpid());

    while (!feof(file))
    {
        if (canContinue == 1)
        {
            char buf[3][255];
            fscanf(file, "%s %s %s", buf[0], buf[1], buf[2]);
            printf("comando: %s\n", buf[0]);
            if (strcmp("kill", buf[0]) == 0)
            {
                kill(atoi(buf[1]), atoi(buf[2]));
            }
            else if (strcmp("queue", buf[0]) == 0)
            {
                queueMsg.category = atoi(buf[1]);
                strcpy(queueMsg.word, buf[2]);
                msgsnd(queueId, &queueMsg, sizeof(queueMsg.word), 0);
            }
            else if (strcmp("fifo", buf[0]) == 0)
            {
                mkfifo(buf[1], S_IRUSR | S_IWUSR);
                int fd = open(buf[1], O_WRONLY);
                write(fd, buf[2], strlen(buf[2]));
            }
            canContinue = 0;
        }
    }
    fclose(file);
    return 0;
}