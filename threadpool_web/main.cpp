#include"thttpd.h"
#include"locker.h"
#include"threadpool.cpp"

#define MAX_FD 65535
int main(int argc,char* argv[])
{
	daemon(1,0);
	if(argc!=3)
	{
		printf_log("argv failed\n",FATAL);
		return 1;
	}

	int lis_sock=startup(argv[1],atoi(argv[2]));
	
	threadpool< http > pool(8,1000);

	http users[MAX_FD];
	for(int i=0;i<MAX_FD;i++)
	{
		users[i].init(i);
	}

	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len=sizeof(peer);
		http users[MAX_FD];

		int sock=accept(lis_sock,(struct sockaddr*)&peer,&len);
		
		if(sock<0)
		{
			printf_log("accept failed\n",FATAL);
			continue;
		}
		pool.append(&users[sock]);
	}
	return 0;
}
