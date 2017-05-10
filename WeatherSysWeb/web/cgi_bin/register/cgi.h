#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _SIZE_ 1024


int cgi(char* content_data)
{
	char method[_SIZE_];
	char content_len[_SIZE_];
	if(getenv("METHOD"))         //取出METHOND环境变量
	{
		strcpy(method,getenv("METHOD"));
	}
	else
	{
		return 1;
	}

	if(strcasecmp(method,"GET")==0)      //如果是GET方法
	{
		if(getenv("QUERY_STRING"))
		{
			strcpy(content_data,getenv("QUERY_STRING"));
		}
		else
		{
			return 1;
		}
	}
	else if(strcasecmp(method,"POST")==0)     //如果是post方法，需要先获取conten_len，然后再从管道中读数据
	{
		if(getenv("CONTENT_LEN"))
		{
			strcpy(content_len,getenv("CONTENT_LEN"));
		}
		else
		{
			return 1;
		}
	
		int i=0;
		char ch='\0';
		for(i=0;i<atoi(content_len);i++)
		{
			read(0,&ch,1);
			content_data[i]=ch;
		}
		content_data[i]='\0';
	}

	return 0;
}









