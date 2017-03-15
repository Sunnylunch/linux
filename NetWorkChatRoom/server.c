#define _GNU_SOURCE 1
#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<poll.h>
#include<fcntl.h>
#define BUFFSIZE 128
#define USER_LIMIT 5
#define FD_LIMIT 10
#include<cstring>

struct client_data
{
	struct sockaddr_in address;
	char* write_post;
	char buf[BUFFSIZE];
	struct sockaddr_in * write_addr;
};

int  setnonblocking(int fd)
{
	int old_option=fcntl(fd,F_GETFL);
	int new_option=old_option | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
}


int startup(const int port, char *ip)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("sock");
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



int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("usage: %s [IP] [PORT]\n",argv[0]);
		return 0;
	}

	int sock=startup(atoi(argv[2]),argv[1]);
	
	client_data *users=new client_data [FD_LIMIT];
	pollfd fds[USER_LIMIT+1];

	fds[0].fd=sock;
	fds[0].events=POLLIN|POLLERR;
	fds[0].revents=0;
	for(int i=1;i<USER_LIMIT+1;i++)
	{
		fds[i].fd=-1;
	}
	int user_count=0;

	while(1)
	{
		if(poll(fds,user_count+1,-1)<0)
		{
			printf("poll faliure\n");
			break;
		}

		if(fds[0].revents & POLLIN)
		{
			struct sockaddr_in peer;
			socklen_t len=sizeof(peer);

			int connfd=accept(sock,(struct sockaddr*)&peer,&len);
			if(connfd<0)
			{
 				perror( "accept");
				continue;
			}
			
			if(user_count>=USER_LIMIT)
			{
 				const char *info="User count reached limit";
				printf("%s\n",info);
				write(connfd,info,strlen(info));
				close(connfd);
				continue;
			}
			user_count++;
			users[connfd].address=peer;
			setnonblocking(connfd);
			fds[user_count].fd=connfd;
			fds[user_count].events=POLLIN|POLLERR|POLLRDHUP;
			fds[user_count].revents=0;
			printf("a new user,now have %d user\n",user_count);
		}
		
		int i=0;
		for(i=0;i<user_count+1;i++)
		{
			if(fds[i].revents & POLLERR)
			{
 				printf("get a error from %d\n",fds[i].fd);
				char err[100];
				socklen_t len=sizeof(err);
				memset(err,0,len*sizeof(char));
				if(getsockopt(fds[i].fd,SOL_SOCKET,SO_ERROR,&err,&len)<0)
				{
					printf("get socket option failed\n");
				}
				continue;
			}
			else if(fds[i].revents & POLLRDHUP)
			{
 				printf("%s:%d close\n",inet_ntoa(users[fds[i].fd].address.sin_addr),ntohs(users[fds[i].fd].address.sin_port));
				users[fds[i].fd]=users[fds[user_count].fd];
				close(fds[i].fd);
				fds[i]=fds[user_count];
				i--;
				user_count--;
			}
			else if(i>0&&fds[i].revents & POLLIN )
			{
				int connfd=fds[i].fd;
				memset(users[connfd].buf,0,BUFFSIZE);
				ssize_t siz=read(connfd,users[connfd].buf,BUFFSIZE-1);
			
				if(siz<0)
				{
 					users[fds[i].fd]=users[fds[user_count].fd];
					close(fds[i].fd);
					fds[i]=fds[user_count];
					i--;
					user_count--;
				}
				else if(siz>0)
				{

					users[connfd].buf[siz-1]=0;
					printf("read %d bytes of client_data [ %s ] from %s:%d\n",siz,users[connfd].buf,inet_ntoa(users[connfd].address.sin_addr),ntohs(users[connfd].address.sin_port));
 					int j=0;
					for(j=1;j<user_count+1;j++)
					{
						if(fds[j].fd!=connfd)
						{
 							fds[j].events |=~POLLIN;
							fds[j].events|=POLLOUT;
							users[fds[j].fd].write_post=users[connfd].buf;
							users[fds[j].fd].write_addr=&users[connfd].address;
						}
					}
				}
			}
			else if(fds[i].revents & POLLOUT)
			{
 				int connfd=fds[i].fd;
				if(!users[connfd].write_post)
				{
					continue;
				}
				struct sockaddr_in addr=*(users[connfd].write_addr);
			    char *ip=inet_ntoa(addr.sin_addr);
				char buf[BUFFSIZE+30]={0};
				strcpy(buf,ip);
				strcat(buf," say:");
				strcat(buf,users[connfd].write_post);
				int s=write(connfd,buf,BUFFSIZE+29);
				users[connfd].write_post=NULL;
				users[connfd].write_addr=NULL;
				fds[i].events|=~POLLOUT;
				fds[i].events|=POLLIN;
			}
		}
	}
	delete users;
	close(sock);
	return 0;
}
