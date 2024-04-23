/*
Realizzare funzioni per stringhe
- char *stringrev(char * str) (inverte ordine caratteri)
- int stringpos(char * str, char chr) (cerca chr in str e restituisce la posizione)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stringrev(char *str);
int stringpos(char *str, char chr);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Errore. Fornire almeno una stringa\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        printf("BASE: %s\tReversed:%s\t, carattere 'a' in posizione: %d\n", argv[i], stringrev(argv[i]), stringpos(argv[i], 'a'));
    }
    return 0;
}
char *stringrev(char *str)
{
    char *reversed = (char *)malloc(sizeof(char) * strlen(str));
    reversed[strlen(str)] = '\0';
    for (int i = 0; i < strlen(str); i++)
    {
        reversed[strlen(str) - i - 1] = str[i];
    }
    return reversed;
}

int stringpos(char *str, char chr)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == chr)
        {
            return i;
        }
    }
    return -1;
}