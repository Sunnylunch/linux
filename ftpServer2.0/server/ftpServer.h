#ifndef _FTP_SERVER_H__
#define _FTP_SERVER_H__

#include"../common/common.h"
#include<sys/stat.h>


void ftpserver_process(int sock_ctl);

int ftpserver_recv_cmd(int sock_ctl,char* cmd,char* arg);

int ftpserver_login(int sock_ctl);

int ftpserver_check_user(const char* user,const char* pass);

int ftpserver_start_data_conn(int sock_ctl);

int ftpserver_list(int sock_data,int sock_ctl);

void ftpserver_retr(int sock_data,int sock_ctl,char *filename);

void ftpserver_push(int sock_data,int sock_ctl,char* filename);

#endif  //_FTP_SERVER_H__
