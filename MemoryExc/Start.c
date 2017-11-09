//
//  Start.c
//  MemoryExc
//
//  Created by KsGin on 2017/11/7.
//  Copyright © 2017年 KsGin. All rights reserved.
//

#include "MemoryExc.h"

int main(int argc , char** argv) {
    
    int flag = 0;
    int pid = 0;
    int mem_size = DEFAULT_MEM_SIZE;
    int ma_algorithm = MA_FF;
    struct free_block_type* free_block = init_free_block(mem_size);
    struct allocated_block* allocated_block_head = NULL;
    
    int choice = 0;
    while (1) {
        display_menu();
        fflush(stdin);
        scanf("%d" , &choice);
        switch (choice) {
            case 1:
                set_mem_size(free_block , &mem_size , &flag);
                break;
            case 2:
                set_algorithm(&ma_algorithm);
                flag = 1;
                break;
            case 3:
                new_process(&pid , &free_block , &allocated_block_head , &mem_size);
                flag = 1;
                break;
            case 4:
                kill_process(ma_algorithm , allocated_block_head , &mem_size , &free_block);
                flag = 1;
                break;
            case 5:
                display_mem_usage(free_block , allocated_block_head);
                flag = 1;
                break;
            default:
                exit(0);
                break;
        }
    }
    return 1;
}



