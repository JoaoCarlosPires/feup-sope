#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    pid_t pid = fork();
    pid_t pid2;
    
    switch (pid) {
        
        case -1:
            perror("fork");
            break;
        
        case 0: //filho
            pid2 = fork();
            switch (pid2) {
                case -1:
                    perror("fork");
                    break;
                case 0: //filho2
                    write(STDOUT_FILENO,"friends!",8);
                    break;
                default: //pai2
                    write(STDOUT_FILENO,"my ",3);
                    break;
            }
            break;
        
        default: //pai
            write(STDOUT_FILENO,"Hello ",6);
            break;
    }
    
    return 0;
}
