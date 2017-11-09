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
int set_mem_size(struct free_block_type*, int*, int*);
void set_algorithm(int* , struct free_block_type**);
void rearrange(int , struct free_block_type**);
void rearrange_WF(struct free_block_type**);
void rearrange_FF(struct free_block_type**);
void rearrange_BF(struct free_block_type**);
int new_process(int*, struct free_block_type**,struct allocated_block**,int*);
int allocate_mem(struct allocated_block*, struct free_block_type**,int*, struct allocated_block**, int*);
void kill_process(int, struct allocated_block**, int*, struct free_block_type**);
int free_mem(struct allocated_block *, int, int*, struct free_block_type**);
int dispose(struct allocated_block*, struct allocated_block**);
int display_mem_usage(struct free_block_type*, struct allocated_block*);
void display_menu(void);
struct allocated_block* find_process(int, struct allocated_block*);
void free_memory_rearrage(int,int,struct free_block_type**,int*, int*,struct allocated_block**);
void do_exit(struct free_block_type**, struct allocated_block**);
/*-----------------------------------------------------------*/


#endif
