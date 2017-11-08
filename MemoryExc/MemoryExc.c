/*
 * File Name : MemoryExc
 * Author    : KsGin
 * Date      : 11/07/2017
 */

#include "MemoryExc.h"

/*初始化空闲区，默认为一块，可以指定大小及起始地址*/
struct free_block_type *init_free_block(int mem_size) {
    struct free_block_type *fb = (struct free_block_type *) malloc(sizeof(struct free_block_type));
    if (fb == NULL) {
        printf("No memory error\n");
        return NULL;
    }

    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;
}

/*设置内存的大小*/
int set_mem_size() {
    int size;
    if (flag) {
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size = ");
    scanf("%d", &size);
    if (size > 0) {
        mem_size = size;
        free_block->size = mem_size;
    }
    flag = 1;
    return 1;
}


/*设置分配算法*/
void set_algorithm() {
    int algorithm;
    printf("\t1 - Frist Fit \n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit\n");
    scanf("%d", &algorithm);
    if (algorithm >= 1 && algorithm <= 3) {
        ma_algorithm = algorithm;
    }
    //指定算法
    rearrange(ma_algorithm);
}

/*设置指定算法*/
void rearrange(int algorithm) {
    switch (algorithm) {
        case MA_FF:
            rearrange_FF();
            break;
        case MA_BF:
            rearrange_BF();
            break;
        case MA_WF:
            rearrange_WF();
            break;
        default:break;
    }
}


/*按照FF算法*/
void rearrange_FF() {

}

/*按照BF算法*/
void rearrange_BF() {

}

/*按照WF算法*/
void rearrange_WF() {

}

/*创建新的进程,主要是获取内存的申请数量*/
int new_process() {
    struct allocated_block *ab = (struct allocated_block *) malloc(sizeof(struct allocated_block));
    int size, ret;
    if (!ab) exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if (size > 0) ab->size = size;
    ret = allocate_mem(ab); //从空闲区分配内存 ret==1表示OK

    //如果此时allocated_block_head尚未赋值 ， 则赋值给他
    if ((ret == 1) && (allocated_block_head == NULL)) {
        allocated_block_head = ab;
        return 1;
    } else if (ret == 1) {
        ab->next = allocated_block_head;
        allocated_block_head = ab;
        return 2;
    } else if(ret == -1){
        printf("Allocation fail\n");
        free(ab);
        return -1;
    }

    return 3;
}

/*分配内存模块*/
int allocate_mem(struct allocated_block *ab){
    struct free_block_type *fpt , *pre;
    int request_size = ab->size;
    fpt = pre = free_block;

    /*DO SOMETHING*/

    return 1;
}

/*删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点*/
void kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process , pid = ");
    scanf("%d" , &pid);
    ab = find_process(pid);
    if(ab){
        free_mem(ab); /*释放ab所表示的分配区*/
        dispose(ab); /*释放ab数据结构节点*/
    }
}

/*将ab所表示的已分配区归还,并进行可能的合并*/
int free_mem(struct allocated_block *ab){
    int algorithm = ma_algorithm;

    struct free_block_type *fpt , *pre , *work;
    fpt=(struct free_block_type*)malloc(sizeof(struct free_block_type));
    if(!fpt) return -1;

    /*DO SOMETHING*/

    return 1;
}

/*释放数据结构节点*/
int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre , *ab;
    if (free_ab == allocated_block_head) { //释放第一个节点
        allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }
    pre = allocated_block_head;
    ab = allocated_block_head->next;
    while (ab != free_ab) {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}

/*显示当前的内存使用情况*/
int display_mem_usage(void){
    struct free_block_type *fpt = free_block;
    struct allocated_block *ab = allocated_block_head;
    if(fpt == NULL) return -1;
    printf("-------------------------------------------------------");
    
    /*显示空闲区*/
    printf("Free memory:\n");
    printf("%20s %20s\n" , "    Start_addr" , "     size");
    while (fpt) {
        printf("%20d %20d\n" , fpt->start_addr , fpt->size);
        fpt = fpt->next;
    }
    
    /*显示已分配区*/
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n" , "   Pid" , "     Process Name" , "start_addr" , "size");
    while (ab) {
        printf("%10d %20s %10d %10d\n" , ab->pid , ab->process_name , ab->start_addr , ab->size);
        ab = ab->next;
    }
    printf("--------------------------------------------------------");
    return 0;
}

/*根据pid寻找进程*/
struct allocated_block* find_process(int pid){
    return NULL;
}

/*显示菜单*/
void display_menu(){
    printf("\n");
    printf("1 - Set memory size(defualt = %d)\n" , DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algroithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");
}
