/*
Scrivere un’applicazione che data una stringa come argomento ne stampa a video la
lunghezza, ad esempio:
./lengthof "Questa frase ha 28 caratteri"
deve restituire a video il numero 28
senza usare strlen.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Errore. Fornire una stringa\n");
        exit(1);
    }
    int counter = 0;
    while (argv[1][counter] != '\0')
        counter++;
    printf("La stringa ha %d caratteri\n", counter);
    return 0;
}