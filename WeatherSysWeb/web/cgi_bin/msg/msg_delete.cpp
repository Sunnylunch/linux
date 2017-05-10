#include"sql_api.h"
#include"cgi.h"

int main()
{
	char content_data[_SIZE_]={0};
	if(cgi(content_data)!=0)
	{
		return -1;
	}

	char* start=content_data;
	std::string str;
	while(*start)
	{
		if(*start=='=')
		{
			str+="='";
		}
		else if(*start=='&')
		{
			str+="'&&";
		}
		else
		{
			str+=*start;
		}
		start++;
	}
	str+="'";

	int ret=0;
	sql_api sql;
	sql.sql_connect();
	ret=sql.sql_delete(str);
	if(ret==0)
	{
		printf("<html>");
		printf("<h1>");
		printf("YES");
		printf("</h1>");
		printf("</html>");
	}
	else
	{
		printf("<html>");
		printf("<h1>");
		printf("error");
		printf("</h1>");
		printf("</html>");
	}

	return 0;
}
