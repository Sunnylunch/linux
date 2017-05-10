#ifndef __weather__
#define __weather__
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<vector>
#include<mysql.h>
using namespace std;
class weather
{
	public:
		weather();
		~weather();
		int sql_connect();
		int sql_select(std::string v,std::string &outString);
	private:
		MYSQL * mysql;
};

#endif
