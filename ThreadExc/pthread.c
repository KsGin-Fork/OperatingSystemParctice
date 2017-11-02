#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>

#define INTTOVOIDP(i) (void*)(uintptr_t)i
#define MAX_THREAD 3

pthread_mutex_t pmt;

unsigned long long main_counter, counter[MAX_THREAD];

void *thread_worker(void *);

int main(int argc, char *argv[]) {
    int i,ch;

    pthread_mutex_init(&pmt , PTHREAD_MUTEX_NORMAL);

    pthread_t pthread_id[MAX_THREAD] = {0};
    for (i = 0; i < MAX_THREAD; i++) {
       if(pthread_create(&pthread_id[i] , NULL, thread_worker ,INTTOVOIDP(i))!=0){
            printf("create failed");
            exit(1);
       }
    }
    do {
        unsigned long long sum = 0;
        //pthread_mutex_lock(&pmt);  //主线程锁
        for (i = 0; i < MAX_THREAD; i++) {
            sum += counter[i];
            printf("第%d个线程的计数值 = %llu \t", i+1 , counter[i]);
        }
        printf("主计数值/各个线程计数之和(%llu/%llu)", main_counter, sum);
        //pthread_mutex_unlock(&pmt);
    } while ((ch = getchar()) != 'q');
    return 0;
}


void *thread_worker(void *p) {
    int thread_num = (int)p;
    for (;;) {
        //pthread_mutex_lock(&pmt); //子线程锁
        counter[thread_num]++;
        main_counter++; /* 主counter 加一 */
        //pthread_mutex_unlock(&pmt);
    }
}



