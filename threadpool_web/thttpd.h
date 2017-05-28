#ifndef __THTTPD_H__
#define __THTTPD_H__

#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<syslog.h>
#include<ctype.h>

#define NORMAL  0
#define WRONING 1
#define FATAL   2
#define SIZE 4096

int startup(const char* _ip,int _port);

void printf_log(const char* msg,int level);

class http
{
public:
	void process();
	void init(int sockfd);
private:
	int handler_msg();
	
	int get_line(char* buf);
	void clear_header();  
	
	void echo_error(int err_code);
	void show_404();  
	
	int echo_www(const char * path,size_t s);  
	int excu_cgi(const char* method,const char* path,const char* query_string); 
	
private:
	int sock;
};

#endif
