/*
Crea un’applicazione che copia il contenuto di un file,
leggendolo con i file descriptors e
scrivendolo con i file streams
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    int srcFile = open("./src_dummy_file.txt", O_RDONLY);

    if (srcFile == -1)
    {
        printf("Errore apertura file. File non trovato\n");
        exit(1);
    }

    FILE *dstFile = fopen("./dst_dummy_file.txt", "w");

    int byte_read = 0;
    do
    {

        char char_read[100];
        byte_read = read(srcFile, char_read, 100);
        char_read[byte_read] = 0;
        fputs(char_read, dstFile);
    } while (byte_read > 0);

    fclose(dstFile);
    close(srcFile);
    printf("\n\nDONE!!!!\n\n");
    return 0;
}