#include"thttpd.h"
int startup(const char* _ip,int _port) //创建监听套接字
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

void printf_log(const char* msg,int level)     //记录日志
{
//	#ifdef DEBUG
//	char* buf[10]={"NORMAL","WRONING","FATAL"};
//	printf("%s ..%s\n",msg,buf[level]);
//	#endif
	openlog("thttpd",LOG_PID|LOG_CONS|LOG_NDELAY,LOG_USER);
	switch(level)
	{
	case NORMAL:
		syslog(LOG_NOTICE,msg);
		break;
	case WRONING:
		syslog(LOG_WARNING,msg);
		break;
	case FATAL:
		syslog(LOG_ERR,msg);
		break;
	}
	closelog();
}


static int get_line(int sock,char* buf)   //按行读取请求报头
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


static void show_404(int sock)      //404错误处理
{
	clear_header(sock);
	char* msg="HTTP/1.0 404	Not Found\r\n";
	send(sock,msg,strlen(msg),0);         //发送状态行
	send(sock,"\r\n",strlen("\r\n"),0);      //发送空行

	struct stat st;
	stat("wwwroot/404.html",&st);
	int fd=open("wwwroot/404.html",O_RDONLY);
	sendfile(sock,fd,NULL,st.st_size);
	close(fd);
}

void echo_error(int sock,int err_code)    //错误处理
{
	switch(err_code)
	{
	case 403:
		break;
	case 404:
		show_404(sock);
		break;
	case 405:
		break;
	case 500:
		break;
	defaut:
		break;
	}
}


static int echo_www(int sock,const char * path,size_t s)  //处理非CGI的请求
{
	int fd=open(path,O_RDONLY);
	if(fd<0)
	{
		echo_error(sock,403);
		return 7;
	}

	char* msg="HTTP/1.0 200 OK\r\n";
	send(sock,msg,strlen(msg),0);         //发送状态行
	send(sock,"\r\n",strlen("\r\n"),0);      //发送空行
	
	if(sendfile(sock,fd,NULL,s)<0)
	{
		echo_error(sock,500);
		return 8;	
	}	
	close(fd);
	return 0;
}


static int excu_cgi(int sock,const char* method,\
		const char* path,const char* query_string) //处理CGI模式的请求
{
	char line[SIZE];
	int ret=0;
	int content_len=-1;
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
			ret=get_line(sock,line);
			if(strncmp(line,"Content-Length: ",16)==0)  //post的消息体记录正文长度的字段
			{
				content_len=atoi(line+16);	//求出正文的长度
			}
		}while(ret!=1&&(strcmp(line,"\n")!=0));
	}

	//将method、query_string、content_len导出为环境变量
	char method_env[SIZE/8];
	char query_string_env[SIZE/8];
	char content_len_env[SIZE/8];
	
	sprintf(method_env,"METHOD=%s",method);
	putenv(method_env);

	sprintf(query_string_env,"QUERY_STRING=%s",query_string);
	putenv(query_string_env);
	
	sprintf(content_len_env,"CONTENT_LEN=%d",content_len);
	putenv(content_len_env);


	int input[2];      //站在CGI程序的角度，创建两个管道
	int output[2];

	pipe(input);
	pipe(output);

	pid_t id=fork();

	if(id<0)
	{
		printf_log("fork failed",FATAL);
		echo_error(sock,500);
		return 9;
	}
	else if(id==0)     //子进程
	{
		close(input[1]);    //关闭input的写端      
		close(output[0]);  //关闭output的读端

		//将文件描述符重定向到标准输入标准输出
		dup2(input[0],0);
		dup2(output[1],1);

		execl(path,path,NULL);         //替换CGI程序
		exit(1);
	}
	else         //father
	{
		close(input[0]);      //关闭input的读端
		close(output[1]);     //关闭output的写端

		char ch='\0';
		if(strcasecmp(method,"POST")==0) //如果是post方法，则父进程需要给子进程发送参数
		{
			int i=0;
			for( i=0;i<content_len;i++)
			{
 				recv(sock,&ch,1,0);        //从sock里面一次读一个字符，总共读conten_len个字符
				write(input[1],&ch,1);
			}
		}
	
		char* msg="HTTP/1.0 200 OK\r\n\r\n";
		send(sock,msg,strlen(msg),0);

		//接收子进程的返回结果
		while(read(output[0],&ch,1))      //如果CGI程序结束，写端关闭，则读端返回0
		{
			send(sock,&ch,1,0);
		}
		waitpid(id,NULL,0);              //回收子进程
	}

	return 0;
}


int handler_msg(int sock)       //浏览器请求处理函数
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

	//获取请求方法和请求路径
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
		echo_error(sock,405);
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

	struct stat st;           
	if(stat(path,&st)<0)            //获取客户端请求的资源的相关属性
	{
		printf_log("stat path faile\n",FATAL);
		echo_error(sock,404);
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
		ret=excu_cgi(sock,method,path,query_string);           //处理CGI模式的请求
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


