#include"ChessServer.h"
#include"sql_api.h"
using namespace std;

ChessServer CheSvr;
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;


int Startup(int port,const char* ip)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		printf("socket failed\n");
		exit(2);
	}

	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		printf("bind failed\n");
		exit(3);
	}

	if(listen(sock,1024)<0)
	{
		printf("listen failed\n");
		exit(4);
	}
	return sock;
}

bool IsRecord(string name,string& passwd,string& record)      //是否有用户记录
{
	sql_api sql;
	if(sql.sql_connect()!=0)  //failed
	{
		return false;
	}

	int ret=0;
	name="\""+name+"\"";
	ret=sql.sql_select(name,passwd,record);

	if(ret==0)
	{
		return true;
	}
	return false;
}


///////////////////////////////////////////////////

void UserRegister(int sock,string &data)  //注册处理函数
{
	string UserName;
	string Passwrod;
	ParseRegister(data,UserName,Passwrod);

	string retMsg="1";     //failed
	string record;
	string passwd;

	if(!IsRecord(UserName,passwd,record))
	{
		sql_api sql;
		if(sql.sql_connect()==0)  //succ
		{
			string str="\""+UserName+"\",\""+Passwrod+"\",\"";
			str+="0|0|0|0\"";

			int ret=0;
			ret=sql.sql_insert(str);		
			if(ret==0)
			{
				retMsg="0";
			}
			else
			{
				retMsg="1";     //失败
			}
		}
	}
	
	send(sock,retMsg.c_str(),retMsg.size(),0);
	close(sock);
}


void UserLogin(int sock,string &data)    //用户登录处理
{
	string UserName;
	string Passwrod;
	ParseLogin(data,UserName,Passwrod);
	
	string passwd;
	string record;
	string retMsg;

	if(!IsRecord(UserName,passwd,record))    //没有注册信息
	{
		retMsg="2";
		send(sock,retMsg.c_str(),retMsg.size(),0);
		close(sock);
		return;
	}
	else                                     //有注册信息
	{
		if(passwd!=Passwrod)                 //密码不对
		{
			retMsg="1";
			send(sock,retMsg.c_str(),retMsg.size(),0);
			close(sock);
			return;
		}
	}
	

	bool IsLogin;
	pthread_mutex_lock(&lock);
	IsLogin=CheSvr.IsLogin(UserName);
	pthread_mutex_unlock(&lock);
	
	if(IsLogin)           //已经登录
	{
		retMsg="3";
		send(sock,retMsg.c_str(),retMsg.size(),0);	
		close(sock);
		return;
	}
	else                   //还没有登陆
	{
		pthread_mutex_lock(&lock);
		CheSvr.AddLoginInfo(UserName);	
		pthread_mutex_unlock(&lock);
		
		retMsg="0";       //登录成功 
		send(sock,retMsg.c_str(),retMsg.size(),0);
		close(sock);
		return;
	}
}


void UserLogout(int sock,string& data)  //用户注销
{
	string UserName;
	ParseLogout(data,UserName);

	pthread_mutex_lock(&lock);
	CheSvr.Clear(UserName);	
	pthread_mutex_unlock(&lock);

	close(sock);
}


void UserSearch(int sock,string& data)  //用户search
{
	string UserName;
	ParseSearch(data,UserName);
	
	string passwd;
	string record;
	string retMsg;

	if(!IsRecord(UserName,passwd,record))    //查找失败
	{
		retMsg="1";                          //查找失败
		send(sock,retMsg.c_str(),retMsg.size(),0);
	}
	else                                    //查找成功
	{                    
		retMsg=record;
		printf("record:%s\n",retMsg.c_str());
		send(sock,retMsg.c_str(),retMsg.size(),0);	
	}
	close(sock);
}

void UserPvpMatch(int sock,string& data)
{
	string UserName;
	ParsePvpMatch(data,UserName);
	
	pthread_mutex_lock(&lock);
	CheSvr.AddPvpList(UserName);
	pthread_mutex_unlock(&lock);
	
	int count=0;
	while(count< WAITTIME)
	{
	
		if(CheSvr.GetMatchRet(UserName))
		{
			bool ret=false;
			pthread_mutex_lock(&lock);
			ret=CheSvr.GetMatchRet(UserName);
			pthread_mutex_unlock(&lock);
			if(ret)
				break;
		}
		sleep(1);
		count++;
	}
	
	string retMsg="0";

	if(count>=WAITTIME)
	{
		retMsg="1";
		pthread_mutex_lock(&lock);
		if(CheSvr.GetMatchRet(UserName))
			retMsg="0";
		else
			CheSvr.ClearPvpInfo(UserName);
		pthread_mutex_unlock(&lock);
	}
	send(sock,retMsg.c_str(),retMsg.size(),0);	
	close(sock);
}


void UserPvpLogout(int sock,string& data)
{
	string UserName;
	ParsePvpLogout(data,UserName);

	pthread_mutex_lock(&lock);
	CheSvr.ClearPvpInfo(UserName);	
	pthread_mutex_unlock(&lock);
}
		

void UserGameResult(int sock,string& data)
{
	string UserName;
	string result;
	ParseGameResult(data,UserName,result);

	string passwd;
	string record;
	if(IsRecord(UserName,passwd,record))    //succss
	{
		int Sum=0,sum=0;
		int Win=0,win=0;
		int Fail=0,fail=0;
		int Draw=0,draw=0;
		StringToNum_record(record,Sum,Win,Fail,Draw);
		StringToNum_record(result,sum,win,fail,draw);

		Sum+=sum;
		Win+=win;
		Fail+=fail;
		Draw+=draw;
		record.clear();
		NumTonString_record(Sum,Win,Fail,Draw,record);

		sql_api sql;
		if(sql.sql_connect()==0)  //succ
		{
			UserName="\""+UserName+"\"";
			record="\""+record+"\"";
			sql.sql_update(UserName,record);		
		}
	}

	close(sock);
}

void UserPvpCheat(int sock,string& data)
{
	string UserName;
	ParsePvpCheat(data,UserName);
	
	int count=0;
	int fd=-1;
	while(count<60)
	{
		fd=CheSvr.NameToSockCheat(UserName);
		if(fd!=-1)
			break;

		count++;
		sleep(1);
	}

	while(1)
	{

		if(!CheSvr.IsLinkExist(UserName))
		{
			bool ret;
			pthread_mutex_lock(&lock);
			ret=CheSvr.IsLinkExist(UserName);
			pthread_mutex_unlock(&lock);
			if(!ret)
				break;
		}

		char buf[MAXSIZE];
		memset(buf,0,MAXSIZE);
		ssize_t ret=recv(sock,buf,MAXSIZE-1,0);
		string str=buf;
		send(fd,str.c_str(),str.size(),0);	
	}
	close(sock);
}
		
void UserPvpCheatRecv(int sock,string& data)
{
	string UserName;
	ParsePvpCheatRecv(data,UserName);

	pthread_mutex_lock(&lock);
	CheSvr.AddPvpCheatInfo(UserName,sock);	
	pthread_mutex_unlock(&lock);

	while(1)
	{
		if(!CheSvr.IsLinkExist(UserName))
		{
			bool ret;
			pthread_mutex_lock(&lock);
			ret=CheSvr.IsLinkExist(UserName);
			pthread_mutex_unlock(&lock);
			if(!ret)
				break;
		}
	}
}

void UserPvp(int sock,string& data)
{
	string UserName;
	ParsePvp(data,UserName);

	pthread_mutex_lock(&lock);
	CheSvr.AddPvpInfo(UserName,sock);	
	pthread_mutex_unlock(&lock);

	string side=CheSvr.GetSideColor(UserName);
	send(sock,side.c_str(),side.size(),0);	
	printf("UserName:cloro %s\n",side.c_str());


	int fd=-1;
	int count=0;
	while(count<60)
	{
		fd=CheSvr.NameToSock(UserName);
		if(fd!=-1)
			break;
		
		count++;
		sleep(1);
	}
	

	while(1)
	{
		if(!CheSvr.IsLinkExist(UserName))
		{
			bool ret;
			pthread_mutex_lock(&lock);
			ret=CheSvr.IsLinkExist(UserName);
			pthread_mutex_unlock(&lock);
			if(!ret)
				break;
		}

		char buf[MAXSIZE];
		memset(buf,0,MAXSIZE);
		ssize_t ret=recv(sock,buf,MAXSIZE-1,0);
		string str=buf;
		send(fd,str.c_str(),str.size(),0);	
	}
}


int handle_msg(int sock)
{
	char buf[MAXSIZE];
	memset(buf,0,MAXSIZE);

	ssize_t ret=recv(sock,buf,MAXSIZE-1,0);

	printf("buf:%s\n",buf);

	int tag;
	string data;
	ParseTag(tag,buf,data);
	
	switch(tag)
	{
	case LOGIN:
		UserLogin(sock,data);
		break;
	case LOGOUT:
		UserLogout(sock,data);
		break;
	case REGISTER:
		UserRegister(sock,data);
		break;
	case SEARCH:
		UserSearch(sock,data);
		break;
	case PVPMATCH:
		UserPvpMatch(sock,data);
		break;
	case PVP:
		UserPvp(sock,data);
		break;
	case PVPLOGOUT:
		UserPvpLogout(sock,data);
		break;
	case PVPCHEAT:
		UserPvpCheat(sock,data);
		break;
	case PVPCHEATRECV:
		UserPvpCheatRecv(sock,data);
		break;
	case GAMERESULT:
		UserGameResult(sock,data);
		break;
	default:
		close(sock);
	}
	return 0;
}



void* ClientMsgHandle(void* arg)       //客户端消息处理线程
{
	int sock=(int) arg;
	pthread_detach(pthread_self());
	return (void *)handle_msg(sock);
}



void *HandleMatchPvP(void* arg)   //匹配处理线程
{
	pthread_detach(pthread_self());
	while(1)
	{
		if(CheSvr.GetPvpListCount()>1)
		{
			pthread_mutex_lock(&lock);
			CheSvr.MatchPvp();
			pthread_mutex_unlock(&lock);
		}
	}
	return NULL;
}



int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("Usage: ./%s [local_ip] [local_port]\n",argv[0]);
		return 1;
	}

	int sockfd=Startup(atoi(argv[2]),argv[1]);

	Init();

	pthread_t MatchTid;
	if(pthread_create(&MatchTid,NULL,HandleMatchPvP,NULL)>0)
	{
		printf("pthread_create Match failed\n");
	}

	while(1)
	{
		struct sockaddr_in peer;
		socklen_t len=sizeof(peer);
		int sock=accept(sockfd,(struct sockaddr*)&peer,&len);
		if(sock<0)
		{
			printf("accept failed\n");
			continue;
		}
		printf("accept\n");
		
		pthread_t tid;
		if(pthread_create(&tid,NULL,ClientMsgHandle,(void*)sock)>0)
		{
			printf("pthread_create failed\n");
			close(sock);
		}
	}
	return 0;
}
