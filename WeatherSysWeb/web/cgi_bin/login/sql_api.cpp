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


int sql_api::sql_select(std::string tel,std::string &passwd)
{
	std::string data="select * from login where tel=";
	data+=tel;
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
	
	size_t num=mysql_num_fields(res);     //获取结果集的行数
	size_t num_field=mysql_field_count(mysql);  //获取结果集中的列数

	MYSQL_ROW fetch;
	if(num==4)
	{
		fetch=mysql_fetch_row(res);
		int i=0;
		char c='\0';
		for(i=0;(c=fetch[3][i])!='\0';i++)
		{
			if(c!=' '&&c!='\0')
				passwd+=c;
		}
		return 0;
	}
	return -1;
}
