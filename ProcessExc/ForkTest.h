/*
 * File   : ForkTest
 * Author : KsGin 
 * Date   : 2017/10/10
 */

#ifndef PROCESSEXC_FORKTEST_H
#define PROCESSEXC_FORKTEST_H


#include <cstdio>
#include <zconf.h>

int fork1();
int fork2();
int fork3();




int fork1(){

    pid_t pid = fork();

    if (pid < 0) {
        printf("fork error\n");
    }

    if (pid == 0) {
        printf("child process , child id = %d , parent id = %d\n" , getpid() , getppid());
    }

    if (pid > 0){
        printf("parent process , child id = %d , parent id = %d\n" , pid , getpid());
    }

    return 0;
}


int fork2(){
    pid_t pid;
    char * msg;
    int k;
    printf("process creating study!\n");
    pid = fork();
    switch (pid){
        case 0:
            msg = const_cast<char *>("Child process is running");
            k = 3;
            break;
        case -1:
            msg = const_cast<char *>("error");
            k = 0;
            break;
        default:
            msg = const_cast<char *>("parent process is running");
            k = 5;
            break;
    }

    while (k > 0){
        puts(msg);
        k--;
        sleep(1);
    }
    return 0;
}

int fork3(){
    pid_t pid;
    printf("process creating study!\n");
    pid = fork();
    switch (pid){
        case 0:
            while (true){
                printf("background process , id = %d , parent id = %d" , getpid() , getppid());
                sleep(3);
            }
        case -1:
            printf("Error");
            break;
        default:
            printf("parent process , pid = %d\n" , getpid());
            sleep(3);
            break;
    }

    return 0;
}

#endif //PROCESSEXC_FORKTEST_H
