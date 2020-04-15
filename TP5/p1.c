#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


#define READ 0
#define WRITE 1

struct operands 
{ 
   int operand1; 
   int operand2;  
};

int main(void) {
    
    int fd[2];
    pid_t pid;
    pipe(fd);
    pid = fork();
    
    if (pid >0) { //pai 
        
        //struct operands op;
        char s1[32];
        char s2[32];

        
        printf("PARENT:\n");
        //printf("x y ? "); scanf("%d %d",&op.operand1,&op.operand2);
        printf("x y ? "); scanf("%32s %32s", s1, s2);
        close(fd[READ]);
        //write(fd[WRITE],&op,2*sizeof(int));
        write(fd[WRITE],&s1,sizeof(char));
        write(fd[WRITE],&s2,sizeof(char));
        close(fd[WRITE]);

    } else { //filho
        
        char b1[] = "";
        char b2[] = "";
        close(fd[WRITE]);
        //read(fd[READ],b,2*sizeof(int));
        read(fd[READ],b1,sizeof(char));
        read(fd[READ],b2,sizeof(char));
        int b[2];
        b[0] = atoi(b1);
        b[1] = atoi(b2);
        printf("x %d\n", b[0]);
        printf("y %d\n", b[1]);
        printf("SON:\n");
        printf("x + y = %d\n", b[0]+b[1]);
        printf("x - y = %d\n", b[0]-b[1]);
        printf("x * y = %d\n", b[0]*b[1]);
        if (b[1]==0) printf("The following operation isn't possible: x / y\n");
        else {
            float c[2]; c[0] = b[0]; c[1] = b[1];
            printf("x / y = %f \n", c[0]/c[1]);
        }
        close(fd[READ]);
    
    }
    
    return 0;
}