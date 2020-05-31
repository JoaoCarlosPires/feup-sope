#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) { 
    
    if (argc != 3) {
        printf("Numero de argumentos insuficiente.");
        return 1;
    }

    FILE* fp1;
    FILE* fp2;
    char buffer[100];

    fp1 = fopen(argv[1], "r+");
    fp2 = fopen(argv[2], "w+");
    
    fread(buffer, sizeof(buffer)+1, 1, fp1);
    fwrite(buffer, sizeof(buffer)+1, 1, fp2);

    fclose(fp2);
    fclose(fp1);
    
    return 0;
}