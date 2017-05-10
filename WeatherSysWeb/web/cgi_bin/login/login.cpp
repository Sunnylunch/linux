#include"sql_api.h"
#include"cgi.h"

void Register(std::string tel)
{
	printf("<html>\n<head>\n<title>个人主页</title>\n<script type=\"text/javascript\">\n");
	printf("function home() {window.open(\"/home.html\");}");
	printf("</script>\n</head>\n<body>\n");
	printf("<input type=\"button\" id=\"bt2\" value=\"我的关系\" onclick=\"home()\">");	
	printf("</body>\n</html>\n");
}




int main()
{
	char content_data[_SIZE_]={0};
	if(cgi(content_data)!=0)            //获取参数
	{
		return -1;
	}

	char* start=content_data;
	char* cont[3];
	cont[0]=NULL;
	cont[1]=NULL;
	std::string number;
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
	
	number=cont[0];
	std::string tel;
	std::string inPasswd;
	tel+="'";
	tel+=cont[0];
	tel+="'";
	inPasswd=cont[1];
	
	
	sql_api sql;
	sql.sql_connect();
	
	int ret=0;
	std::string passwd;
	ret=sql.sql_select(tel,passwd);

	if(ret==0)      //查询成功
	{
		if(inPasswd==passwd)      //密码相等
		{
			//成功之后返回一个页面
			Register(number);
			return 0;
		}
	}
	printf("<html>");
	printf("<h1>");
	printf("error");
	printf("</h1>");
	printf("</html>");
	return 0;
}
