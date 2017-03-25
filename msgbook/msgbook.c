#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>

#define SIZE 1024
MYSQL mysql;   //mysql的句柄
void Connect(const char* host,const char* user,const char* passwd,const char* db)  //进行数据库的连接
{
	mysql_init(&mysql);  //初始化mysql句柄

	if(!mysql_real_connect(&mysql,host,user,passwd,db,3306,NULL,0))
	{
		printf("mysql_real_connect faile\n");
		mysql_close(&mysql);
		exit(1);
	}
	printf("mysql connect success\n");
}

//name tel  mail addr    通讯录的结构

void Insert(const char* msg)    //向数据库里面进行插入一条信息
{
	//插入的SQL语句:insert into 表名 (列名1，列名2，列名3，列名4) values (name,tel,mail,addr)
	//或使用load data local infile '文本名' into table 表名 批量插入到数据库中
	if(msg)
	{
		char buf[SIZE]="insert into book (name,tel,mail,addr) values ";
		strcat(buf,msg);
		int len=strlen(buf);
		if(mysql_real_query(&mysql,buf,len))    //mysql_real_query失败返回非0
		{
			printf("mysql_real_query insert faile\n");
			return;
		}
	}
}

void Delete(const char* condition)    //删除数据库里面的某一条信息
{
	//删除某一行的SQL语句：DELETE FROM 表名 WHERE 某一行的一个条件          
	if(condition)
	{
		char buf[SIZE]="delete from book where ";
		strcat(buf,condition);
		int len=strlen(buf);
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query insert faile\n");
			return;	
		}
	}
}

void Select(const char* condition)   //查询信息
{
	//SQL语句的查询: select * from 表名 where 某一行的条件
	char buf[SIZE]={0};
	if(condition)
	{
		strcpy(buf,"select * from book where ");
		strcat(buf,condition);
	}
	else      //如果条件为假，打印全部内容
	{
		strcpy(buf,"select * from book");
	}
	int len=strlen(buf);
	if(mysql_real_query(&mysql,buf,len))
	{
		printf("mysql_real_query select faile\n");   //查询失败
		return ;
	}
	
	MYSQL_RES* res;   //用来指向结果集
	MYSQL_ROW row;    //用来获取某一行的值
	if(!(res=mysql_store_result(&mysql)))      //将结果集存储起来
	{
		printf("mysql_store_result faile\n");
		mysql_free_result(res);
		return;
	}
	unsigned int num=mysql_num_fields(res);    //获取结果表中的列数
	printf("%-5s\t %-10s\t %-10s\t %-20s\n","name","tel","mail","addr");
	while((row=mysql_fetch_row(res)))  //获取结果集中的每一行，失败或读完返回NULL
	{
		unsigned int i=0;
		for(i=0;i<num;i++)          //row相当于一个指针数组，每一个元素都是一个字段的值
		{
			printf("%s\t",row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);
}

void Update(const char* column,const char* condition )    //更新某一列的值
{
	//更新某一列的SQL语句：update 表名 set 列名=新的值, ... where 确定到某一行的条件 
	if(condition)
	{
		char buf[SIZE]="update book set ";
		strcat(buf,column);
		strcat(buf," where ");
		strcat(buf,condition);

		int len=strlen(buf);
		if(mysql_real_query(&mysql,buf,len))
		{
			printf("mysql_real_query update faile\n");
			return;
		}
	}
}

void menu()
{
	int i=0;
	while(1)
	{
		printf("0:exit\t 1:insert\t 2:delete\t 3:select\t 4:update\t 5:printBook\n");
		printf("choice->:");
		scanf("%d",&i);
		char buf[100]={0};
		char condition[100]={0};
		switch(i)
		{
		case 0:
			exit(1);
			break;
		case 1:
			printf("输入 (naem,tel,mail,addr) 的格式插入:\n ");
			scanf("%s",buf);
			Insert(buf);
			break;
		case 2:
			printf("输入 name=名字,tel=电话,... 的格式进行删除，至少有一个条件:\n");
			scanf("%s",buf);
			Delete(buf);
			break;
		case 3:
			printf("输入 name=名字,tel=电话,... 的格式确定要查找的行，至少有一个条件:\n");
			scanf("%s",condition);
			Select(condition);
			break;
		case 4:
			printf("输入 name=名字,tel=电话,... 的格式确定要更新的行,至少一个条件\n");
			scanf("%s",condition);
			printf("输入 name=新值,tel=新值,mail=新值,addr=新值 的格式确定要更新的列,至少更改一列:\n");
			scanf("%s",buf);
			Update(buf,condition);
			break;
		case 5:
			Select(NULL);     //打印所有信息
			break;
		default:
			break;
		}
	}
}

int main()
{
	Connect("localhost","root","1996","MsgBook");
	menu();
	mysql_close(&mysql);
	return 0;
}
