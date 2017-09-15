//
//  main.c
//  Process
//
//  Created by ksgin on 14/09/2017.
//  Copyright © 2017 ksgin. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>

//fork

int fork_example() {

    pid_t pd = fork();

    if (pd < 0) {
        printf("error to fork");
    }

    int count = 100;

    if(pd == 0) {
        count *= 200;
        printf("it child process  ， pid = %d , count = %d\n" , getpid() , count);
    } else {
        count += 200;
        printf("it father process ,  pid = %d , count = %d\n" , getpid() , count);
    }

    return 0;
}


int main(){
    fork_example();
    return 0;
}
