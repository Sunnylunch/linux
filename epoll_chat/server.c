#define _GNU_SOURCE 1
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/epoll.h>


#define SIZE 1024
#define USER_LIMIT 65535
#define FD_LIMIT 65535

typedef struct client_data   //创建一个存储客户端数据结构体类型
{
	struct sockaddr_in addr;
	char *write_buf;
	char buf[SIZE];
	struct sockaddr_in *write_post;
}client_data;


int Startup(char* ip,int port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		printf("socket faild\n");
		exit(1);
	}
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);

	int op=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));

	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		printf("bind faild\n");
		exit(2);
	}

	if(listen(sock,5)<0)
	{
		printf("listen faild\n");
		exit(3);
	}
	return sock;
}

int setnonblocking(int fd)
{ 
	int old_option=fcntl(fd,F_GETFL);
	int new_option=old_option|O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
}


void Usage(const char* str)
{
	printf("Usage: %s ip port\n",str);
}

int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		Usage(argv[0]);
		return 1;
	}
	
	int sock=Startup(argv[1],atoi(argv[2]));
	int epollfd=epoll_create(256);
	if(epollfd<0)
	{
		printf("faild epoll_create\n");
		exit(4);
	}

	client_data* user=(client_data*)malloc(FD_LIMIT*sizeof(client_data));
	size_t user_count=0;
	int fd_Set[FD_LIMIT]={-1};

	struct epoll_event evs;
	evs.events=EPOLLIN|EPOLLET;    //注册读事件，ET模式
	evs.data.fd=sock;  
	epoll_ctl(epollfd,EPOLL_CTL_ADD,sock,&evs);

	while(1)
	{
		struct epoll_event evns[10];
		int maxevent=sizeof(evns)/sizeof(evns[0]);
		int num=0;
		int timeout=-1;
		switch(num=epoll_wait(epollfd,evns,maxevent,timeout))
		{
		case 0:
			printf("timeout\n");
			break;
		case -1:
			perror("epoll_wait\n");
			break;
		default:
			for(int i=0;i<num;i++)
			{
				int fd=evns[i].data.fd;
				if(fd==sock&&evns[i].events&EPOLLIN)
				{
					struct sockaddr_in peer;
					socklen_t len=sizeof(peer);
					int connf=accept(sock,(struct sockaddr*)&peer,&len);
					if(connf<0)
					{
						perror("accept faild\n");
						continue;
					}

					if(user_count>=USER_LIMIT)     //用户数量太多
					{
						const char* info="too many users\n";
						printf("%s",info);
						send(connf,info,strlen(info),0);
						close(connf);
						continue;
					}
					printf("ip:%s  connection\n",inet_ntoa(peer.sin_addr));

					setnonblocking(connf);
					evs.events=EPOLLIN|EPOLLRDHUP;    
					evs.data.fd=connf;  
					user[connf].addr=peer;
					user[connf].write_buf=NULL;
					fd_Set[user_count]=connf;
					user_count++;
					epoll_ctl(epollfd,EPOLL_CTL_ADD,connf,&evs);
				}
				else if(fd!=sock&&evns[i].events&EPOLLRDHUP)  //关闭连接
				{
					close(fd);
					fd_Set[i]=fd_Set[--user_count];
					epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,NULL);
					printf("ip:%s  close\n",inet_ntoa(user[fd].addr.sin_addr));
				}
				else if(fd!=sock&&evns[i].events&EPOLLIN)
				{
					memset(user[fd].buf,0,sizeof(user[fd].buf));
					int s=recv(fd,user[fd].buf,sizeof(user[fd].buf),0);
					if(s<0)
					{
				 		if(errno!=EAGAIN)  //读取出错，关闭连接
						{
				 			close(fd);
							fd_Set[i]=fd_Set[--user_count];
							epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,NULL);
						}
					}
					else if(s>0)
					{
				 		for(int j=0;j<user_count;j++)
						{
							if(fd_Set[j]==fd)
							{
								continue;
							}
							evs.events=EPOLLOUT;    
							evs.data.fd=fd_Set[j];
							epoll_ctl(epollfd,EPOLL_CTL_MOD,fd_Set[j],&evs);
							user[fd_Set[j]].write_buf=user[fd].buf;
							user[fd_Set[j]].write_post=&user[fd].addr;
						}
				 	}
				}
				else if(fd!=sock&&evns[i].events&EPOLLOUT)
				{
					if(NULL==user[fd].write_buf) 
					{
						continue;
					}

					char* ip=inet_ntoa((*user[fd].write_post).sin_addr);	
					char buf[SIZE+30];
					memset(buf,0,sizeof(buf));
					strcpy(buf,ip);
					strcat(buf ," #:");
					strcat(buf,user[fd].write_buf);
					int s=send(fd,buf,strlen(buf),0);
					
					user[fd].write_buf=NULL;
					user[fd].write_post=NULL;
					evs.events=EPOLLIN;    
					evs.data.fd=fd;
					epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&evs);
				} 
			}    //for
			break;
		}   //switch
	
	}	//while
	delete [] user;
	close(sock);
	return 0;
}
