#include"ftpServer.h"
#include"pthread_pool/threadpool.cpp"
#include<pthread.h>

void Usage(const char *filename)
{
//	printf("Usage: %s servet_ip server_port \n",filename)
}

int main(int argc,char* argv[])
{
	daemon(1,0);
	if(argc!=3)
	{
		Usage(argv[0]);
		return -1;
	}

	int sock=socket_create(argv[1],atoi(argv[2]));
	threadpool<ftpServer> pool(8,1000);

	while(1)
	{
		int connf=socket_accept(sock);
		if(connf<0)
		{
			continue;
		}

		ftpServer ftp(connf);    //一个新的服务
		pool.append(ftp);   //将这个新的服务放到请求队列
	}
	close(sock);
	return 0;
}


