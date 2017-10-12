#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_THREAD 3 /* 线程的个数 */
unsigned long long main_counter, counter[MAX_THREAD];

void *thread_worker(void *);

int main(int argc, char *argv[]) {
    int i,ch;
    pthread_t pthread_id[MAX_THREAD] = {0};
    for (i = 0; i < MAX_THREAD; i++) {
        pthread_create(&pthread_id[i] , NULL, thread_worker , (void*)i);
    }
    do {

        unsigned long long sum = 0;

        for (i = 0; i < MAX_THREAD; i++) {
            sum += counter[i];
            printf("第%d个线程的计数值 = %llu \t", i+1 , counter[i]);
        }

        printf("主计数值/各个线程计数之和(%llu/%llu)", main_counter, sum);
    } while ((ch = getchar()) != 'q');
    return 0;
}

void *thread_worker(void *p) {

    int thread_num = (int)p;


    for (;;) {
        counter[thread_num]++; /* 本线程的counter加一 */
        main_counter++; /* 主counter 加一 */
    }
}



