#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

// Please don't initialize global variables if you don't need (as here)
int counter;

void *fooplusplus(void *arg) {
    usleep(10000);
    counter++;
    return NULL;
}

void *foo(void *arg){
    int a = counter ;
    usleep(200);
    a++;
    counter = a;
    return NULL;
}

int main(){
    int res;
    counter = 0;
    pthread_t threads[NUM_THREADS];
    for(int t=0;t<NUM_THREADS;t++){
        res = pthread_create(&threads[t], NULL, fooplusplus, NULL);
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
}

