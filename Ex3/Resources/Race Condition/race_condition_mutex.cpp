#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

// Please don't initialize global variables if you don't need (as here)
int counter;

void *foo(void *arg) {
    pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
    if (pthread_mutex_lock(mutex) != 0){
        fprintf(stderr, "error on pthread_mutex_lock");
        exit(1);
    }

    int a = counter ;
    usleep(100);
    a++;
    counter = a;

    if (pthread_mutex_unlock(mutex) != 0) {
        fprintf(stderr, "error on pthread_mutex_unlock");
        exit(1);
    }
}

int main(){
    int res;
    counter = 0;
    pthread_t threads[NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    for(int t=0;t<NUM_THREADS;t++){
        res = pthread_create(&threads[t], NULL, foo, &mutex);
        if (res<0){
            printf("ERROR\n");
            exit(-1);
        }
    }

    for(int t=0;t<NUM_THREADS;t++) {
        res = pthread_join(threads[t], NULL);
        if (res<0) {
            printf("ERROR \n");
            exit(-1);
        }
    }

    printf("counter: %d\n", counter);
    pthread_mutex_destroy(&mutex);
}

