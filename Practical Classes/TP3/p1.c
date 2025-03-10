#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global=1;

int main(void) {
    int local = 2;
    
    if(fork() > 0) {
        printf("PID = %d; PPID = %d\n", getpid(), getppid());
        global++;
        local--;
    } else {
        printf("PID = %d; PPID = %d\n", getpid(), getppid());
        global--;
        local++;
    }
    
    printf("PID = %d - global = %d ; local = %d\n", getpid(), global, local);
    
    return 0;
}