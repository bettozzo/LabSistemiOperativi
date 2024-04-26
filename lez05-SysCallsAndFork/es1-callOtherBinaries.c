/*
Avendo come argomenti dei “binari”, si eseguono con exec ciascuno in un sottoprocesso
*/

#include <stdio.h>
#include <io.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Passare dei binari da eseguire\n");
        exit(1);
    }
    for (int i = 0; i < argc; i++)
    {
        if (fork() == 0) // child
        {
            execlp(argv[i], NULL);
        }
        else // parent
        {
        }
    }
    return 0;
}