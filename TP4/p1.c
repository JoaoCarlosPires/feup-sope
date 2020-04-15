// PROGRAMA p01a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signo) {
    printf("In SIGINT handler ...\n");
    //sleep(rest);
}

int main(void) {

    int rest;

    //v1.0
    /*
    if (signal(SIGINT,sigint_handler) < 0) {
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }
    */

    //v2.0 
    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT,&action,NULL) < 0) {
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }
    
    printf("Sleeping for 30 seconds ...\n");
    rest = sleep(30);
    while (rest!=0) {rest = sleep(rest);}
    printf("Waking up ...\n");
    exit(0);

}