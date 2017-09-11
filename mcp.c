/*************************************************************************
    > File Name: mcp.c
    > Author: ksgin
    > Mail: imqqyangfan@gmail.com 
    > Created Time: Mon Sep  4 11:26:16 2017
 ************************************************************************/

#include<stdio.h>


int main(int argc , char** argv){
  if(argc < 2) return 1;
  open(argv[0] , O_RONLY , 0);
}
