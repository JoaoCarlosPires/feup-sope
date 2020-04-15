#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define MAXLINE 1000
#define READ 0
#define WRITE 1

void sig_pipe(int signo);
void err_sys(char *msg);
void err_msg(char *msg);

int main(void) {
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];
    
    signal(SIGPIPE, sig_pipe);
    pipe(fd1);
    pipe(fd2);
    
    pid=fork();
    
    if (pid>0) { // PARENT

        close(fd1[READ]); close(fd2[WRITE]);
        printf("Input 2 numbers (END = CTRL-D): ");
        
        while (fgets(line, MAXLINE,stdin) != NULL) {
            
            n=strlen(line);
            write(fd1[WRITE],line,n); // null ending char is not send !
            n=read(fd2[READ],line,MAXLINE); //waits for answer (= sum)
            if (n==0) { err_msg("child closed pipe"); break; }
            line[n]=0; // null ending char is not received, so "add" it
            int op = 0;
             for(int i=0; line[i]!='\0'; i++) {
                printf("%c", line[i]);
                if(line[i]==' ') {
                    switch (op) {
                        case 0: printf(" \t(sum)\n"); break;
                        case 1: printf(" \t(difference)\n"); break;
                        case 2: printf(" \t(multiplication)\n"); break;
                        default: break;
                    }
                    op++; 
                }
            }
            printf(" \t(division)\n"); 
            printf("Input 2 numbers (END 0 CTRL-D): ");
        
        }
        
        exit(0);
    
    } else { // CHILD
        
        close(fd1[WRITE]); close(fd2[READ]);
        dup2(fd1[READ],STDIN_FILENO);
        // redirect I/O of the coprocess
        dup2(fd2[WRITE],STDOUT_FILENO); // to the pipes
        if (execl("./p2_cop","p2_cop",NULL) < 0) // execute the coprocess
            err_sys("execl error");
    
    }
    
    return 0;
}


void sig_pipe(int signo) {
    printf("SIGPIPE caught\n");
    exit(1);
}

void err_sys(char *msg) {
    fprintf(stderr,"%s\n",msg);
    exit(1);
}

void err_msg(char *msg) {
    printf("%s\n",msg);
    return;
}