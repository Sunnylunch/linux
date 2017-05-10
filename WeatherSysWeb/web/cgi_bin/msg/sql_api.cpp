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
	std::string data="insert into msg (tel,name,sex,city,value) values ( "; 
	data+=str;
	data+=" )";
	return mysql_real_query(mysql,data.c_str(),data.size());      //成功返回0，
}

int sql_api::sql_delete(std::string str)
{
	std::string data="delete from msg  where ";
	data+=str;
	return mysql_real_query(mysql,data.c_str(),data.size());  //成功返回0
}

int sql_api::sql_select(std::string str)
{
	std::string data="select * from msg where tel=";
	data+=str;
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

	printf("<table border=\"1\" cellspacing=\"0\" >");
	printf("<tr>");	
	MYSQL_FIELD* field;       //获取列名
	int flag=0;
	while(field=mysql_fetch_field(res))
	{
		if(flag!=0)
		{
			printf("<th>");
			cout<<field->name;
			printf("</th>");
		}
		flag++;
	}
	printf("</tr>");

	MYSQL_ROW fetch;
	while(fetch=mysql_fetch_row(res))
	{
		printf("<tr>");
		int i=0;
		for(i=0;i<num_field;i++)
		{
			if(i!=0)
			{
				printf("<td>");	
				cout<<fetch[i];
				printf("</td>");	
			}
		}
		printf("</tr>");
	}
	printf("</table>");
	return 0;
}

int sql_api::sql_update(std::string str1,std::string str2)
{
	std::string data="update msg set ";
	data+=str1;
	data+=" where ";
	data+=str2;
	return mysql_real_query(mysql,data.c_str(),data.size());    //
}
