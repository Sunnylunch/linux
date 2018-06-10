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
	if(!mysql_real_connect(mysql,"127.0.0.1","root","1996","Chess",3306,NULL,0))
	{
		return -1;     //失败返回非0
	}
	return 0;     //成功返回0
}

int sql_api::sql_select(std::string name,std::string &passwd,std::string &record)
{
	std::string data="select * from PersonalMsg where name=";
	data+=name;	
	
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
	if(num==3)
	{
		fetch=mysql_fetch_row(res);
		
		if(fetch==NULL)
			return -1;
		
		char c='\0';
		for(int i=0;(c=fetch[1][i])!='\0';i++)
		{
			if(c!=' '&&c!='\0')
				passwd+=c;
		}

		for(int i=0;(c=fetch[2][i])!='\0';i++)
		{
			if(c!=' '&&c!='\0')
				record+=c;
		}
		return 0;
	}
	return -1;
}



int sql_api::sql_insert(std::string str)
{
	std::string data="insert into PersonalMsg (name,passwd,record) values ( "; 
	data+=str;
	data+=" )";
	printf("sql:%s\n",data.c_str());
	
	return mysql_real_query(mysql,data.c_str(),data.size());      //成功返回0，
}


int sql_api::sql_update(std::string name,std::string record)
{
	std::string data="update PersonalMsg set record=";
	data+=record;
	data+=" where name=";
	data+=name;
	return mysql_real_query(mysql,data.c_str(),data.size());      //成功返回0，
}

