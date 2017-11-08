//
//  Start.c
//  MemoryExc
//
//  Created by KsGin on 2017/11/7.
//  Copyright © 2017年 KsGin. All rights reserved.
//

#include "MemoryExc.h"

int main(int argc , char** argv) {
    char choice;
    pid = 0;
    free_block = init_free_block(mem_size);
    while (1) {
        display_menu();
        fflush(stdin);
        choice = getchar();
        switch (choice) {
            case '1':
                set_mem_size();
                break;
            case '2':
                set_algorithm();
                flag = 1;
                break;
            case '3':
                new_process();
                flag = 1;
                break;
            case '4':
                kill_process();
                flag = 1;
                break;
            case '5':
                display_mem_usage();
                flag = 1;
                break;
            default:
                exit(0);
                break;
        }
    }
    return 1;
}



