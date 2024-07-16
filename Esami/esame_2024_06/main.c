#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_STRINGS 100

struct msgBox
{
    long type;
    char text[MAX_STRINGS];
} msg;

char password[MAX_STRINGS];
long otherPid;

void handlerSigtstp(int signo, siginfo_t *info, void *empty)
{
    printf("Arrivato SIGTSTP\n");
    exit(0);
}

void *threadFunz(void *_username)
{
    printf("ciao\n");
    fflush(stdout);
    char *username = (char *)_username;
    mkfifo("./login.fifo", S_IRUSR | S_IWUSR);
    int fileLogin = open("./login.fifo", O_RDONLY);
    char buffer[MAX_STRINGS];

    // punto 8
    int key = ftok("./login.fifo", 51);
    int queueId = msgget(key, 0777 | IPC_CREAT);
    while (1)
    {
        int r = read(fileLogin, buffer, MAX_STRINGS);
        if (r > 0)
        {
            buffer[r] = 0;
            if (strcmp(password, buffer) == 0)
            {
                fprintf(stdout, "OK\n");
                fprintf(stderr, "%s logged in\n", username);
                fflush(stdout);
                fflush(stderr);
            }
            else
            {
                fprintf(stdout, "NO\n");
                fflush(stdout);
            }
            // punto 9
            msg.type = otherPid;
            sprintf(msg.text, "%s", password);
            msgsnd(queueId, &msg, strlen(msg.text), 0);
        }
    }
}

int main()
{

    // punto 1
    printf("parent pid: %d\n", getpid());
    FILE *fileCredentials = fopen("./credentials.txt", "r");
    char username[MAX_STRINGS];
    fscanf(fileCredentials, "%s\n%s", username, password);

    FILE *fileSecret = fopen("./secrets.txt", "w");
    fprintf(fileSecret, "%s\n%s", username, password);
    fflush(fileSecret);

    // punto 2
    // remove("./credentials.txt");

    // punto 3
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handlerSigtstp;
    sigaction(SIGTSTP, &sa, NULL);

    // punto 4
    mkfifo("./authenticator.fifo", 007);
    int fileAuth = open("./authenticator.fifo", O_WRONLY);
    char buffer[MAX_STRINGS];
    sprintf(buffer, "%d", getpid());
    write(fileAuth, buffer, sizeof(buffer));
    close(fileAuth);

    // punto 6 - 7
    pthread_t threadId;
    pthread_create(&threadId, NULL, threadFunz, (void *)username);

    // punto 5
    mkfifo("./clients.fifo", S_IRUSR | S_IWUSR);
    int fileClients = open("./clients.fifo", O_RDONLY);
    while (1)
    {
        if (read(fileClients, buffer, MAX_STRINGS) > 0)
        {
            otherPid = atoi(buffer);
            kill(otherPid, SIGUSR1);
        }
    }
    return 0;
}