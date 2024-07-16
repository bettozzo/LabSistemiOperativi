/*
Avendo come argomenti dei “binari”, si eseguono con exec ciascuno in un sottoprocesso
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Passare dei binari da eseguire\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (fork() == 0) // child
        {
            execl(argv[i], "", NULL);
        }
        else // parent
        {
        }
    }
    return 0;
}