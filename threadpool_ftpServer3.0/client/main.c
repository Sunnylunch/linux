#include"ftpClient.h"

void Usage(const char *filename)
{
	printf("Usage: %s servet_ip server_port\n",filename);
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		Usage(argv[0]);
		return -1;
	}
	
	
	int sock_ctl=socket_connect(argv[1],atoi(argv[2]));
	if(sock_ctl<0)
	{
		printf("connected failed,\n");
		return -1;
	}

	printf("Connected  to %s.\n",argv[1]);
	print_reply(read_reply(sock_ctl));
	if(ftpclient_login(sock_ctl)<0)
	{
		return -1;
	}

	while(1)
	{
		char buf[MAXSIZE];
		memset(buf,0,sizeof(buf));
		struct command cmd;
		memset(&cmd,0,sizeof(cmd));
		if(ftpclient_read_cmd(buf,sizeof(buf),&cmd)<0)
		{
			printf("Invalid command\n");
			continue;
		}
		
		if(send(sock_ctl,buf,strlen(buf),0)<0)  //发送命令到服务器
		{
			break;
		}
	
		int status=read_reply(sock_ctl);    //读取服务器的返回码

		if(status==221)     //退出命令
		{
			print_reply(221);      //提示用户退出
			break;
		}

		if(status==502)      //不合法的命令
		{
			printf("%d Invalid commond.\n",status);
		}
		else        //status is 200
		{
			int sock_data=ftpclient_open_conn(sock_ctl);   //创建一个数据连接
			if(sock_data<0)
			{
				perror("Error open data connect failed\n");
				break;
			}
			
			if(strcmp(cmd.code,"LIST")==0)      //list命令
			{
				ftpclient_list(sock_ctl,sock_data);
				close(sock_data);      
			}
			else if(strcmp(cmd.code,"RETR")==0)  //retr命令
			{
				if(read_reply(sock_ctl)==550)  //判断服务器端文件正常
				{
					print_reply(550);    //打印550回复
					close(sock_data);
					continue;
				}
				ftpclient_get(sock_data,cmd.arg);         //下载文件
				print_reply(read_reply(sock_ctl));    
				close(sock_data);      
			}
			else if(strcmp(cmd.code,"PUSH")==0)
			{
				if(ftpclient_put(sock_data,cmd.arg)<0)   //告诉服务器文件上传失败
					send_response(sock_ctl,553);
				else									  //告诉服务器文件已经上传
					send_response(sock_ctl,200);
				close(sock_data);      
				print_reply(read_reply(sock_ctl));    //打印服务器端的响应
			}
		}
	}
	close(sock_ctl);
	return 0;
}
