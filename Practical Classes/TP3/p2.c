#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

    /*v1.0*/
    //write(STDOUT_FILENO,"1",1);
    
    /*v2.0*/
    //printf("1");

    /*v3.0*/
    printf("1\n");
    
    if(fork() > 0) {
        /*v1.0*/
        //write(STDOUT_FILENO,"2",1);
        //write(STDOUT_FILENO,"3",1);
        
        /*v2.0*/
        printf("2");
        printf("3");
    } else {
        /*v1.0*/
        //write(STDOUT_FILENO,"4",1);
        //write(STDOUT_FILENO,"5",1);
        
        /*v2.0*/
        printf("4");
        printf("5");
    }
    
    /*v1.0*/
    //write(STDOUT_FILENO,"\n",1);

    /*v2.0*/
    printf("\n");

    return 0;
}