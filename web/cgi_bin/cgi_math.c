#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _SIZE_ 1024
int main()
{
	char method[_SIZE_];
	char content_len[_SIZE_];
	char content_data[_SIZE_];

	if(getenv("METHOD"))         //取出METHOND环境变量
	{
		strcpy(method,getenv("METHOD"));
	}
	else
	{
	//	printf("cgi: method not exist");
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
		//	printf("cgi: query_string not exist");
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
		//	printf("cgi: content_len not exist");
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

	int data1=0;
	int data2=0;
	char *arr[3];
	int i=0;
	char* start=content_data;
	while(*start)
	{
		if(*start=='=')
		{
			arr[i++]=start+1;
		}
		else if(*start=='&')
		{
			*start='\0';
		}
		start++;
	}
	data1=atoi(arr[0]);
	data2=atoi(arr[1]);

	//将运算结果返回
	printf("<html>");
	printf("<h2>");
	printf("<tr>%d+%d=%d</tr><br/>",data1,data2,data1+data2);
	printf("<tr>%d-%d=%d</tr><br/>",data1,data2,data1-data2);
	printf("<tr>%d*%d=%d</tr><br/>",data1,data2,data1*data2);
	printf("<tr>%d/%d=%d</tr><br/>",data1,data2,data1/data2);
	printf("<tr>%d%%%d=%d</tr><br/>",data1,data2,data1%data2);
	printf("</h2>");
	printf("</html>");

	return 0;
}









