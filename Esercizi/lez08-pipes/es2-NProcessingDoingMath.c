/*
Creare un programma che prenda come argomento 'n' il numero di figli da
generare. Ogni figlio creato comunicherà al genitore (tramite pipe) un numero
casuale e il genitore calcolerà la somma di tutti questi numeri, inviandola a
stdout.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Inserire Numero di figli che si vogliono far partire\n");
        exit(1);
    }

    int fd[2];
    pipe(fd);
    int pipe_read = fd[0];
    int pipe_write = fd[1];

    char buffer[50];
    int total = 0;
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        int isChild = !fork();
        if (isChild)
        {
            srand(time(NULL) ^ getpid() << 16);
            int numberGenerated = rand() % 10 + 1;
            char number[4];
            sprintf(number, "%d", numberGenerated); // Converts int to string

            printf("%d\n", numberGenerated);
            write(pipe_write, number, strlen(number) + 1);
            close(pipe_write);
            return 0;
        }
        else
        {
            read(pipe_read, buffer, 50);
            total += atoi(buffer);
        }
    }
    printf("Total is %d\n", total);
    close(pipe_read);
    close(pipe_write);
}
