#ifndef _COMMON_H__
#define _COMMON_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<fcntl.h>
#include<errno.h>

#define MAXSIZE 1024
#define CLIENT_PORT 3306

int socket_create(const char* ip,const int port);
int socket_accept(int sock);
int socket_connect(const char* ip,const int port);
int recv_data(int sock,char* buf,int bufsize);
int send_response(int sock,int code);
void trimstr(char* str,int n);

#endif //_COMMON_H__
