
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define STATE_INIT (0)

typedef struct {
    pthread_t th;
    int state;
} my_thread_t;

my_thread_t *thd;

// 1. SEMAPHORE & LOCK
sem_t sem;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *routine(void *arg) {
    sem_wait(&sem);                     // 4. SEMWAIT
    pthread_mutex_lock(&mutex);         // 3. LOCK
    printf("routine: begin\n");
    printf("routine: state is %d\n", thd->state);
    pthread_mutex_unlock(&mutex);       // 3. UNLOCK
    return NULL;
}

void myWaitThread(my_thread_t *p) {
    pthread_join(p->th, NULL); 
}

my_thread_t *myCreateThread(void *(*start_routine)(void *)) {
    my_thread_t *p = malloc(sizeof(my_thread_t));
    if (p == NULL) 
	    return NULL;
    p->state = STATE_INIT;
    pthread_create(&p->th, NULL, start_routine, NULL); 
    // turn the sleep off to avoid the fault, sometimes...
    sleep(1);        // 자는동안 쓰레드에서 thd에 접근하는데, 리턴이 되지 않았으므로
                     // 값이 안들어있음 -> fault
    return p;       
}

int main(int argc, char *argv[]) {

    // 2. SEMAPHORE INIT : 0
    sem_init(&sem, 0, 0);

    printf("ordering: begin\n");
    pthread_mutex_lock(&mutex);      // 3. LOCK
    thd = myCreateThread(routine);
    pthread_mutex_unlock(&mutex);    // 3. UNLOCK
    sem_post(&sem);                  // 4. SEMSIGNAL
    myWaitThread(thd);
    printf("ordering: end\n");
    return 0;
}