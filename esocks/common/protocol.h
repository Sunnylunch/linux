#ifndef _PROTOCOL_H__
#define _PROTOCOL_H__

#pragma pack(push)     //保存对齐状态
#pragma pack(1)        //以1字节对齐

typedef struct SKReq		   //接收socks5建立连接的时候客户端发送给服务器的请求
{
	char ver;          //socks的版本
	char nmethods;     //表示客户端可接受的认证方法数量
}SKReq;

typedef struct SKRep           //服务器收到客户端的数据之后做出的响应
{
	char ver;
	char method;       //验证方法编号
}SKRep;

//VER | CMD | RSV | ATYPE | DST.ADDR | DST.PORT

typedef struct AddrReq         //接收客户端的请求数据包
{
	char ver;
	char cmd;
	char rsv;
	char atype;
}AddrReq;

typedef struct AddrRep        //服务器对客户端的请求数据包做出的响应包
{
	char ver;
	char rep;         //客户端请求的命令
	char rsv;
	char atype;       //使用的地址格式
	int addr;
	short port;
}AddrRep;

#pragma pack(pop)     //恢复对齐状态
#endif  //_PROTOCOL_H__
