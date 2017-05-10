#ifndef __THTTPD_H__
#define __THTTPD_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<assert.h>

#include<unistd.h>
#include<pthread.h>
#include<syslog.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>

/////////////////////////////////
//协程头文件 
#include"co_routine.h"
#include<sys/time.h>
#include<stack>
#include<signal.h>
#include<sys/wait.h>
#include<ctype.h>
#include<sys/un.h>
/////////////////////////////////
using namespace std;

#define NORMAL  0
#define WRONING 1
#define FATAL   2
#define SIZE 4096

int startup(const char* _ip,int _port);      //创建监听套接字
int handler_msg(int sock);                   //浏览器请求处理函数
void printf_log(const char* msg,int level);  //日志打印函数 

#endif
