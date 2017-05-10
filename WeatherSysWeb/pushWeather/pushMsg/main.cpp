#include"UserMsg.h"
#include"weather.h"
#include<unistd.h>
#include<iostream>
using namespace std;

void GetUserMsg(vector<string> &v)                          //拿到用户的信息,保存在vector里面
{
	UserMsg sql;
	sql.sql_connect();
	sql.sql_select(v);
}

void GetWeather(std::string inString,std::string &outString)  //拿到用户所在城市的天气
{
	weather sql;
	sql.sql_connect();
	sql.sql_select(inString,outString);
}

int main(int argc,char* argv[])
{
	char *path=argv[1];
	vector<string> v;
	GetUserMsg(v);

	int i=0;
	for(i=0;i<(int)v.size();)
	{
		std::string name=v[i++];
		std::string city=v[i++];
		std::string mail=v[i++];
		std::string air;
		GetWeather(city,air);
		std::string temp="\"";
		temp+=air;
		temp+="\"";
		air=temp;
		pid_t id=vfork();
		if(id==0)                         //子进程去执行消息推送
		{
			const char* n=name.c_str();
			const char* a=air.c_str();
			const char* m=mail.c_str();
			execl(path,"./sendMsg.sh",n,a,m,NULL);
			exit(1);
		}
	}
	return 0;
}
