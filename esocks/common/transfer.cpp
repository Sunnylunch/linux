#include"transfer.h"

void *transfer(void* arg)
{
	TParam *param =(TParam*)arg;
	int in=param->in;
	int out=param->out;
	char buf[BUFSIZE];
	while(1)
	{
		int ret=recv(in,buf,sizeof(buf),0);
		if(ret<=0)
			break;
		ret=send_enc(out,buf,ret); //加密并发送
		if(ret<0)
			break;
	}
	close(in);
	close(out);
	return NULL;
}
