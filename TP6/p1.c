// PROGRAMA p01.c 
#include <stdio.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <stdint.h>

#define STDERR 2 
#define NUMITER 1000000 

void * thrfunc(void * arg) {   
    int i;   
    
    fprintf(stderr, "Starting thread %s\n", (char *) arg);
    
    for (i = 1; i <= NUMITER; i++) 
        write(STDERR,arg,1);   
    
    return NULL; 
} 

void * thrfunc2(void * arg) {   
    int i;  
    int arg2 = *(int *)arg;

    fprintf(stderr, "Starting thread %i\n", arg2);
    
    for (i = 1; i <= NUMITER; i++)
        fprintf(stderr, "%i", arg2);
        //write(STDERR,&arg2,sizeof(int));   
    
    return NULL; 
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
    
    pthread_create(&ta, NULL, thrfunc2, (void *)&a);
    pthread_create(&tb, NULL, thrfunc2, (void *)&b);   
    
    pthread_join(ta, NULL);   
    pthread_join(tb, NULL);
    
    return 0; 
}