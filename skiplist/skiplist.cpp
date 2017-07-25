#include"skiplist.h"
#include<stdlib.h>
#include<stdio.h>

template<class K,class V>
int SkipList<K,V>::Random()
{
	typedef SkipNode<K,V> Node;
	int leve=1;
	while(rand()%2&&leve<=MAXLEVE)  //产生0或1,1的话leve++,最后平均下来leve的值趋向与正态分布
	{
		leve++;
	}
	return leve;
}


template<typename K,typename V>
SkipList<K,V>::SkipList()
{
	typedef SkipNode<K,V> Node;
	_maxLeve=1;
	_size=0;
	_head=new Node(0,-1,MAXLEVE);
	_tail=NULL;
}

template<typename K,typename V>
void SkipList<K,V>::Insert(K key,V value)
{
	typedef SkipNode<K,V> Node;
	int i=_maxLeve-1;
	int j=0;
	Node* cur=_head;			 //指向跳表的最高层的起点
	Node* s[MAXLEVE];			 //用来保存每层向下跳转位置的前驱
	while(i>=0)
	{
		while(cur->_pleve[i])
		{
			if(key>=cur->_pleve[i]->_key)
			{
				cur=cur->_pleve[i];
			}
			else
				break;
		}
		s[j++]=cur;
		i--;
	}
	i=0;     //在最底层插入元素
	int leve=Random();	
	Node* newNode=new Node(key,value,leve);  //创建一个节点
	for(i=0;i<leve;i++)
	{
		if(i<s.size())   
		{
			newNode->_pleve[i]=s[s.size()-i-1]->_pleve[i];
			s[s.size()-i-1]->_pleve[i]=newNode;
		}
		else
		{
			_head->_pleve[i]=newNode;
		}
	}
}

template<typename K,typename V>
bool SkipList<K,V>::Find(K key,V& value)
{
	typedef SkipNode<K,V> Node;
	int i=_maxLeve-1;
	Node* cur=_head;			 //指向跳表的最高层的起点
	while(i>=0)
	{
		while(cur->_pleve[i])
		{
			if(key>=cur->_pleve[i]->_key)
			{
				cur=cur->_pleve[i];
			}
			else
				break;
		}
		i--;
	}
	i=0;
	if(cur->_key==key)                        //最好采用仿函数进行比较
	{
		value=cur->_value;
		return true;
	}
	return false;
}

template<typename K,typename V>
bool SkipList<K,V>::Erase(K key)
{
	typedef SkipNode<K,V> Node;
	int i=_maxLeve-1;
	int j=0;
	Node* cur=_head;			 //指向跳表的最高层的起点
	Node* s[MAXLEVE];			 //用来保存每层向下跳转位置的前驱
	while(i>=0)
	{
		while(cur->_pleve[i])
		{
			if(key>=cur->_pleve[i]->_key)
			{
				cur=cur->_pleve[i];
			}
			else
				break;
		}
		s[j++]=cur;
		i--;
	}

	if(cur->_key==key)
	{
		int leve=cur->_sz;
		Node *del=NULL;
		for(i=0;i<leve;i++)
		{
			del=cur;
			s[s.size()-i-1]->_pleve[i]=cur->_pleve[i];
		}
		delete del;
	}
	return false;
}

template<typename K,typename V>
void SkipList<K,V>::Print()
{
	typedef SkipNode<K,V> Node;
	int i=_maxLeve-1;
	while(i>=0)
	{
		Node* cur=_head;
		printf("this is %d leve:");
		while(cur->_pleve[i])
		{
			cout<<cur->_key<<"["<<cur->_value<<"]"<<" ";
			cur=cur->_pleve[i];
		}
		printf("\n");
		i--;
	}
}

template<typename K,typename V>
SkipList<K,V>::~SkipList()
{

}

