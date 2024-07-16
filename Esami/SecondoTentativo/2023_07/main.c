#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_STRINGS 100

int sigwhinchRicevuto = 0;

struct msgBox
{
    long type;
    char text[MAX_STRINGS];
} msg;

void sigusr1Funz(int signo, siginfo_t *info, void *empty)
{
    printf("ricevuto SIGUSR1, ora lo rinvio\n");
    fflush(stdout);
    kill(info->si_pid, SIGUSR1);
}
void sigusr2Funz(int signo, siginfo_t *info, void *empty)
{
    printf("ricevuto SIGUSR2, ora lo rinvio\n");
    fflush(stdout);
    kill(info->si_pid, SIGUSR2);
}

void sigwinchFunz(int signo, siginfo_t *info, void *empty)
{
    printf("ricevuto SIGWHINCH\n");
    sigwhinchRicevuto = 1;
}

int checkArgs(int n, char *path, int pid)
{

    if (n < 0 || n > 10)
    {
        fprintf(stderr, "errore numero\n");
        fflush(stderr);
        return 0;
    }

    if (access(path, F_OK) != 0)
    {
        fprintf(stderr, "errore file\n");
        fflush(stderr);
        return 0;
    }

    if (kill(pid, SIGUSR1) == -1)
    {
        fprintf(stderr, "errore pid\n");
        fflush(stderr);
        return 0;
    }

    return 1;
}
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "errore numero file\n");
        fflush(stderr);
        return 1;
    }
    int n = atoi(argv[1]);
    char path[MAX_STRINGS];
    sprintf(path, "%s", argv[2]);
    int otherPid = atoi(argv[3]);

    int valid = checkArgs(n, path, otherPid);
    if (valid == 0)
    {
        return 1;
    }
    int parentPid = getpid();
    printf("padre pid: %d\n", parentPid);
    int childPids[10];
    int pipes[10][2];
    int currentIndex = -1;
    for (int i = 0; i < n; i++)
    {
        pipe(pipes[i]);
        childPids[i] = fork();
        if (childPids[i] == 0)
        {
            currentIndex = i;
            // kill(otherPid, SIGTERM);
            close(pipes[i][1]);
            struct sigaction sa1;
            sa1.sa_sigaction = sigusr1Funz;
            sa1.sa_flags = SA_SIGINFO | SA_RESETHAND;
            sigemptyset(&sa1.sa_mask);
            sigaction(SIGUSR2, &sa1, NULL);

            struct sigaction sa2;
            sa2.sa_sigaction = sigusr2Funz;
            sa2.sa_flags = SA_SIGINFO;
            sigemptyset(&sa2.sa_mask);
            sigaction(SIGUSR2, &sa2, NULL);
            break;
        }
        else
        {
            close(pipes[i][0]);
            struct sigaction sa;
            sa.sa_sigaction = sigwinchFunz;
            sa.sa_flags = SA_SIGINFO;
            sigemptyset(&sa.sa_mask);
            sigaction(SIGWINCH, &sa, NULL);
        }
    }

    if (getpid() == parentPid)
    {
        while (1)
        {
            if (sigwhinchRicevuto == 1)
            {
                break;
            }
            pause();
        }
        int turn = 0;
        FILE *file = fopen(path, "r");
        char buffer[MAX_STRINGS];
        while (!feof(file))
        {
            fgets(buffer, MAX_STRINGS, file);
            if (buffer[strlen(buffer) - 1] == '\n')
            {
                buffer[strlen(buffer) - 1] = '\0';
            }
            write(pipes[turn][1], buffer, strlen(buffer));
            turn = (turn + 1) % n;
            sleep(1);
        }

        for (int i = 0; i < n; i++)
        {
            kill(childPids[i], SIGTERM);
            break;
        }
    }
    else
    {
        // child
        while (1)
        {
            int key = ftok(path, parentPid);
            int queueId = msgget(key, 0777 | IPC_EXCL);

            while (1)
            {
                if (read(pipes[currentIndex][0], msg.text, MAX_STRINGS) > 0)
                {
                    msg.type = getpid();
                    msgsnd(queueId, &msg, strlen(msg.text), 0);
                    fflush(stdout);

                    msgrcv(queueId, &msg, MAX_STRINGS, 0, 0);
                    printf(" sono child: %d. Messaggio ricevuto: %s\n", getpid(), msg.text);
                }
            }
        }
    }

    return 0;
}