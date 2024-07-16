#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Is the number 2 even?\n%s\n", 2 % 2 == 0 ? "Yes" : "No");
    return 0;
}