#include"server.h"


int main(int argc,char *argv[])
{
	daemon(0,0);
	if(2!=argc)            //./server port
	{
		return -1;
	}

	int lis_sock=socket(AF_INET,SOCK_STREAM,0);
	assert(-1!=lis_sock);

	sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(argv[1]));
	local.sin_addr.s_addr=INADDR_ANY;

	int op=1;
	setsockopt(lis_sock,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
	
	assert(0==bind(lis_sock,(struct sockaddr*)&local,sizeof(local)));
	assert(0==listen(lis_sock,5));

	while(1)
	{
		sockaddr_in peer;
		socklen_t len=sizeof(peer);
		int in=accept(lis_sock,(struct sockaddr*)&peer,&len);
		if(in==-1)
			continue;
		pthread_t tid;
		pthread_create(&tid,NULL,handler,(void*)in);  //创建一个线程处理处理客户端的请求
	}
	close(lis_sock);
	return 0;
}

void* handler(void* arg)
{
	pthread_detach(pthread_self());
	int in=(int)arg;

	//接受客户端发送过来的请求
	SKReq req;
	if(0!=recv_enc(in,(char*)&req,sizeof(req))) //接收请求并解密
	{
		close(in);
		return NULL;
	}
	if(req.ver!=5||req.nmethods!=1)  //判断socks的版本，客户端接受的验证方法的数量
	{
		close(in);
		return NULL;
	}
	char methods[8];
	if(0!=recv_enc(in,methods,req.nmethods))  //获取客户端的验证方法
	{
		close(in);
		return NULL;
	}
	if(0!=methods[0])  //0表示匿名访问
	{
		close(in);
		return NULL;
	}
	
	//服务器做出响应
	SKRep rep;
	rep.ver=5;           //服务器使用socks5
	rep.method=0;        //接受匿名访问
	if(0!=send_enc(in,(char*)&rep,sizeof(rep)))  //发送回应并加密
	{
		close(in);
		return NULL;
	}

	sockaddr_in outAddr;
	outAddr.sin_family=AF_INET;
	
	//接受客户端发送过来的请求数据包
	AddrReq areq;
	if(0!=recv_enc(in,(char*)&areq,sizeof(areq)))
	{
		close(in);
		return NULL;
	}

	if(areq.ver!=5||areq.cmd!=1||areq.rsv!=0)  //socks版本，cmd为1表示connect
	{
		close(in);
		return NULL;
	}

	if(areq.atype==1)    //1表示传递的是IP4地址
	{
		if(0!=recv_enc(in,(char*)&outAddr.sin_addr.s_addr,sizeof(outAddr.sin_addr.s_addr)))
		{
			close(in);
			return NULL;
		}
	}
	else if(areq.atype==3)   //3表示传递过来的是要访问的域名
	{
		char len;    
		if(0!=recv_enc(in,(char*)&len,1))  //获取域名的长度
		{
			close(in);
			return NULL;
		}
		char domain[256];
		if(0!=recv_enc(in,domain,len))   //获取域名
		{
			close(in);
			return NULL;
		}
		domain[len]='\0';
		hostent *host=gethostbyname(domain);   //获取域名对应的主机信息
		if(host==NULL||host->h_addrtype!=AF_INET||host->h_length<=0)     //判断获取的是否正确
		{
			close(in);
			return NULL;
		}
		memcpy(&outAddr.sin_addr.s_addr,host->h_addr_list[0],sizeof(outAddr.sin_addr.s_addr));
	}
	else          //ip6
	{
		close(in);
		return NULL;
	}

	if(0!=recv_enc(in,(char*)&outAddr.sin_port,sizeof(outAddr.sin_port)))     //获取端口
	{
		close(in);
		return NULL;
	}

	int out=socket(AF_INET,SOCK_STREAM,0);
	if(out<0)
	{
		close(in);
		return NULL;
	}
	if(connect(out,(struct sockaddr*)&outAddr,sizeof(outAddr))<0)    //服务器与要访问的ip建立连接
	{
		close(in);
		close(out);
		return NULL;
	}


	//服务器对客户端做出响应
	AddrRep arep;
	arep.ver=5;
	arep.rep=0;      //successful
	arep.rsv=0;
	arep.atype=1;  //表示IP4地址
	sockaddr_in local;
	socklen_t len=sizeof(local);
	if(0!=getsockname(out,(struct sockaddr*)&local,&len))    //获取out这个套接字的相关信息
	{
		close(in);
		close(out);
		return NULL;
	}
		
	memcpy(&arep.addr,&local.sin_addr.s_addr,sizeof(arep.addr));
	memcpy(&arep.port,&local.sin_port,sizeof(arep.port));
	if(send_enc(in,(char*)&arep,sizeof(arep))<0)  //发送响应
	{
		close(in);
		close(out);
		return NULL;
	}
	//创建两个线程进行数据传输
	TParam treq={in,out,0};  //从in读取，发到out
	TParam trep={out,in,0};  //从out读取，发到in
	pthread_create(&treq.tid,NULL,transfer,(void*)&treq);
	pthread_create(&trep.tid,NULL,transfer,(void*)&trep);

	pthread_join(treq.tid,NULL);
	pthread_join(trep.tid,NULL);
	return NULL;
}
