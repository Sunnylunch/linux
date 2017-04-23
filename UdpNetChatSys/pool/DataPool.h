#ifndef _DATA_POOL_
#define _DATA_POOL_
#include<iostream>
#include<vector>
#include<semaphore.h>

#define CAPACITY 1024
class data_pool
{
public:
	data_poll();
	getData(std::string &inString);
	putData(std::string &outString);
	~data_poll();
private:
	int _cap;
	int _Product_post;
	int _Consume_post;
	vector<string> _datapool;
	sem_t _blank;
	sem_t _data;
};

#endif
