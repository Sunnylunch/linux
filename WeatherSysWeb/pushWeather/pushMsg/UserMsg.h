#ifndef __UserMsg__
#define __UserMsg__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<vector>
#include<mysql.h>
using namespace std;
class UserMsg
{
	public:
		UserMsg();
		~UserMsg();
		int sql_connect();
		int sql_select(std::vector<string> &v);
	private:
		MYSQL * mysql;
};

#endif
