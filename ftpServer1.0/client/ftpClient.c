#include"ftpClient.h"

int read_reply(int sock_ctl)       //读取服务器的回复
{
	int status=0;
	if(recv(sock_ctl,&status,sizeof(status),0)<0)
	{
		printf("client:error reading message from server.\n");
		return -1;
	}
	return ntohl(status);
}

void print_reply(int status)     //打印回复信息
{
	switch(status)
	{
	case 220:
		printf("220 welcome,server ready.\n");
		break;
	case 221:
		printf("221 bye!\n");
		break;
	case 226:
		printf("226 close data connection. requested fileaction successful.\n");
		break;
	case 550:
		printf("550 request action not taken.file unavailable.\n");
		break;
	default:
		printf("unknown error.\n");
		break;
	}
}

int ftpclient_read_cmd(char* buf,size_t size,struct command *cmd) //读取客户端输入的命令
{
	memset(cmd->code,0,sizeof(cmd->code));
	memset(cmd->arg,0,sizeof(cmd->arg));
	printf("ftpclient> ");  //输入提示符
	fflush(stdout);
	read_input(buf,size);
	char *arg=NULL;
	arg=strtok(buf," ");     //将读取的内容以空格分隔
	arg=strtok(NULL," ");    //指向输入的命令所带的参数
	if(NULL!=arg)
		strncpy(cmd->arg,arg,strlen(arg));

	if(strncmp(buf,"ls",4)==0)
		strcpy(cmd->code,"LIST");
	else if(strncmp(buf,"get",3)==0)
		strcpy(cmd->code,"RETR");
	else if(strncmp(buf,"bye",4)==0)
		strcpy(cmd->code,"QUIT");
	else
		return -1;

	//将这个命令再存到buf中
	memset(buf,0,size);
	strcpy(buf,cmd->code);
	if(NULL!=arg)        //如果命令带参数的话，追加到命令后面
	{	
		strcat(buf," ");
		strncat(buf,cmd->arg,strlen(cmd->arg));
	}
	return 0;
}


int ftpclient_get(int sock_data,char *filename)
{
	int fd=open(filename,O_CREAT|O_WRONLY,0664);	
	while(1)
	{
		ssize_t s=0;
		char data[MAXSIZE];
		memset(data,0,sizeof(data));
		s=recv(sock_data,data,sizeof(data),0);
		if(s<=0)
		{
			if(s<0)
				perror("error");
			break;
		}
		write(fd,data,s);
	}
	close(fd);
	return 0;
}

int ftpclient_open_conn(int sock_ctl)   //打开数据连接
{
	int sock_listen=socket_create("0.0.0.0",CLIENT_PORT);   //创建一个数据连接
	int ack=1;
	//给服务器发送一个确认，告诉服务器客户端创建好了一条数据链路
	if(send(sock_ctl,(char*)&ack,sizeof(ack),0)<0) 
	{
		printf("client:ack write error:%d\n",errno);
		return -1;
	}
	int sock_data=socket_accept(sock_listen);
	close(sock_listen);
	return sock_data;     
}

int ftpclient_list(int sock_ctl,int sock_data) //处理list命令
{
	int tmp=0;
	if(recv(sock_ctl,&tmp,sizeof(tmp),0)<0)    //等待服务器连接成功发送过来一个信号
	{
		perror("client:error reading message from server.\n");
		return -1;
	}

	while(1)      //接收服务器发送过来的数据
	{
		char buf[MAXSIZE];
		memset(buf,0,sizeof(buf));
		ssize_t s=recv(sock_data,buf,MAXSIZE,0);
		if(s<=0)
		{
			if(s<0)
				perror("error");
			break;
		}
		printf("%s",buf);
	}

	//等待服务器发送完成的信号
	if(recv(sock_ctl,&tmp,sizeof(tmp),0)<0)
	{
		perror("client:error reading message from server.");
		return -1;
	}
	return 0;
}

int ftpclient_send_cmd(int sock_ctl,struct command* cmd)     //发送命令
{
	char buf[MAXSIZE];
	sprintf(buf,"%s %s",cmd->code,cmd->arg);
	if(send(sock_ctl,buf,strlen(buf),0)<0)
	{
		perror("error sending command to server.\n");
		return -1;
	}
	return 0;
}


int ftpclient_login(int sock_ctl)
{
	struct command cmd;
	char user[256];
	memset(user,0,sizeof(user));

	printf("Name: ");
	fflush(stdout);
	read_input(user,256);   //获取用户名

	strcpy(cmd.code,"USER");
	strcpy(cmd.arg,user);
	ftpclient_send_cmd(sock_ctl,&cmd);     //发送用户名到服务器
	
	int wait=0;
	recv(sock_ctl,&wait,sizeof(wait),0);      //接受应答码 331

	fflush(stdout);
	char* pass=getpass("Password: ");      //使用getpass函数获取密码

	strcpy(cmd.code,"PASS");
	strcpy(cmd.arg,pass);
	ftpclient_send_cmd(sock_ctl,&cmd);   //发送密码到服务器

	int retcode=read_reply(sock_ctl);   //读取服务器的回应
	switch(retcode)
	{
	case 430:
		printf("Invalid username/password.\n");
		return -1;
	case 230:
		printf("Successful login.\n");      //登录成功
		break;
	default:
		printf("error reading message from server.\n");
		return -1;
	}
	return 0;
}
