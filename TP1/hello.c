#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Aula Teórico-Prática 1 - 20/02/2020

//Programa para imprimir Hello <Nome> (<n> vezes), em que <Nome> é o primeiro 
//argumento usado quando o programa é corrido e <n> é o segundo argumento 

/*
int main(int argc, char* argv[]) {
    for (int i = 1; i <= atoi(argv[2]); i++)
        printf("Hello %s!\n", argv[1]);
    return 1;
}
*/

// Programa para imprimir as variáveis de ambiente, uma por linha

/*
int main(int argc, char *argv[], char * envp[]) { 
    int i;
    for (i = 0; envp[i] != NULL; i++) {
        printf("%s\n", envp[i]);
    }
    return 0;
}
*/

// Programa para imprimir Hello <Nome>, em que <Nome> representa
// o nome do utilizador atual

/* Solução 1
int main(int argc, char *argv[], char * envp[]) { 
    int i;
    printf("Hello "); 
    for (i = 0; envp[i] != NULL; i++) {
        if (strncmp("USERNAME=", envp[i], 8) == 0) {
            for (int k = 1; k <=strlen(envp[i]); k++) {
                if (k<=8) {
                } else {
                    printf("%c", envp[i][k]);
                }
            }    
        }
    }
    printf("!\n");
    return 0;
}
*/

/* Solução 2
int main(int argc, char *argv[], char * envp[]) { 
    printf("Hello %s!\n", getenv("USERNAME"));
    return 0;
}
*/

/* Solução 3

To set a new environment variable USER_NAME, we should start by typing in the terminal USER_NAME = ... and then echo $USER_NAME.
To export to an environment variable, we must use the export command, typing export USER_NAME.


int main(int argc, char *argv[], char * envp[]) { 
    printf("Hello %s!\n", getenv("USER_NAME"));
    return 0;
} 
*/


// PROGRAMA p6a.c
#define BUF_LENGTH 256
#define MAX 256

int main(void) {
    FILE *src, *dst;
    char buf[BUF_LENGTH];
    if ((src = fopen("infile.txt", "r")) == NULL) {
        exit(1);
    }
    if ((dst = fopen("outfile.txt", "w")) == NULL) {
        exit(2);
    }
    while(fgets(buf, MAX, src) != NULL) {
        fputs(buf, dst);
    }
    fclose(src);
    fclose(dst);
    exit(0); // zero é geralmente indicativo de "sucesso"
}


