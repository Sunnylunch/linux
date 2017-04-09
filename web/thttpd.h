#ifndef __THTTPD_H__
#define __THTTPD_H__

#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
#include<syslog.h>


#define NORMAL  0
#define WRONING 1
#define FATAL   2
#define SIZE 4096

int startup(const char* _ip,int _port);
int handler_msg(int sock);
void printf_log(const char* msg,int level);

#endif
