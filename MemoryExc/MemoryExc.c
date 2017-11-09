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

    //设置初始化空闲内存块的信息
    fb->size = DEFAULT_MEM_SIZE;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    
    return fb;
}

/*设置内存的大小*/
int set_mem_size(struct free_block_type* free_block , int *mem_size , int *flag) {
    int size;
    if (*flag) {
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size = ");
    scanf("%d", &size);
    if (size > 0) {
        *mem_size = size;
        free_block->size = *mem_size;
    }
    *flag = 1;
    return 1;
}


/*设置分配算法*/
void set_algorithm(int *ma_algorithm , struct free_block_type** free_block) {
    int algorithm;
    printf("\t1 - Frist Fit \n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit\n");
    scanf("%d", &algorithm);
    if (algorithm >= 1 && algorithm <= 3) {
        *ma_algorithm = algorithm;
    }
    //指定算法
    rearrange(*ma_algorithm , free_block);
}

/*设置指定算法*/
void rearrange(int algorithm , struct free_block_type** free_block) {
    switch (algorithm) {
        case MA_FF:
            rearrange_FF(free_block);
            break;
        case MA_BF:
            rearrange_BF(free_block);
            break;
        case MA_WF:
            rearrange_WF(free_block);
            break;
        default:break;
    }
}


/*按照FF算法（最先适配法）*/
/*将其按照地址顺序排序*/
void rearrange_FF(struct free_block_type** free_block) {
    //只有一个或者没有内存块的时候不需要排序
    struct free_block_type* fpt = *free_block;
    if (!fpt) return;
    struct free_block_type* net = fpt;
    if (!net) return;
    
    //根据内存大小排序
    while (fpt->next) {
        while (net->next) {
            if (net->next->start_addr < net->start_addr) {
                int tmp = net->size;
                net->size = net->next->size;
                net->next->size = tmp;
                
                tmp = net->start_addr;
                net->start_addr = net->next->start_addr;
                net->next->start_addr = tmp;
            }
            net = net->next;
        }
        fpt = fpt->next;
        net = *free_block;
    }
}

/*按照BF算法（最佳适配法）*/
void rearrange_BF(struct free_block_type** free_block) {
    //只有一个或者没有内存块的时候不需要排序
    struct free_block_type* fpt = *free_block;
    if (!fpt) return;
    struct free_block_type* net = fpt;
    if (!net) return;

    //根据内存大小排序
    while (fpt->next) {
        while (net->next) {
            if (net->next->size < net->size) {
                int tmp = net->size;
                net->size = net->next->size;
                net->next->size = tmp;
                
                tmp = net->start_addr;
                net->start_addr = net->next->start_addr;
                net->next->start_addr = tmp;
            }
            net = net->next;
        }
        fpt = fpt->next;
        net = *free_block;
    }
}

/*按照WF算法（最坏适配法）*/
void rearrange_WF(struct free_block_type** free_block) {
    //只有一个或者没有内存块的时候不需要排序
    struct free_block_type* fpt = *free_block;
    if (!fpt) return;
    struct free_block_type* net = fpt;
    if (!net) return;
    
    //根据内存大小排序
    while (fpt->next) {
        while (net->next) {
            if (net->next->size > net->size) {
                int tmp = net->size;
                net->size = net->next->size;
                net->next->size = tmp;
                tmp = net->start_addr;
                net->start_addr = net->next->start_addr;
                net->next->start_addr = tmp;
            }
            net = net->next;
        }
        fpt = fpt->next;
        net = *free_block;
    }
}

/*创建新的进程,主要是获取内存的申请数量*/
int new_process(int *pid , struct free_block_type** free_block ,
                struct allocated_block** allocated_block_head ,int* mem_size) {
    struct allocated_block *ab = (struct allocated_block *) malloc(sizeof(struct allocated_block));
    int size;
    if (!ab) exit(-5);
    ab->next = NULL;
    (*pid)++;
    sprintf(ab->process_name, "PROCESS-%02d", *pid);
    ab->pid = *pid;
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if (size > 0) ab->size = size;
    int ret = allocate_mem(ab , free_block , mem_size , allocated_block_head , pid); //从空闲区分配内存 ret==1表示OK

    //如果此时allocated_block_head尚未赋值 ， 则赋值给他
    if ((ret == 1) && (*allocated_block_head == NULL)) {
        *allocated_block_head = ab;
        return 1;
    } else if (ret == 1) {
        ab->next = *allocated_block_head;
        *allocated_block_head = ab;
        return 2;
    } else if(ret == -1){
        printf("Allocation fail\n");
        free(ab);
        return -1;
    }

    return 3;
}

/*分配内存模块*/
int allocate_mem(struct allocated_block *ab, struct free_block_type** free_block , int *mem_size , struct allocated_block** allocated_block_head , int *pid){
    
    //根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
    // 1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
    // 2. 找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
    // 3. 找不可满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术，进行空闲分区的合并，然后再分配
    // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序
    // 5. 分配成功则返回1，否则返回-1
    
    struct free_block_type *fpt , *pre;
    int request_size = ab->size;
    fpt = pre = *free_block;

    /*DO SOMETHING*/
    while (fpt) {
        if (fpt->size >= request_size) { //找到可满足空闲分区
            if (fpt->size - request_size >= MIN_SLICE) { //分配后足够大
                
                ab->start_addr = fpt->start_addr;
                
                //分割内存
                fpt->size -= request_size;
                fpt->start_addr += request_size;
            } else {
                ab->size = fpt->size;
                ab->start_addr = fpt->start_addr;
                
                //修复空闲分区链表
                if(*free_block == fpt){
                    *free_block = fpt->next;
                } else {
                    pre->next = fpt->next;
                }
            }
            *mem_size -= ab->size;  //减少mem_size
            return 1;
        }
        pre = fpt;
        fpt = fpt->next;
    }
    
    if(!fpt){//fpt == null 表示没有找到需要的内存块，此时需要进行内存紧缩技术
        if(*mem_size >= ab->size){
            if(*mem_size - ab->size >= MIN_SLICE){
                free_memory_rearrage(*mem_size-ab->size, ab->size, free_block, mem_size, pid, allocated_block_head);
            }else {
                free_memory_rearrage(0, ab->size, free_block, mem_size, pid, allocated_block_head);
            }
            return 0;
        }
    }
    //没内存了
    return -1;
}

/*删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点*/
void kill_process(int ma_algorithm , struct allocated_block **allocated_block_head , int* mem_size, struct free_block_type** free_block){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process , pid = ");
    scanf("%d" , &pid);
    ab = find_process(pid , *allocated_block_head);
    if(ab){
        free_mem(ab , ma_algorithm , mem_size , free_block); /*释放ab所表示的分配区*/
        dispose(ab , allocated_block_head); /*释放ab数据结构节点*/
    }
}

/*将ab所表示的已分配区归还,并进行可能的合并*/
int free_mem(struct allocated_block *ab, int ma_algorithm , int* mem_size , struct free_block_type** free_block){

    struct free_block_type *fpt , *pre , *work;
    fpt=(struct free_block_type*)malloc(sizeof(struct free_block_type));
    if(!fpt) return -1;
    
    /*DO SOMETHING*/
    *mem_size += ab->size; //free_mem 增加
    
    fpt->size = ab->size;
    fpt->start_addr = ab->start_addr;
    fpt->next = NULL;
    
    //按FF分配
    rearrange(MA_FF , free_block);
    
    pre = NULL;
    work = *free_block;
    
    if (!work) {
        *free_block = fpt;
        return 1;
    }
    
    //插入空闲链表头部
    if(fpt->start_addr < work->start_addr){
        if (!work) {
            *free_block = fpt; //free_block为空时
        }else{
            fpt->next = *free_block;
            *free_block = fpt;
            work = fpt->next;
            if (fpt->start_addr + fpt->size == work->start_addr) { //如果两块地址相连，则合并
                fpt->size += work->size;
                fpt->next = work->next;
                free(work);
            }
        }
    } else {
        //查找插入位置
        while((work!=NULL)&&(fpt->start_addr>work->start_addr))
        {
            pre=work;
            work=work->next;
        }
        
        if (!work) { //此时插入末尾
            pre->next = fpt;
            if (pre->start_addr + pre->size == fpt->start_addr) {
                pre->size += fpt->size;
                pre->next = NULL;
                free(fpt);
            }
        } else { //找到了合适的位置，work地址大于fpt , pre地址小于fpt ,则插入两者之间
            fpt->next = work;
            pre->next = fpt;
            if (pre->start_addr + pre->size == fpt->start_addr
                && fpt->start_addr + fpt->size == work->start_addr) { //插入之后三个地址相连，合并三个区块
                pre->size += fpt->size + work->size;
                pre->next = work->next;
                free(fpt);
                free(work);
            } else if(pre->start_addr + pre->size == fpt->start_addr){ //只与前边相连
                pre->size += fpt->size;
                pre->next = fpt->next;
                free(fpt);
            } else if(fpt->start_addr + fpt->size == work->start_addr){ //只与后边相连
                fpt->size += work->size;
                fpt->next = work->next;
                free(work);
            }
        }
        
    }
    
    rearrange(ma_algorithm , free_block); //按照所选算法分配
    return 1;
}

/*释放数据结构节点*/
int dispose(struct allocated_block *free_ab , struct allocated_block **allocated_block_head){
    struct allocated_block *pre , *ab;
    if (free_ab == *allocated_block_head) { //释放第一个节点
        *allocated_block_head = (*allocated_block_head)->next;
        free(free_ab);
        return 1;
    }
    pre = *allocated_block_head;
    ab = (*allocated_block_head)->next;
    while (ab != free_ab) {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}

/*显示当前的内存使用情况*/
int display_mem_usage(struct free_block_type* free_block , struct allocated_block* allocated_block_head){
    struct free_block_type *fpt = free_block;
    struct allocated_block *ab = allocated_block_head;
//    if(fpt == NULL) return -1;
    printf("-------------------------------------------------------\n");
    
    /*显示空闲区*/
    printf("Free memory:\n");
    printf("%20s  %20s\n" , "             Start_addr" , "size");
    while (fpt) {
        printf("%20d %20d\n" , fpt->start_addr , fpt->size);
        fpt = fpt->next;
    }
    
    /*显示已分配区*/
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n" , "Pid" , "Process Name" , "          start_addr" , "size");
    while (ab) {
        printf("%10d %20s %10d %10d\n" , ab->pid , ab->process_name , ab->start_addr , ab->size);
        ab = ab->next;
    }
    printf("--------------------------------------------------------\n");
    return 0;
}

/*根据pid寻找进程*/
struct allocated_block* find_process(int pid  , struct allocated_block *allocated_block_head){
    struct allocated_block *p = allocated_block_head;
    while(p)
    {
        if(p->pid==pid)
            return p;
        p=p->next;
    }
    return p;
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

//内存紧缩处理
/*内存紧缩：将各个占用分区向内存一端移动，然后将各个空闲分区合并成为一个空闲分区。*/
void free_memory_rearrage(int memory_reduce_size,int allocated_size ,
                          struct free_block_type** free_block ,
                          int* mem_size , int* pid,
                          struct allocated_block** allocated_block_head){
    struct free_block_type *p1,*p2;
    struct allocated_block *a1,*a2;
    if(memory_reduce_size!=0) //分配完还有小块空间
    {
        p1=*free_block;
        p2=p1->next;
        
        p1->start_addr=0;
        p1->size=memory_reduce_size;
        p1->next=NULL;
        
        *mem_size=memory_reduce_size;  //
    }
    else
    {
        p2=*free_block;
        free_block=NULL;
        mem_size=0;
    }
    while(p2!=NULL)//释放节点
    {
        p1=p2;
        p2=p2->next;
        free(p1);
    }
    //allocated_block 重新修改链接
    a1=(struct allocated_block *)malloc(sizeof(struct allocated_block));
    a1->pid=*pid;
    a1->size=allocated_size;
    a1->start_addr=memory_reduce_size; //已申请的开始地址，从memory_reduce_size开始
    sprintf(a1->process_name, "PROCESS-%02d", *pid);
    
    a1->next=*allocated_block_head;
    a2=*allocated_block_head;
    *allocated_block_head=a1;
    
    while(a2!=NULL)
    {
        a2->start_addr=a1->start_addr+a1->size;
        a1=a2;
        a2=a2->next;
    }
}

void do_exit(struct free_block_type** free_block , struct allocated_block** allocated_block_head){
    struct free_block_type *p1,*p2;
    struct allocated_block *a1,*a2;
    p1=*free_block;
    if(p1!=NULL)
    {
        p2=p1->next;
        for(;p2!=NULL;p1=p2,p2=p2->next)
        {
            free(p1);
        }
        free(p1);
    }
    a1=*allocated_block_head;
    if(a1!=NULL)
    {
        a2=a1->next;
        for(;a2!=NULL;a1=a2,a2=a2->next)
        {
            free(a1);
        }
        free(a1);
    }
}
