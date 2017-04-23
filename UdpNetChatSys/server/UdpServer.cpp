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
	if(bind(_sock,(struct sockaddr *)&local,sizeof(local))<0)
	{
		print_log("socket error",FATAL);
		exit(2);
	}
}


void udp_server::addrUser(const struct sockaddr_in &remote)
{
	userList.insert(pair<in_addr_t,struct sockaddr_in>(remote.sin_addr.s_addr,remote));
}


int udp_server::recvData(std::string &outString)
{
	//可靠性
	struct sockaddr_in remote;
	socklen_t len=sizeof(remote);
	char buf[SIZE];
	ssize_t s=recvfrom(_sock,buf,sizeof(buf),0,(struct sockaddr*)&remote,&len);
	if(s<0)
	{
		print_log("recvfrom error",WARNING);
		return s;
	}
	addrUser(remote);
	pool.putData(outString);
	return s;
}


int udp_server::sendData(std::string &inString,struct sockaddr_in &remote)
{
	ssize_t s=sendto(_sock,inString.c_str(),inString.size(),0,(struct sockaddr*)&remote,sizeof(remote));
	if(s<0)
	{
		print_log("sendto error",WARING);
		return s;
	}
	return s;
}


int udp_server::broadcast()
{
	std::string inString;
	pool.getData(inString);
	map<in_addr_in,struct sockaddr_in>::iterator iter=userList.begin();
	for(;iter!=userList.end();iter++)
	{
		sendData(inString,iter->second);
	}
}

udp_server::~udp_server()
{
	close(_sock);
}
