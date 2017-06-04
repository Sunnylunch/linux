#define _GNU_SOURCE 1
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<sys/epoll.h>
#include<fcntl.h>

#define SIZE 1024

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("Usage: %s server_ip server_port\n",argv[0]);
		return 1;
	}
	
	int sock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in peer;
	peer.sin_family=AF_INET;
	peer.sin_port=htons(atoi(argv[2]));
	peer.sin_addr.s_addr=inet_addr(argv[1]);

	if(connect(sock,(struct sockaddr*)&peer,sizeof(peer))<0)
	{
		printf("connection failed\n");
		return 2;
	}
	
	int epollfd=epoll_create(10);
	struct epoll_event evs;
	evs.events=EPOLLIN;
	evs.data.fd=0;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,0,&evs);

	evs.events=EPOLLIN|EPOLLRDHUP;
	evs.data.fd=sock;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,sock,&evs);

	char read_buf[SIZE];
	int pipefd[2];
	int ret=pipe(pipefd);
	assert(-1!=ret);

	while(1)
	{
		struct epoll_event evn[2]={0};
		int num=0;
		num=epoll_wait(epollfd,evn,2,-1);
		if(num<0)
		{
			printf("epoll_wait failure\n");
			break;
		}
		int i=0;
		for(i=0;i<num;i++)
		{
			if(evn[i].data.fd==sock&&evn[i].events&EPOLLRDHUP)
			{
				printf("server close the connection\n");
				close(epollfd);
				return 0;
			}
			else if(evn[i].data.fd==sock&&evn[i].events&EPOLLIN)
			{
				memset(read_buf,0,sizeof(read_buf));
				recv(sock,read_buf,sizeof(read_buf)-1,0);
				printf("%s",read_buf);
			}		
			else if(evn[i].data.fd==0&&evn[0].events&EPOLLIN)
			{
				ret=splice(0,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE | SPLICE_F_MOVE);
				ret=splice(pipefd[0],NULL,sock,NULL,32768,SPLICE_F_MORE | SPLICE_F_MOVE);
			}
		}
	}
	return 0;
}
