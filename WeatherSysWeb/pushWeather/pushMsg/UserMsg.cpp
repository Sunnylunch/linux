#include"UserMsg.h"

UserMsg::UserMsg()
{
	mysql=mysql_init(NULL);
}

UserMsg::~UserMsg()
{
	mysql_close(mysql);
}

int UserMsg::sql_connect()
{
	if(!mysql_real_connect(mysql,"localhost","root","xxxx","Weather",3306,NULL,0))
	{
		return -1;     //失败返回非0
	}
	return 0;     //成功返回0
}


int UserMsg::sql_select(std::vector<string> &v)
{
	std::string data="select name,city,value from msg ";
	if(mysql_real_query(mysql,data.c_str(),data.size()))     //成功返回0
	{
		return -1;
	}
	//获取结果集
	MYSQL_RES *res=mysql_store_result(mysql);
	if(res==NULL)
	{
		return -1;     //失败返回非0
	}
	
	size_t num_field=mysql_field_count(mysql);  //获取结果集中的列数

	MYSQL_ROW fetch;
	int j=0; 
	while(fetch=mysql_fetch_row(res))
	{
		int i=0;
		for(i=0;i<num_field;i++)
		{
			v.push_back(fetch[i]);
		}
		j++;
	}
	return 0;
}


