/*
Dati due eseguibili come argomenti del tipo ls e wc si eseguono in due processi distinti: il primo
deve generare uno stdout redirezionato su un file temporaneo, mentre il secondo deve essere
lanciato solo quando il primo ha finito leggendo lo stesso file come stdin.
Ad esempio ./main ls wc deve avere lo stesso effetto di ls | wc.
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

    if (argc == 1)
    {
        fprintf(stderr, "Passare dei binari da eseguire\n");
        exit(1);
    }

    char *argList[2];
    if (fork() == 0) // child
    {
        argList[0] = argv[1];
        argList[1] = NULL;
        execvp(argList[0], argList);
    }
    else // parent
    {
        while (wait(NULL) > 0)
        {
        } // Wait for all children

        argList[0] = argv[2];
        argList[1] = NULL;

        dup2(outfile, 0); // Redirect stdinput to tmpFile
        lseek(outfile, 0, SEEK_SET);

        execvp(argList[0], argList);
    }
    close(outfile);
    return 0;
}