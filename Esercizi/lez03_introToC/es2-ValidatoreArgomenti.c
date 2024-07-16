/*
Scrivere un’applicazione che definisce una lista di argomenti validi e legge quelli passati
alla chiamata verificandoli e memorizzando le opzioni corrette, restituendo un errore in
caso di un’opzione non valida.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Errore. Fornire serie di numeri\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        if (atoi(argv[i]) % 2 == 0)
        {
            printf("%d valido\n", atoi(argv[i]));
        }
        else
        {
            printf("Errore, %d non valido\n", atoi(argv[i]));
            exit(1);
        }
    }
    return 0;
}