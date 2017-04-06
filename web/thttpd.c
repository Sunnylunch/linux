#include"thttpd.h"
int startup(const char* _ip,int _port)
{
	assert(_ip);

	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		printf_log("socket failed",FATAL);
		exit(2);
	}

	int opt=1;                     
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(_port);
	local.sin_addr.s_addr=inet_addr(_ip);
	
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		printf_log("bind failed",FATAL);
		exit(3);
	}

	if(listen(sock,5)<0)
	{
		printf_log("listen failed",FATAL);
		exit(4);
	}

	return sock;
}

void printf_log(const char* msg,int level)
{
//	#ifdef DEBUG
	char* buf[10]={"NORMAL","WRONING","FATAL"};
	printf("%s ..%s\n",msg,buf[level]);
//	#endif
}

void echo_error()
{

}

static int get_line(int sock,char* buf)
{
	char ch='\0';
	int i=0;
	ssize_t ret=0;
	while(i<SIZE && ch!='\n')
	{
		ret=recv(sock,&ch,1,0);
		if(ret>0&&ch=='\r')
		{
			ssize_t s=recv(sock,&ch,1,MSG_PEEK);
			if(s>0&&ch=='\n')
			{
				recv(sock,&ch,1,0);
			}
			else
			{
				ch='\n';
			}
		}
		buf[i++]=ch;
	}
	buf[i]='\0';
	return i;
}



static void clear_header(int sock)    //清空消息报头
{
	char buf[SIZE];
	int ret=0;
	do
	{
		ret=get_line(sock,buf);
	}while(ret!=1&&(strcmp(buf,"\n")==0));
}


static int echo_www(int sock,const char * path,size_t s)
{
	int fd=open(path,O_RDONLY);
	if(fd<0)
	{
		echo_error();
		return 7;
	}

	char* msg="HTTP/1.0 200 OK\r\n";
	send(sock,msg,strlen(msg),0);         //发送状态行
	send(sock,"\r\n",strlen("\r\n"),0);      //发送空行
	
	if(sendfile(sock,fd,NULL,s)<0)
	{
		echo_error();
		return 8;	
	}
	
	close(fd);
	return 0;
}


static void excu_cgi(int sock,const char* method,/
		const char* path,const char* query_string) //处理CGI模式的请求
{
	char line[SIZE];
	int ret=0;
	int len=0;
	if(strcasecmp(method,"GET")==0)         //如果是GET方法的CGI
	{
		//清空消息报头
		clear_header(sock);
	}
	else              //POST方法的CGI
	{
		//获取post方法的参数大小
		do
		{
			ret=get_line();
			if(strncmp(line,"Content-Length: ",16)==0)  //post的消息体记录正文长度的字段
			{
				len=atoi(line+16);	//求出正文的长度
			}
		}while(ret!=1&&(strcmp(line,"\n")!=0))
	}

}


int handler_msg(int sock)
{
	char buf[SIZE];
	int count=get_line(sock,buf);
	int ret=0;
	char method[32];
	char url[SIZE];
	char *query_string=NULL;
	int i=0;
	int j=0;
	int cgi=0;

	while(j<count)
	{
		if(isspace(buf[j]))
		{
			break;
		}
		method[i]=buf[j];	
		i++;
		j++;
	}
	method[i]='\0';
	while(isspace(buf[j])&&j<SIZE)      //过滤空格
	{
		j++;
	}

	if(strcasecmp(method,"POST")&&strcasecmp(method,"GET"))
	{
		printf_log("method failed",FATAL);
		echo_error();
		ret=5;
		goto end;
	}

	if(strcasecmp(method,"POST")==0)
	{
		cgi=1;
	}

	i=0;
	while(j<count)
	{
		if(isspace(buf[j]))
		{
			break;
		}
		if(buf[j]=='?')
		{
			query_string=&url[i];
			query_string++;
			url[i]='\0';
		}
		else
			url[i]=buf[j];
		j++;
		i++;
	}
	url[i]='\0';

	if(strcasecmp(method,"GET")==0&&query_string!=NULL)
	{
		cgi=1;
	}

	//找到请求的资源路径
	char path[SIZE];
	sprintf(path,"wwwroot%s",url);       
	
	if(path[strlen(path)-1]=='/')              //判断浏览器请求的是不是目录
	{
		strcat(path,"index.html");	     //如果请求的是目录，则就把该目录下的首页返回回去
	}

	printf("path:%s\n",path);
	struct stat st;           
	if(stat(path,&st)<0)            //获取客户端请求的资源的相关属性
	{
		printf_log("stat path faile\n",FATAL);
		echo_error();
		ret=6;
		goto end;
	}
	else
	{
		if(S_ISDIR(st.st_mode))
		{
			strcat(path,"/index.html");
		}

		if(st.st_mode & (S_IXUSR | S_IXOTH | S_IXGRP))
		{
			cgi=1;
		}
	}

	if(cgi)       //请求的是可执行程序或者带参数用cgi程序处理
	{
		excu_cgi(sock,method,path,query_string);           //处理CGI模式的请求
	}
	else
	{ 
		clear_header(sock);
		ret=echo_www(sock,path,st.st_size);  //如果是GET方法，而且没有参数，请求的也不是可执行程序，则直接返回资源	
	}
	
end:
	close(sock);
	return ret;
}



