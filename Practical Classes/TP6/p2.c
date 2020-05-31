// PROGRAMA p02.c 
#include <stdio.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <stdint.h>

#define STDERR 2 
#define NUMITER 30000 

int N = 50000;

void * thrfunc(void * arg) {   
    int i;   
    int auxN = N;

    fprintf(stderr, "\nStarting thread %s\n", (char *) arg);
    
    for (i = 1; i <= NUMITER; i++) { 
        if (N > 0) {
            write(STDERR,arg,1);
            N--;
        }
    }   
    
    pthread_exit((void *)(uintptr_t)(auxN-N)); 
} 

void * thrfunc2(void * arg) {   
    int i;  
    int arg2 = *(int *)arg;
    int auxN = N;

    fprintf(stderr, "\nStarting thread %i\n", arg2);
    
    for (i = 1; i <= NUMITER; i++) {
        if (N > 0) {
            fprintf(stderr, "%i", arg2);
            N--;
        }
    }

    pthread_exit((void *)(uintptr_t)(auxN-N));
} 


int main() {    
    pthread_t ta, tb;
    
    //char a[] = "1";
    //char b[] = "2";

    //pthread_create(&ta, NULL, thrfunc, a);   
    //pthread_create(&tb, NULL, thrfunc, b);  
    
    //pthread_join(ta, NULL);   
    //pthread_join(tb, NULL);
    
    int a = 1;
    int b = 2; 

    int counterA = 0;
    int counterB = 0;
    int *aux;     
    
    pthread_create(&ta, NULL, thrfunc2, (void *)&a);
    pthread_create(&tb, NULL, thrfunc2, (void *)&b);   
    
    pthread_join(ta, (void *)&aux);
    counterA+=(uintptr_t)aux;
    pthread_join(tb, (void *)&aux);
    counterB+=(uintptr_t)aux;

    printf("\n\nNo. A: %i\n", counterA);
    printf("No. B: %i\n", counterB);

    return 0; 
}