#include"thttpd.c"

static void* msg_request(void *arg)
{
	int sock=(int)arg;
	pthread_detach(pthread_self());
	return (void*)handler_msg(sock);
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("Usage:%s [local_ip] [local_port]\n",argv[0]);
		return 1;
	}
	
	int lis_sock=startup(argv[1],atoi(argv[2]));

	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len=sizeof(peer);
		
		int sock=accept(lis_sock,(struct sockaddr*)&peer,&len);
		
		if(sock<0)
		{
			printf_log("accept failed",WRONING);
			continue;
		}
		
		pthread_t tid;
		if(pthread_create(&tid,NULL,msg_request,(void*)sock)>0)
		{
			printf_log("pthread_create failed",WRONING);
			close(sock);
		}
	}

	return 0;
}
