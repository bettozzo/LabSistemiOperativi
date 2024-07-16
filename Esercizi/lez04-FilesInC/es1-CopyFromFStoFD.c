/*
Crea un’applicazione che copia il contenuto di un file,
leggendolo con i file streams e
scrivendolo con i file descriptors
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    FILE *srcFile = fopen("./src_dummy_file.txt", "r");

    if (srcFile == NULL)
    {
        printf("Errore apertura file. File non trovato\n");
        exit(1);
    }

    int dstFile = open("./dst_dummy_file.txt", O_WRONLY);

    while (!feof(srcFile))
    {
        char char_read[100];
        fgets(char_read, 100, srcFile);
        write(dstFile, char_read, strlen(char_read));
    }

    close(dstFile);
    fclose(srcFile);
    return 0;
}