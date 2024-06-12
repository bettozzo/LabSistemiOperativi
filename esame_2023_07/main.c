#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer
{
    long mtype;
    char mtext[255];
} message;

int parentPid = -1;

void sigHandler(int signo, siginfo_t *info, void *ctx)
{
    printf("Reinvio signale %d al mittente %d", signo, info->si_pid);
    kill(info->si_pid, signo);
}

void sigWinchHandler(int signo, siginfo_t *info, void *empty)
{
    message.mtype = getpid();
    message.mtext = "CIAOOO";

    int key = ftok(argv[2] + parentPid, 0);
    int queueId = msgget(key, 0777 | IPC_CREAT);

    msgsnd(queueId, &message, 255, 0);
}

int checkArgs(int argc, char *argv[])
{
    // binario eseguito + 3 args
    if (argc != 4)
    {
        fprintf(stderr, "Pochi parametri\n");
        return 0;
    }

    // controllo di primo parametro, n
    int n = atoi(argv[1]);
    if (n < 1 || n > 10)
    {
        fprintf(stderr, "Parametro 'n' non valido. Deve essere un numero tra 1 e 10\n");
        return 0;
    }
    // controllo del secondo parametro, filePath
    char *filePath = argv[2];
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File non esiste\n");
        return 0;
    }
    fclose(file);

    // controllo del terzo parametro, pid
    int pidInput = atoi(argv[3]);
    if (kill(pidInput, 0) == -1)
    {
        fprintf(stderr, "Pid non esiste\n");
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    if (!checkArgs(argc, argv))
    {
        return 1;
    }

    int n = atoi(argv[1]);
    int pidInput = atoi(argv[3]);

    int isChild = 0;
    parentPid = getpid();

    int fd = open(argv[2], O_RDONLY);

    for (int i = 0; i < n; i++)
    {
        if (fork() == 0)
        {
            isChild = 1;
            struct sigaction sa1;                     // Define sigaction struct
            struct sigaction sa2;                     // Define sigaction struct
            sa1.sa_sigaction = sigHandler;            // Assign handler to struct field
            sa2.sa_sigaction = sigHandler;            // Assign handler to struct field
            sa1.sa_flags = SA_RESETHAND | SA_SIGINFO; // Initialise flags
            sa2.sa_flags = SA_SIGINFO;                // Initialise flags
            sigemptyset(&sa1.sa_mask);                // Define an empty mask
            sigemptyset(&sa2.sa_mask);                // Define an empty mask
            sigaction(SIGUSR1, &sa1, NULL);
            sigaction(SIGUSR2, &sa2, NULL);
            kill(pidInput, SIGTERM);
            break;
        }
    }

    if (isChild)
    {
        struct sigaction s;
        s.sa_sigaction = sigWinchHandler;
        s.sa_flags = SA_SIGINFO;
        sigemptyset(&s.sa_mask);
        sigaction(SIGWINCH, &s, NULL);
    }
    else
    {
        while (wait(NULL) > 0)
            ;
    }

    while (1)
    {
    };
    return 0;
}