#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include<pthread.h>
#include<cstdio>
#include<list>
#include<exception>
#include<iostream>

#include"locker.h"


//为了提高复用性，我们用模板实现线程池，模板类型T是任务类


template<typename T>
class threadpool
{
public:
	threadpool(int thread_number=8,int max_requests=1000);
	~threadpool();
	bool append(T* request);        //向请求队列中添加请求
private:
	static void* woker(void *arg);  //线程执行函数
	void run();						//实际运行线程的函数
private:
	int m_thread_number;			//线程的数量
	int m_max_requests;             //请求队列的最大容量
	pthread_t *m_threads;			//指向管理线程tid的数组
	std::list<T*> m_workqueue;		//请求队列
	locker m_queuestat;				//保护请求队列的互斥锁
	sem m_queuestat;				//请求队列中是否有任务要处理
	bool m_stop;					//是否结束线程
};

#endif      
