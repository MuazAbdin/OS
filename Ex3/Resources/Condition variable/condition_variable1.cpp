#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2

typedef struct ThreadContext {
	int threadID;
    int *canUseVar;
    int *sharedVar;
    pthread_mutex_t *varMutex;
    pthread_cond_t *cv;
} ThreadContext;

void *foo(void *arg){
	ThreadContext* tc = (ThreadContext*) arg;

    if (pthread_mutex_lock(tc->varMutex) != 0){
        fprintf(stderr, "error on pthread_mutex_lock");
        exit(1);
    }
    int canUseVar = *(tc->canUseVar);

    if (canUseVar == 0){
        pthread_cond_wait(tc->cv, tc->varMutex);
    }
    printf("Thread %d\n", tc->threadID);

    // other threads can safely use the var now 
    *(tc->sharedVar) += 1;
    printf("SharedVar: %d\n", *(tc->sharedVar));

    if (pthread_mutex_unlock(tc->varMutex) != 0) {
        fprintf(stderr, "error on pthread_mutex_unlock");
        exit(1);
    }
}

void *init_foo(void *arg){
	ThreadContext* tc = (ThreadContext*) arg;
    usleep(100);

    if (pthread_mutex_lock(tc->varMutex) != 0){
        fprintf(stderr, "error on pthread_mutex_lock");
        exit(1);
    }

    printf("Thread %d\n", tc->threadID);
    // Do var initialization
    *(tc->sharedVar) = 0;
    *(tc->canUseVar) = 1;

    pthread_cond_signal(tc->cv);
    /* pthread_cond_broadcast(tc->cv); */

    if (pthread_mutex_unlock(tc->varMutex) != 0) {
        fprintf(stderr, "error on pthread_mutex_unlock");
        exit(1);
    }

}

int main(){
    int res;
    int sharedVar = 100;
    int canUse = 0;
    pthread_mutex_t varMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

    pthread_t threads[NUM_THREADS];
	ThreadContext contexts[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; ++i) {
        ThreadContext c = {i, &canUse, &sharedVar, &varMutex, &cv};
		contexts[i] = c; 
	}

    for(int t=0; t<NUM_THREADS; t++){
        if (t == 0)
            res = pthread_create(threads + t, NULL, init_foo, contexts + t);
        else 
            res = pthread_create(threads + t, NULL, foo, contexts + t);

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

	if (pthread_mutex_destroy(&varMutex) != 0) {
		fprintf(stderr, "error on pthread_mutex_destroy");
		exit(1);
	}
	if (pthread_cond_destroy(&cv) != 0){
		fprintf(stderr, "error on pthread_cond_destroy");
		exit(1);
	}
}

