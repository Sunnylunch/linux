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
	char* cont[8];
	int i=0;
	while(*start)
	{
		if(*start=='=')
		{
			cont[i++]=start+1;
		}
		else if(*start=='&')
		{
			*start='\0';
		}
		start++;
	}

	std::string field="tel='";
	field+=cont[0];
	field+="',name='";
	field+=cont[1];
	field+="',sex='";
	field+=cont[0];
	field+="',city='";
	field+=cont[0];
	field+="'";

	std::string query="value='";
	query+=cont[4];
	query+="'";
	

	sql_api sql;
	sql.sql_connect();
	int ret=0;
	ret=sql.sql_update(field,query);
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
