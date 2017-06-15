#ifndef _TRANSFER_H__
#define _TRANSFER_H__
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include"common.h"

#define BUFSIZE 1024*1024

typedef struct TParam		   	//线程函数的参数
{
	int in;			    //读文件描述符
	int out;		    //写文件描述符
	pthread_t tid;
}TParam;

void *transfer(void* arg);    //转发数据

#endif //_TREANSFER_H__
