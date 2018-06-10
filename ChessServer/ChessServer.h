#ifndef _CHESS_SERVER_H__
#define _CHESS_SERVER_H__

#define MAXSIZE 100
#define WAITTIME 100


#include<iostream>
#include<string.h>
#include<stdio.h>
#include<strings.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>

#include<string>
#include<map>
#include<set>
#include<list>

#include"Common.h"


using namespace std;


class ChessServer
{
public:
	ChessServer()
	{}

	~ChessServer()
	{}

	bool IsLogin(string name)
	{
		set<string>::iterator iter=m_sLoginList.find(name);
		if(iter==m_sLoginList.end())
		{
			return false;
		}
		return true;
	}

	void Clear(string& name)
	{
		set<string>::iterator IterLogin=m_sLoginList.find(name); 
		set<string>::iterator IterNoPvp=m_sNoPvpList.find(name);
		if(IterLogin!=m_sLoginList.end())
		{
			m_sLoginList.erase(IterLogin);
		}

		if(IterNoPvp!=m_sNoPvpList.end())
		{
			m_sNoPvpList.erase(IterNoPvp);
		}
	}
	
	void ClearPvpInfo(string& name)
	{	
		map<string,string>::iterator IterMatchList=m_mMatchList.find(name);
		map<string,int>::iterator IterPvp=m_mPvp.find(name);
		set<string>::iterator IterPvpList=m_sPvpList.find(name);
		map<string,int>::iterator IterPvpCheat=m_mPvpCheat.find(name);

		if(IterMatchList!=m_mMatchList.end())
		{
			m_mMatchList.erase(IterMatchList);
		}

		if(IterPvp!=m_mPvp.end())
		{
			close(IterPvp->second);
			m_mPvp.erase(IterPvp);
		}
		
		if(IterPvpCheat!=m_mPvpCheat.end())
		{
			close(IterPvpCheat->second);
			m_mPvpCheat.erase(IterPvpCheat);
		}
		
		if(IterPvpList!=m_sPvpList.end())
		{
			m_sPvpList.erase(IterPvpList);
		}

		m_sNoPvpList.insert(name);
	}

	void AddLoginInfo(string name)
	{
		m_sLoginList.insert(name); 
		m_sNoPvpList.insert(name);
	}

	void AddPvpList(string name)
	{
		set<string>::iterator IterNoPvp=m_sNoPvpList.find(name);
		if(IterNoPvp!=m_sNoPvpList.end())
		{
			m_sNoPvpList.erase(IterNoPvp);
			m_sPvpList.insert(name);
		}
	}

	void MatchPvp()
	{
		while(m_sPvpList.size()>1)
		{
			string p1;
			string p2;
			set<string>::iterator Iter=m_sPvpList.begin();
			if(Iter!=m_sPvpList.end())
			{
				p1=*Iter;
				m_sPvpList.erase(Iter);
			}
			
			Iter=m_sPvpList.begin();
			if(Iter!=m_sPvpList.end())
			{
				p2=*Iter;
				m_sPvpList.erase(Iter);
			}

			m_mMatchList[p1]=p2;
			m_mMatchList[p2]=p1;
		}
	}


	int GetPvpListCount()
	{
		return m_sPvpList.size();
	}

	bool GetMatchRet(string name)
	{
		map<string,string>::iterator Iter=m_mMatchList.find(name);
		if(Iter==m_mMatchList.end())
			return false;
		else
			return true;
	}

	int NameToSock(string name)
	{
		map<string,string>::iterator IterMatch=m_mMatchList.find(name);
		if(IterMatch==m_mMatchList.end())
			return -1;

		map<string,int>::iterator iter=m_mPvp.find(IterMatch->second);
		if(iter==m_mPvp.end())
		{
			return -1;
		}
		return iter->second;
	}

	int NameToSockCheat(string name)
	{
		map<string,string>::iterator IterMatch=m_mMatchList.find(name);
		if(IterMatch==m_mMatchList.end())
			return -1;

		map<string,int>::iterator iter=m_mPvpCheat.find(IterMatch->second);
		if(iter==m_mPvpCheat.end())
		{
			return -1;
		}
		return iter->second;
	}

	void AddPvpInfo(string& name,int sock)
	{
		m_mPvp[name]=sock;
	}
	
	void AddPvpCheatInfo(string& name,int sock)
	{
		m_mPvpCheat[name]=sock;
	}

	bool IsLinkExist(string name)
	{
		map<string,string>::iterator Iter1=m_mMatchList.find(name);
		if(Iter1==m_mMatchList.end())
			return false;

		map<string,string>::iterator Iter2=m_mMatchList.find(Iter1->second);
		if(Iter2==m_mMatchList.end())
			return false;
		
		if(Iter1->first!=Iter2->second||Iter1->second!=Iter2->first)
			return false;

		map<string,int>::iterator iter1=m_mPvp.find(Iter1->first);
		if(iter1==m_mPvp.end())
		{
			return false;
		}
		
		map<string,int>::iterator iter2=m_mPvp.find(Iter2->first);
		if(iter2==m_mPvp.end())
		{
			return false;
		}
		
		return true;
	}
	
	string GetSideColor(string name)
	{
		map<string,string>::iterator Iter=m_mMatchList.find(name);	
		if(name > Iter->second)
			return "1";
		else
			return "2";
	}

private:
	set<string> m_sLoginList; 
	set<string> m_sNoPvpList;
	set<string> m_sPvpList;
	map<string,string> m_mMatchList;
	map<string,int> m_mPvp;
	map<string,int> m_mPvpCheat;
};



void StringToNum_record(string record,int &Sum,int &Win,int &Fail,int& Draw)
{
	string sum;
	string win;
	string fail;
	string draw;

	int flag=0;
	for(int i=0;i<record.size();i++)
	{
		if(record[i]=='|')
		{
			flag++;
			continue;
		}

		if(flag==0)
		{
			sum+=record[i];
		}
		else if(flag==1)
		{
			win+=record[i];
		}
		else if(flag==2)
		{
			fail+=record[i];
		}
		else if(flag==3)
		{
			draw+=record[i];
		}
	}
	Sum=atoi(sum.c_str());
	Win=atoi(win.c_str());
	Fail=atoi(fail.c_str());
	Draw=atoi(draw.c_str());
}

void NumTonString_record(int Sum,int Win,int Fail,int Draw,string &record)
{
	char sum[10];
	char win[10];
	char fail[10];
	char draw[10];
	sprintf(sum,"%d",Sum);
	sprintf(win,"%d",Win);
	sprintf(fail,"%d",Fail);
	sprintf(draw,"%d",Draw);

	record+=sum;
	record+='|';
	record+=win;
	record+='|';
	record+=fail;
	record+='|';
	record+=draw;;
}



#endif //_CHESS_SERVER_H__

