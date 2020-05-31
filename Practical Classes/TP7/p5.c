//PRODUCER-CONSUMER PROBLEM
//PRODUCER AND CONSUMER ARE THREADS OF A SINGLE PROCESS
//Illustrates the use of POSIX semaphores for synchronization
//Note: buffer capacity is 1
//JAS
// NOTE: error return codes are not checked ...
// You must add them.
// compilation: gcc p5.c -lpthread -lrt -Wall -o p5

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NOT_SHARED 0 // sem. is not shared w/other processes

sem_t empty, full; // the global semaphores
int data; // shared buffer; capacity=1
int numItems; // number of items to be produced/consumed

// Put items (1, ..., numItems) into the data buffer and sum them
void *producer(void *arg) {
    int total=0, produced;
    printf("Producer running\n");
    for (produced = 1; produced <= numItems; produced++) {
        sem_wait(&empty);
        data = produced;
        total = total+data;
        sem_post(&full);
    }
    printf("Producer: total produced is %d\n",total);
    return NULL;
}

// Get values from the data buffer and sum them
void *consumer(void *arg) {
    int total = 0, consumed;
    printf("Consumer running\n");
    for (consumed = 1; consumed <= numItems; consumed++) {
        sem_wait(&full);
        total = total+data;
        sem_post(&empty);
    }
    printf("Consumer: total consumed is %d\n",total);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t pid, cid;
    
    if (argc != 2) {
        fprintf(stderr,"USAGE: %s numItems\n",argv[0]);
        exit(1);
    }

    numItems = atoi(argv[1]); // num. of items to be produced/consumed
    
    sem_init(&empty, NOT_SHARED, 1);
    sem_init(&full, NOT_SHARED, 0);
    
    printf("Main started.\n");
    
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("Main done.\n");

    return 0;
}