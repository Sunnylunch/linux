#ifndef __SQL_API__
#define __SQL_API__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<mysql.h>

using namespace std;
class sql_api
{
	public:
		sql_api();

		~sql_api();

		int sql_connect();
         
		int sql_select(std::string tel,std::string &passwd);

	private:
		MYSQL * mysql;
};

#endif
