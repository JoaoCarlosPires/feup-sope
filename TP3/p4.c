#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    
    if(fork() > 0) {
        /*b)*/
        //write(STDOUT_FILENO,"Hello ",6);
        
        /*a)*/
        printf("world!");
    } else {
        /*b)*/
        //write(STDOUT_FILENO,"world!",6);
        
        /*a)*/
        write(STDOUT_FILENO,"Hello ",6);
    }
    
    return 0;
}