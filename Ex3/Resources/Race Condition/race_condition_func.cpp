#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>

#define NUM_THREADS 1


int counter_a; 
int counter_b; 

void *foo2(int a, int b){
    printf("a: %d, b; %d\n", a,b);
    counter_a += a;
    counter_b += b;
}

void *foo(void *arg){
    int a = 1;
    foo2(a, a++);
}

int main(){
    int res;
    counter_a = 0;
    counter_b = 0;
    pthread_t threads[NUM_THREADS];
    for(int t=0;t<NUM_THREADS;t++){
        res = pthread_create(&threads[t], NULL, foo, NULL);
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

    printf("counter_a: %d\n", counter_a);
    printf("counter_b: %d\n", counter_b);
}

