#include <stdio.h>
#include <pthread.h>

static volatile int counter = 0;
static const int end = 100000000;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct { char *id; int val; } myarg;

void *mythread(void *arg) {
    myarg *ma = (myarg *) arg;

    // 인자값[쓰레드 id]
    printf("%s[%u] : begin \n", ma->id, (unsigned) pthread_self());

    pthread_mutex_lock(&lock);      // LOCK!
    for(int i = 0; i < end; i++) {  // CRITICAL SECTION : 카운터 1억 감소, 인자값 1억 증가
        --counter;
        ma->val++;
    }
    pthread_mutex_unlock(&lock);    // UNLOCK

    // 결과 인자값 반영하여 출력
    printf("%s[%u] : done \n", ma->id, (unsigned) pthread_self());
    return (void *)ma;
}

int main() {
    printf("main[%u] : begin (counter = %d) \n", (unsigned) pthread_self(), counter);
    pthread_t t1, t2;
    myarg ma1 = {"thread", end}; // 시작 ~ 인자값 : 1억, 카운터 : 0

    // t1에 저장된 쓰레드가 mythread 루틴을 ma1 인자를 넣어서 실행
    // 루틴 결과 ~ 카운터 1억 감소, 인자값 1억 증가됨
    pthread_create(&t1, NULL, mythread, &ma1);
    

    // 카운터 1억 증가
    pthread_mutex_lock(&lock);                  // LOCK!
    for(int i = 0; i < end; i++) counter++;     // CRITICAL SECTION
    pthread_mutex_unlock(&lock);                // UNLOCK

    /* 
        쓰레드와 메인의, counter를 둔 race 발생
        load, inc/dec, store 과정에서 방해를 받으면 결과가 반영되지 않음
        이를 막기 위하여 critical section 주위에 lock을 걸어주었음
    */
    
    // t1 종료 후, 카운터와 인자값 출력
    pthread_join(t1, (void **) &ma1);
    printf("main[%u] : done (counter = %d) (ma1.val = %d) \n", 
            (unsigned) pthread_self(), counter, ma1.val);

    return 0;
}
