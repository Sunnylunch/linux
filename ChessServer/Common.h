/*************************************************************************
	> File Name: Common.h
	> Author: lf
	> Mail: 2401503224@qq.com 
	> Created Time: Mon 14 May 2018 12:25:55 AM PDT
 ************************************************************************/

#ifndef _COMMON_H__
#define _COMMON_H__
#include<string>
#include<map>
using namespace std;

enum {
	LOGIN,
	LOGOUT,
	REGISTER,
	SEARCH,
	PVPMATCH,
	PVP,
	PVPLOGOUT,
	PVPCHEAT,
	PVPCHEATRECV,
	GAMERESULT	
};

map<string,int> MapTag;

void Init()
{
	MapTag["LOGIN"]=LOGIN;
	MapTag["LOGOUT"]=LOGOUT;
	MapTag["REGISTER"]=REGISTER;
	MapTag["SEARCH"]=SEARCH;
	MapTag["PVPMATCH"]=PVPMATCH;
	MapTag["PVP"]=PVP;
	MapTag["PVPLOGOUT"]=PVPLOGOUT;
	MapTag["PVPCHEAT"]=PVPCHEAT;
	MapTag["PVPCHEATRECV"]=PVPCHEATRECV;
	MapTag["GAMERESULT"]=GAMERESULT;
}

void ParseTag(int& tag,char* buf,string& data)
{
	string strTag;
	
	int i=0;
	for(i=0;buf[i]!='\0';i++)
	{
		if(buf[i]=='|')
			break;
		strTag+=buf[i];
	}

	data=&buf[i+1];
	tag=MapTag[strTag];
}

void ParseLogin(string &str,string& name,string& passwd)
{
	int flag=0;
	for(int i=0;i<str.size();i++)
	{
		if(str[i]=='&')
		{
			flag=1;
			continue;
		}
		else if(flag== 0)
		{
			name+=str[i];
		}
		else
		{
			passwd+=str[i];
		}
	}
}

void ParseLogout(string& str,string& name)
{
	name=str;
}


void ParseRegister(string& str,string& name,string& passwd)
{
	int flag=0;
	for(int i=0;i<str.size();i++)
	{
		if(str[i]=='&')
		{
			flag=1;
			continue;
		}

		if(flag== 0)
		{
			name+=str[i];
		}
		else
		{
			passwd+=str[i];
		}
	}
}

void ParseSearch(string &str,string& name)
{
	name=str;
}

void ParsePvpMatch(string &str,string& name)
{
	name=str;
}

void ParsePvp(string &str,string& name)
{
	name=str;
}

void ParsePvpLogout(string &str,string& name)
{
	name=str;
}

void ParsePvpCheatRecv(string &str,string& name)
{
	name=str;
}

void ParsePvpCheat(string &str,string& name)
{
	name=str;
}

void ParseGameResult(string &str,string& name,string& result)
{
	int flag=0;
	for(int i=0;i<str.size();i++)
	{
		if(str[i]=='&')
		{
			flag=1;
			continue;
		}

		if(flag== 0)
		{
			name+=str[i];
		}
		else
		{
			result+=str[i];
		}
	}
}
#endif //_COMMON_H__




