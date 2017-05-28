#include"threadpool.h"

template<typename T>
threadpool<T>::threadpool(int thread_number,int max_requests)
	:m_thread_number(thread_number)
	 ,m_max_requests(max_requests)
	 ,m_stop(false)
	 ,m_threads(NULL)
{
	if((thread_number<=0)||(max_requests<=0))
	{
		throw std::exception();
	}

	m_threads=new pthread_t[thread_number];  
	if(!m_threads)
	{
		throw std::exception();
	}
	
	for(int i=0;i<thread_number;i++)          //创建thread_number个线程，并且将其设置为分离状态
	{
		if(pthread_create(m_threads+i,NULL,worker,this)!=0)
		{
			delete [] m_threads;
			throw std::exception();
		}

		if(pthread_detach(m_thread[i]))
		{
			delete [] m_threads;
			throw std::exception();
		}
	}
}

template<typename T>
threadpool<T>::~threadpool()
{
	delete [] m_threads;
	m_stop=true;
}

template<typename T>
bool threadpool<T>::append(T* request)         //向请求队列中添加请求任务
{
	m_queuelocker.lock();						
	if(m_workqueue.size()>m_max_requests)      //确保请求队列中没有被任务堆积满
	{
		m_queuework.unlock();
		return false;
	}
	m_workqueue.push_back(request);
	m_queuelocker.unlock();
	m_queuestat.post();                         //没添加一个任务，信号量增加
	return true;
}
	
template<typename T>
static void* threadpool<T>::woker(void *arg)
{
	threadpool *pool=(threadpool*)arg;
	pool->run();						//调用run函数处理请求队列中的请求任务
	return pool;
}


template<typename T>
void threadpool<T>::run()           //处理请求队列中的请求任务
{
	while(!m_stop)
	{
		m_queuestat.wait();
		m_queuelocker.lock();
		if(m_workqueue.empty())
		{
			m_queuework.unlock();
			continue;
		}

		T* request=m_workqueue.front();
		m_workqueue.pop_front();
		m_queuelocker.unlock();
		request->process();      //process是任务类里面的一个方法
	}
}


