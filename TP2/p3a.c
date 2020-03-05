//FOLHA 2 - p2b.c
//FILE COPY
//USAGE: copy source destination

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>


int main(int argc, char *argv[]) {
    char c;
    FILE* fp1;

    if (argc != 2) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    fp1 = fopen(argv[1], "r+");

    c = fgetc(fp1); 
    while (c != EOF) 
    { 
        printf ("%c", c); 
        c = fgetc(fp1); 
    } 

    printf("\n");

    fclose(fp1);

    return 0;
}