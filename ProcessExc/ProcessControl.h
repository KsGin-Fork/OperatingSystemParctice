/*
 * File   : ProcessControl
 * Author : KsGin 
 * Date   : 2017/10/10
 */

#ifndef PROCESSEXC_PROCESSCONTROL_H
#define PROCESSEXC_PROCESSCONTROL_H

#include <cstdio>
#include <sys/types.h>
#include <cstdlib>
#include <csignal>
#include <cctype>
#include <zconf.h>
#define MAX_CHILD_NUMBER 10

/*子进程睡眠时间*/
#define SLEEP_INTERVAL 2

int proc_number=0; /*子进程的自编号，从0开始*/

void do_something() {
    while(true)  {
        printf("This is process No.%d and its pid is %d\n", proc_number,  getpid());
        sleep(SLEEP_INTERVAL);
    }
}


void ProcessControlTest(int argc, char* argv[]) {

    int child_proc_number = MAX_CHILD_NUMBER;  /* 子进程个数 */
    int i, ch;
    pid_t child_pid;

    pid_t pid[10] = {0}; /* 存放每个子进程的id */

    if (argc > 1) /* 命令行参数第一个参数表示子进程个数*/
    {
        child_proc_number = atoi(argv[1]);
        child_proc_number = (child_proc_number > 10) ? 10 :
                            child_proc_number;
    }

    for (i=0; i<child_proc_number; i++) {
        /* 填写代码，建立child_proc_number个子进程要执行
        * proc_number = i;
        * do_something();
        * 父进程把子进程的id保存到pid[i] */

        pid_t tmp = fork();

        if(tmp < 0){
            printf("error");
        }else if(tmp == 0) {
            proc_number = i;
            do_something();
        }else{
            pid[i] = tmp;
        }

    }
    /* 让用户选择杀死进程，数字表示杀死该进程，q退出 */
    while ((ch = getchar()) != 'q')      {
        if (isdigit(ch))  {
            /*  填写代码，向pid[ch-'0']发信号SIGTERM，
             * 杀死该子进程 */
            kill(pid[ch-'0'] , SIGTERM);
        }
    }

    /* 在这里填写代码，杀死本组的所有进程 */
    kill(0 , SIGTERM);
}

#endif //PROCESSEXC_PROCESSCONTROL_H
