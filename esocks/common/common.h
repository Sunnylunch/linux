#ifndef _COMMON_H__
#define _COMMON_H__
#include<sys/types.h>
#include<sys/socket.h>
int recv_enc(int sock,char *buf,int len);    //接收数据并解密
int send_enc(int sock,char *buf,int len);    //发送数据并加密
void  enc(char* data,int len);				 //加密或解密数据

#endif //_COMMON_H__
