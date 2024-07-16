/*
Impostare una comunicazione bidirezionale tra due processi con due livelli di
complessità:
    ○ Alterna due scambi (P1 → P2, P2 → P1, P1 → P2, P2 → P1)
    ○ Mantenendo una comunicazione fino alla ricezione di un carattere di terminazione (da decidere)
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main()
{
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    int isChild = !fork();
    char buffer[50];
    if (isChild)
    {
        close(pipe1[WRITE]);
        close(pipe2[READ]);
        char msg[18] = "Messaggio da Child";
        while (1)
        {
            scanf("%s", &msg);
            if (strcmp("FINE", msg) == 0)
            {
                break;
            }
            write(pipe2[WRITE], msg, strlen(msg) + 1);
            read(pipe1[READ], &buffer, 50);
            printf("Messaged recevied to child: %s\n", buffer);
        }
        close(pipe1[READ]);
        close(pipe2[WRITE]);
    }
    else
    {
        close(pipe1[READ]);
        close(pipe2[WRITE]);
        char msg[19] = "Messaggio da Parent";
        while (1)
        {
            write(pipe1[WRITE], msg, strlen(msg) + 1);
            read(pipe2[READ], &buffer, 50);
            if (strcmp("FINE", buffer) == 0)
            {
                break;
            }
            printf("Messaged recevied to parent: %s\n", buffer);
        }
        close(pipe1[WRITE]);
        close(pipe2[READ]);
    }
}
