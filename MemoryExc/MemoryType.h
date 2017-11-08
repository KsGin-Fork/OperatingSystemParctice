/*
 * File Name : MemoryType.h
 * Author    : KsGin
 * Date      : 11/07/2017
 */

#ifndef MEMORYTYPE_H
#define MEMORYTYPE_H

#include <stdio.h>

#define PROCESS_NAME_LEN 32 /*进程名长度*/
#define MIN_SLICE 10 /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024 /*默认内存大小*/
#define DEFAULT_MEM_START 0 /*默认起始位置*/

/*内存分配算法*/
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3

/*描述内存空闲块的数据结构*/
struct free_block_type{
    int size;
    int start_addr;
    struct free_block_type *next;
};

/*描述已分配的内存块*/
struct allocated_block{
    int pid;
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block* next;
};

#endif
