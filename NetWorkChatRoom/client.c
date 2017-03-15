#define _GNU_SOURCE 1
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<poll.h>
#include<fcntl.h>
#define BUFFSIZE 128

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("usage: %s  [IP] [PORT]\n",argv[0]);
		return 0;
	}

	int connfd=socket(AF_INET,SOCK_STREAM,0);
	if(connfd<0)
	{
		perror("socket");
		return 1;
	}

	struct sockaddr_in dstaddr;
	dstaddr.sin_family=AF_INET;
	dstaddr.sin_port=htons(atoi(argv[2]));
	dstaddr.sin_addr.s_addr=inet_addr(argv[1]);

	if(connect(connfd,(struct sockaddr*)&dstaddr,sizeof(dstaddr))<0)
	{
		perror("connect");
		return 2;
	}

	struct pollfd fds[2];
	fds[0].fd=0;
	fds[0].events=POLLIN;
	fds[0].revents=0;
	fds[1].fd=connfd;
	fds[1].events=POLLIN | POLLRDHUP;
	fds[1].revents=0;
	
	int pipefd[2];
	if(pipe(pipefd)<0)
	{
		perror("pipe");
		return 3;
	}
	
	while(1)
	{
		int ret=poll(fds,2,-1);
		if(ret<0)
		{
			printf("poll faliure");
			break;
		}

		if(fds[0].revents & POLLIN)
		{
			splice(0,NULL,pipefd[1],NULL,32768,SPLICE_F_MOVE|SPLICE_F_MORE);

			splice(pipefd[0],NULL,connfd,NULL,32768,SPLICE_F_MOVE|SPLICE_F_MORE);

		}

		if(fds[1].revents & POLLIN)
		{
			char buf[BUFFSIZE+30]={0};
			int s=read(connfd,buf,sizeof(buf)-1);
			if(s>0)
			{
 				buf[s-1]=0;
				printf("%s\n",buf);
			}
		}
		else if(fds[1].revents & POLLRDHUP)
		{
			printf("server close connect\n");
			break;
		}
	}
	close(pipefd[0]);
	close(pipefd[1]);
	close(connfd);
	return 0;
}
