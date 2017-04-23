#include"log.h"

void print_log(std::string &msg,int l)
{
#ifdef DEBUG
	if(l>=0&&l<4)
		printf("%s %s\n",msg,leve[l]);
#endif
}
