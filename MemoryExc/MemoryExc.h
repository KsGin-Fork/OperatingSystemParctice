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


/*--------------------全局变量区-----------------------------*/
static int mem_size = DEFAULT_MEM_SIZE; /*内存大小*/
static int ma_algorithm = MA_FF; /*当前分配算法*/
static int pid = 0; /*初始pid*/
static int flag = 0; /*设置内存大小标志*/

/*指向内存中空闲区块链表的首指针*/
static struct free_block_type *free_block;

/*进程分配内存块链表的首指针*/
static struct allocated_block *allocated_block_head = NULL;

/*----------------------------------------------------------*/


/*---------------------函数区--------------------------------*/
struct free_block_type* init_free_block(int);
int set_mem_size(void);
void set_algorithm(void);
void rearrange(int);
void rearrange_WF(void);
void rearrange_FF(void);
void rearrange_BF(void);
int new_process(void);
int allocate_mem(struct allocated_block*);
void kill_process(void);
int free_mem(struct allocated_block*);
int dispose(struct allocated_block*);
int display_mem_usage(void);
void display_menu(void);
struct allocated_block* find_process(int);
/*-----------------------------------------------------------*/


#endif
