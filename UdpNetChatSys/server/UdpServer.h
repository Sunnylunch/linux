#ifndef _UDP_SERVER_
#define _UDP_SERVER_

#include<iostream>
#include<string>
#include<map>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define SIZE 1024


class udp_server
{
public:
	udp_server(const std::string ip,const int port);
	initServer();
	int recvData(std::string &outString);
	int sendData(std::string &inString,struct sockaddr_in &remote);
	int broadcast();
	~udp_server();
private:
	void addrUser(struct sockaddr_in &remote);
private:
	int _sock;
	std::string _ip;
	int _port;
};





#endif  
