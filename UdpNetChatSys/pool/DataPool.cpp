#include"DataPool.h"

data_pool::data_poll()
	:_cap(CAPACITY)
	,_Product(0)
	,_Consume(0)
	,_datapool(CAPACITY)
{
	sem_init(&_blank,0,CAPACITY);
	sem_init(&_data,0,0);
}


void data_pool::getData(std::string &inString)
{
	sem_wait(&_data);
	inString=_datapool[_Consume_post];
	_Consume_post++;
	_Consume_post%=_cap;
	sem_post(&_blank);
}

void data_pool::putData(std::string &outString)
{
	sem_wait(&_blank);
	_datapool[_Product_post]=outString;
	_Product_post++;
	_Product_post%=_cap;
	sem_post(&_data);
}

data_pool::~data_poll()
{
	_cap=0;
	_size=0;
	sem_destory(&_blank);
	sem_destory(&_data);
}
