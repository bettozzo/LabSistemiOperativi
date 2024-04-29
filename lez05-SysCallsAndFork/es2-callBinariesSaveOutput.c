/*
Avendo come argomenti dei “binari”, si eseguono con exec ciascuno in un sottoprocesso
salvando i flussi di stdout e stderr in un unico file
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int outfile = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(outfile, 1); // Redirect stdout to outputfile
    dup2(outfile, 2); // Redirect stderr to outputfile

    if (argc == 1)
    {
        printf("Passare dei binari da eseguire\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (fork() == 0) // child
        {
            fprintf(stderr, "***Eseguo eseguibile numero %d\n", i);
            execl(argv[i], "", NULL);
            printf("\n");
        }
        else // parent
        {
        }
    }
    while (wait(NULL) > 0)
    {
    } // Wait for all children
    close(outfile);
    return 0;
}