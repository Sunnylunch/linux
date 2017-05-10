#include"thttpd.c"

struct task_t
{
	stCoRoutine_t *co;
	int fd;
};


static int g_listen_fd=-1;
static stack<task_t*> g_readwrite;
static int SetNonBlock(int iSock)
{
	int iFlags;
	iFlags=fcntl(iSock,F_GETFL,0);
	iFlags |=O_NONBLOCK;
	iFlags |=O_NDELAY;
	int ret=fcntl(iSock,F_SETFL,iFlags);
	return ret;
}

static void *readwrite_routine(void *arg)
{
	co_enable_hook_sys();
	task_t *co=(task_t*)arg;
	char buf[1024*16];
	
	while(1)
	{
		if(-1==co->fd)
		{
			g_readwrite.push(co);
			co_yield_ct();
			continue;
		}

		int fd=co->fd;
		co->fd=-1;
		
		while(1)
		{
			struct pollfd pf={0};
			pf.fd=fd;
			pf.events=(POLLIN|POLLERR|POLLHUP);
			co_poll(co_get_epoll_ct(),&pf,1,1000);
			handler_msg(fd);
		}
	}
	return 0;
}


int co_accept(int fd,struct sockaddr *addr,socklen_t *len);
static void *accept_routine(void *)
{
	co_enable_hook_sys();

	while(1)
	{
		if(g_readwrite.empty())
		{
			struct pollfd pf={0};
			pf.fd=-1;
			poll(&pf,1,1000);
			continue;
		}

		struct sockaddr_in addr;
		memset(&addr,0,sizeof(addr));
		socklen_t len=sizeof(addr);

		int fd=co_accept(g_listen_fd,(struct sockaddr *)&addr,&len);
		if(fd<0)
		{
			struct pollfd pf={0};
			pf.fd=g_listen_fd;
			pf.events=(POLLIN|POLLERR|POLLHUP);
			co_poll(co_get_epoll_ct(),&pf,1,1000);
			continue;
		}
		if(g_readwrite.empty())
		{
			close(fd);
			continue;
		}

		SetNonBlock(fd);
		task_t *co=g_readwrite.top();
		co->fd=fd;
		co_resume(co->co);
	}
	return 0;
}



int main(int argc,char* argv[])
{
//	daemon(1,0);
	if(argc!=5)
	{
		printf_log("Usage: [local_ip] [local_port] [proccnt] [cnt]\n",FATAL);
		return 1;
	}

	g_listen_fd=startup(argv[1],atoi(argv[2]));     //创建一个全局的监听套接字

	int cnt=atoi(argv[3]);                          //获取每个进程应创建协程的数量
	int proccnt=atoi(argv[4]);                      //获取总的进程的数量
	

	SetNonBlock(g_listen_fd);
	int k=0;
	for(k=0;k<proccnt;k++)               
	{
		pid_t pid=fork();            
		if(pid>0)
		{
			continue;
		}
		else if(pid<0)
		{
			break;
		}
		int i=0;
		for(i=0;i<cnt;i++)   //每个进程创建cnt个协程     
		{
			task_t *task=(task_t*)calloc(1,sizeof(task_t));
			task->fd=-1;
			co_create(&(task->co),NULL,readwrite_routine,task); 
			co_resume(task->co);
		}
		
		stCoRoutine_t *accept_co=NULL;
		co_create(&accept_co,NULL,accept_routine,0);
		co_resume(accept_co);
		
		co_eventloop(co_get_epoll_ct(),0,0);
		exit(0);
	}
	
	return 0;
}
