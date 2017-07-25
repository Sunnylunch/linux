#ifndef _SKIPLIST_H__
#define _SKIPLIST_H__

#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define MAXLEVE 8

template<typename K,typename V>
struct SkipNode                                           //跳表的节点类型
{
   K _key;			
   V _value;  
   size_t _sz;                                            //表示该节点的层数
   vector<SkipNode<K,V> *> _pleve;                        //存放每一层的指针
   SkipNode(K key=K(),V value=V(),size_t sz=size_t())
	   :_key(key)
		,_value(value)
		,_sz(sz)
	{
		_pleve.resize(0);
		for(size_t i=0;i<sz;i++)
		{
			_pleve.push_back(NULL);
		}
	}

   ~SkipNode()
   {}
};


template<typename K,typename V>
class SkipList                                  //跳表类型
{
	typedef SkipNode<K,V> Node;
public:
	SkipList();
	void Insert(K key,V value);
	bool Find(K key,V& value);
	bool Erase(K key);
	void Print();
	~SkipList();
private:
	int Random();
protected:                                      
	SkipList(SkipList<K,V> &);                  //防拷贝
	SkipList<K,V>& operator=(SkipList<K,V>);    //防赋值
private:
	Node *_head,*_tail;
	int _maxLeve;                               //记录跳表的最大层数
	int _size;                                  //记录跳表最底层元素的个数
};

#endif                                          //_SKIPLIST_H__

