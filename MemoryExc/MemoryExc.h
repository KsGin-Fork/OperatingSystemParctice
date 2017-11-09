/*
 * File Name : MemoryExc
 * Author    : KsGin
 * Date      : 11/07/2017
 */

#ifndef MEMORYEXC_H
#define MEMORYEXC_H

#include <stdlib.h>
#include <stdio.h>
#include "MemoryType.h"

/*---------------------函数区--------------------------------*/
struct free_block_type* init_free_block(int);
int set_mem_size(struct free_block_type* free_block , int *mem_size, int *flag);
void set_algorithm(int *ma_algorithm);
void rearrange(int);
void rearrange_WF(void);
void rearrange_FF(void);
void rearrange_BF(void);
int new_process(int *pid, struct free_block_type** free_block , struct allocated_block** allocated_block_head ,int* mem_size);
int allocate_mem(struct allocated_block*ab , struct free_block_type** free_block ,int* mem_size, struct allocated_block** allocated_block_head , int *pid);
void kill_process(int ma_algorithm , struct allocated_block *allocated_block_head , int* mem_size, struct free_block_type** free_block);
int free_mem(struct allocated_block *ab, int ma_algorithm , int* mem_size, struct free_block_type** free_block);
int dispose(struct allocated_block*ab , struct allocated_block *allocated_block_head);
int display_mem_usage(struct free_block_type* free_block , struct allocated_block* allocated_block_head);
void display_menu(void);
struct allocated_block* find_process(int pid , struct allocated_block *allocated_block_head);
//内存紧缩处理
void free_memory_rearrage(int memory_reduce_size,int allocated_size ,
                          struct free_block_type** free_block ,
                          int* mem_size , int* pid,
                          struct allocated_block** allocated_block_head);
/*-----------------------------------------------------------*/


#endif
