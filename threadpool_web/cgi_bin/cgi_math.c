#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"cgi.h"

int main()
{  
	char content_data[_SIZE_];
	cgi(content_data);
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









