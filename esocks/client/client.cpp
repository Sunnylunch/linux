#include"client.h"

in_addr_t ip;
int port;

int main(int argc,char* argv[])   
{
	daemon(0,0);
	if(argc!=4)                  //./client port peer_server peer_port
	{
		return -1;
	}

	int lis_sock=socket(AF_INET,SOCK_STREAM,0);
	assert(-1!=lis_sock);

	sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(argv[1]));
	local.sin_addr.s_addr=INADDR_ANY;     //表示本主机地址，相当于'0.0.0.0'
	
	int op=1;
	setsockopt(lis_sock,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
	assert(0==bind(lis_sock,(struct sockaddr*)&local,sizeof(local)));
	assert(0==listen(lis_sock,5));
	
	ip=inet_addr(argv[2]);   //服务器端的IP
	port=htons(atoi(argv[3])); //服务器的端口

	while(1)
	{
		sockaddr_in peer;
		socklen_t len=sizeof(peer);
		int in=accept(lis_sock,(struct sockaddr*)&peer,&len);
		if(-1==in)
			continue;
		pthread_t tid;
		pthread_create(&tid,NULL,handler,(void*)in);
	}
	close(lis_sock);
	return 0;
}


void* handler(void *arg)
{
	pthread_detach(pthread_self());
	int in=(int)arg;
	
	int out=socket(AF_INET,SOCK_STREAM,0);
	if(out<0)
	{
		close(in);
		close(out);
		return NULL;
	}

	sockaddr_in serAddr;
	serAddr.sin_family=AF_INET;
	serAddr.sin_port=port;
	serAddr.sin_addr.s_addr=ip;

	if(connect(out,(struct sockaddr*)&serAddr,sizeof(serAddr))<0)
	{
		close(in);
		close(out);
		return NULL;
	}

	//创建两个线程进行数据传输
	TParam req={in,out,0};  //用于转发浏览器的请求
	TParam rep={out,in,0};  //用于转发服务器的响应
	pthread_create(&req.tid,NULL,transfer,(void*)&req);
	pthread_create(&rep.tid,NULL,transfer,(void*)&rep);
	pthread_join(req.tid,NULL);
	pthread_join(rep.tid,NULL);
	return NULL;
}

