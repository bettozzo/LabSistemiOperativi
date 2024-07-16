#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MAX_STRINGS 100

char path[MAX_STRINGS];

void signalLogger(int signo, int sender)
{
    if (fork() == 0)
    {
        sleep(3);
        int file = open(path, O_WRONLY | O_APPEND);
        char buffer[MAX_STRINGS];
        sprintf(buffer, "%d-%d\n", sender, signo);
        write(file, buffer, strlen(buffer));
        close(file);
        exit(0);
    }
}

void stopper(int signo, siginfo_t *info, void *empty)
{
    int file = open(path, O_WRONLY | O_APPEND);
    write(file, "stop\n\0", 6);
    close(file);
}

void repeater(int signo, siginfo_t *info, void *empty)
{
    printf("ricevuto SIGUSR1, sono %d\n", getpid());
    kill(info->si_pid, SIGUSR1);
    signalLogger(signo, info->si_pid);
}
void relay(int signo, siginfo_t *info, void *empty)
{
    if (fork() == 0)
    {
        printf("ricevuto SIGUSR2, sono %d\n", getpid());
        kill(info->si_pid, SIGUSR1);
        exit(0);
    }
    else
    {
        signalLogger(signo, info->si_pid);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "errore arg\n");
        fflush(stderr);
        return 1;
    }
    printf("padre pid: %d\n", getpid());
    sprintf(path, "%s", argv[1]);

    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = repeater;
    sigemptyset(&sa1.sa_mask);
    sigaction(SIGUSR1, &sa1, NULL);

    struct sigaction sa2;
    sa2.sa_flags = SA_SIGINFO;
    sa2.sa_sigaction = relay;
    sigemptyset(&sa2.sa_mask);
    sigaction(SIGUSR2, &sa2, NULL);

    struct sigaction sa3;
    sa3.sa_flags = SA_SIGINFO;
    sa3.sa_sigaction = stopper;
    sigemptyset(&sa3.sa_mask);
    sigaction(SIGINT, &sa3, NULL);
    while (1)
    {
        pause();
    }
    return 0;
}