#include"sql_api.h"

sql_api::sql_api()
{
	mysql=mysql_init(NULL);
}

sql_api::~sql_api()
{
	mysql_close(mysql);
}

int sql_api::sql_connect()
{
	if(!mysql_real_connect(mysql,"localhost","root","xxxx","Weather",3306,NULL,0))
	{
		return -1;     //失败返回非0
	}
	return 0;     //成功返回0
}

int sql_api::sql_insert(std::string str)
{
	std::string data="insert into login (name,mail,tel,passwd) values ( "; 
	data+=str;
	data+=" )";
	return mysql_real_query(mysql,data.c_str(),data.size());      //成功返回0，
}
