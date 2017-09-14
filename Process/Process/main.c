//
//  main.c
//  Process
//
//  Created by ksgin on 14/09/2017.
//  Copyright © 2017 ksgin. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {

    pid_t pd = fork();

    if (pd < 0) {
        printf("error to fork");
    }

    printf("pid = %d\n" , getpid());

    return 0;
}
