#include"common.h"

int recv_enc(int sock,char *buf,int len)
{
	char* cur=buf;
	int size=len; 
	while(size>0)        //固定的接收len个字节
	{
		int ret=recv(sock,cur,size,0);
		if(ret<=0)                         //接收失败
			return -1; 
		cur+=ret;
		size-=ret;
	}
	enc(buf,len);                  //对数据进行解密
	return 0;
}

int send_enc(int sock,char *buf,int len)
{
	enc(buf,len);      //对数据进行加密
	char *cur=buf;
	int size=len;
	while(size>0)     //固定发送len个字节
	{
		int ret=send(sock,cur,size,0);
		if(ret<=0)
			return -1;
		cur+=ret;
		size-=ret;
	}
	return 0;
}

void  enc(char* data,int len)    
{
	for(int i=0;i<len;i++)
	{
		data[i]^=7;               //与7进行异或       
	}
}
