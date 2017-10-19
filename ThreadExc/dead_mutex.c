/*
 *  Author: KsGin
 *  Date  : 2017/10/19
 */

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<pthread.h>

#define LOOP_TIMES 10000

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; //初始化锁

pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_worker(void*); //thread worker 函数

void critical_section(int thread_num , int i);


int main(int argc , char* argv[]){

    pthread_t pthread_id = 0;

    if(pthread_create(&pthread_id , NULL , thread_worker , NULL)){
        printf("create error");
        return -1;
    };

    for (int i = 0; i < LOOP_TIMES; ++i) {
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);
        critical_section(1 , i);
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}


void* thread_worker(void* p){
    for (int i = 0; i < LOOP_TIMES; ++i) {
        pthread_mutex_lock(&mutex1);   //dead lock
        pthread_mutex_lock(&mutex2);
        critical_section(2 , i);
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}


void critical_section(int thread_num , int i){
    printf("Thread%d:%d\n" , thread_num , i);
}
