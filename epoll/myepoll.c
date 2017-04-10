#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/epoll.h>
#define _SIZE_ 128
typedef struct epoll_msg    //创建一个结构体类型，里面保存文件描述符，和一个缓冲区 
{
	int fd;
	char buf[_SIZE_];
}epoll_t,*epoll_p,**epoll_pp;

static void* allocator(int fd)
{
	 epoll_p buf=(epoll_p)malloc(sizeof(epoll_t));
	 if(NULL==buf)
	 {
		 perror("malloc");
		 exit(6);
	 }
	buf->fd=fd;
	return buf;
}


void delalloc(void* ptr)
{	
	if(NULL!=ptr)
	{
		free(ptr);
	}
}

int startup(const char* ip,int port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(1);
	}

	int opt=1;
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))<0)
	{
		perror("setsockopt");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{

		perror("bind");
		exit(3);
	}

	if(listen(sock,5)<0)
	{
		perror("listen");
		exit(4);
	}

	return sock;
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("usage:%s ip_local port_local\n",argv[0]);
		return 1;
	}
	int listen_sock=startup(argv[1],atoi(argv[2]));
	
	int epfd=epoll_create(256);
	if(epfd<0)
	{
		perror("epoll_create");
		exit(5);
	}

	struct epoll_event envs;
	envs.events=EPOLLIN|EPOLLET;
	envs.data.ptr=allocator(listen_sock);
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&envs);
	
	while(1)
	{
		int num=0;
		int timeout=-1;
		struct epoll_event evs[32];
		int max=32;
		switch((num=epoll_wait(epfd,evs,max,timeout)))
		{
		case 0:
			printf("timeout...");
			break;
		case -1:
			perror("epoll_wait");
			break;
		default:
			{
			//	printf("something....\n");
			//	break;

				int i=0;
				for(i=0;i<num;i++)
				{
					int fd=((epoll_p)(evs[i].data.ptr))->fd;
					if(fd==listen_sock&&evs[i].events&EPOLLIN)
					{
						struct sockaddr_in peer;
						socklen_t len=sizeof(peer);
						int connfd=accept(listen_sock,(struct sockaddr*)&peer,&len);
						if(connfd<0)
						{
							perror("accept");
							continue;
						}
						
                        envs.events=EPOLLIN;
						envs.data.ptr=allocator(connfd);
						epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&envs);
					}  //fi
					else if(fd!=listen_sock&&evs[i].events&EPOLLIN)
					{
						int s=read(fd,((epoll_p)(evs[i].data.ptr))->buf,_SIZE_-1);
						if(s>0)
						{
							char* buf=((epoll_p)(evs[i].data.ptr))->buf;
							buf[s]=0;
							printf("client# %s\n",buf);

							evs[i].events=EPOLLOUT;
							epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&evs[i]);
						}
						else if(s==0)
						{
							epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
							delalloc(evs[i].data.ptr);
							evs[i].data.ptr=NULL;
							close(fd);
						}
						else
						{
							perror("read");
						}
					} //fi
					else if(fd!=listen_sock&&evs[i].events&EPOLLOUT)
					{
						printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
						char *msg="http/1.0 200 ok\r\n\r\n<html><h1>姜淼==二哈</h1></html>";
						write(fd,msg,strlen(msg));
						delalloc(evs[i].data.ptr);
						epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&evs[i]);
						close(fd);
					}
				}//for
			}
		}
	}

	return 0;
}
