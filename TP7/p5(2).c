#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h> // For O_* constants
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <sys/types.h>

int max, loops;
int * buffer;
int fillIndex, useIndex, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* Buffer Operations */
void put(int value) {
    buffer[fillIndex] = value;
    fillIndex = (fillIndex + 1) % max;
    count++;
}

int get() {
    int tmp = buffer[useIndex];
    useIndex = (useIndex + 1) % max;
    count--;
    return tmp;
}

/* Currently unsafe producer and consumer */
void *producer(void * arg) {
    printf("Producer running\n");
    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex); // acquire the lock
        if (count == max) // check if the buffer is full
            pthread_cond_wait(&cond, &mutex); // wait on the CV
        put(i); // produce!
        pthread_cond_signal(&cond); // signal the CV
        pthread_mutex_unlock(&mutex); // release the lock
    }
    printf("Producer: total produced is %d\n",get());
    return NULL;
}

void *consumer(void * arg) {
    printf("Consumer running\n");
    int tmp;
    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&mutex); // acquire the lock
        if (count == 0) // check if the buffer is empty
            pthread_cond_wait(&cond, &mutex); // wait on the CV
        tmp+=get();
        pthread_cond_signal(&cond); // signal the CV
        pthread_mutex_unlock(&mutex); // release the lock
    }
    printf("Consumer: total consumed is %d\n", tmp);
    return NULL;
}

int main(int argc, char * argv[]) {
    
    /* Process arguments */
    max = atoi(argv[1]);
    loops = atoi(argv[2]);

    /* Allocate space for the buffer */
    buffer = malloc(sizeof(int)*max);
    pthread_t prods, cons;

    /* Create the producer */
    pthread_create(&prods, NULL, producer, NULL);
    
    /* Create the consumers */
    pthread_create(&cons, NULL, consumer, NULL);
    
    /* Wait for all threads to finish */
    pthread_join(prods, NULL);
    pthread_join(cons, NULL);
    
    return 0;

}

