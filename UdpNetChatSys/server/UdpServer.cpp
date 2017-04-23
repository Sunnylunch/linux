#include"UdpServer.h"

udp_server::udp_server(const std::string ip,const int port)
	:_ip(ip)
	,_port(port)
{}

void udp_server::initServer()
{
	_sock=socket(AF_INET,SOCK_DGRAM,0);
	if(_sock<0)
	{
		print_log("socket error",FATAL);
		exit(1);
	}
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(_port);
	local.sin_addr.s_addr=inet_addr(_ip.c_str());
	if(bind(_sock,)<0)
	{
		print_log("socket error",FATAL);
		exit(2);
	}
}



void udp_server::addrUser(struct sockaddr_in &remote)
{

}

int udp_server::recvData(std::string &outString)
{

}


int udp_server::sendData(std::string &inString)
{

}

int udp_server::broadcast()
{


}

udp_server::~udp_server()
{
	close(_sock);
}
