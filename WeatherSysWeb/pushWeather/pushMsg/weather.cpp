#include"weather.h"

weather::weather()
{
	mysql=mysql_init(NULL);
}

weather::~weather()
{
	mysql_close(mysql);
}

int weather::sql_connect()
{
	if(!mysql_real_connect(mysql,"localhost","root","xxxx","Weather",3306,NULL,0))
	{
		return -1;     //失败返回非0
	}
	return 0;     //成功返回0
}


int weather::sql_select(std::string v,std::string &outString)
{
	std::string data="select * from weather where city=";
	data+="'";
	data+=v;
	data+="天气预报";
	data+="'";

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
	if(fetch=mysql_fetch_row(res))
	{
		int i=0;
		for(i=0;i<num_field;i++)
		{
			outString+=fetch[i];
		}
	}
	return 0;
}


