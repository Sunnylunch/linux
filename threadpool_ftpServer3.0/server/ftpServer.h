#ifndef _FTP_SERVER_H__
#define _FTP_SERVER_H__

#include"common.h"
#include<string>
#include<sys/stat.h>
#include<sys/sendfile.h>
using namespace std;

class ftpServer
{
public:
	ftpServer(int sock);
	~ftpServer();
	void process();
private:
	void ftpserver_process();	
	int ftpserver_recv_cmd(char* cmd,char* arg);
	int ftpserver_login();
	int ftpserver_check_user(const char* user,const char* pass);
	int ftpserver_start_data_conn();
	int ftpserver_list(int sock_data);
	void ftpserver_retr(int sock_data,char *filename);
	void ftpserver_push(int sock_data,char* filename);
private:
	int sock_ctl;
};

#endif  //_FTP_SERVER_H__
