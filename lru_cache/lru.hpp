#pragma once
#include<iostream>

using namespace std;

//缓存链表、hash表的结点类型
template<typename K, typename T>
struct cacheNode
{
	K key;
	T data;
	struct cacheNode<K, T> *hashListPrev;  //指向hash链表的前一个结点
	struct cacheNode<K, T> *hashListNext;  //指向hash链表的后一个结点

	struct cacheNode<K, T> *lruListPrev;   //指向缓存双向链表的前一个结点
	struct cacheNode<K, T> *lruListNext;   //指向缓存双向链表的后一个结点
	cacheNode(K k = K(), T d = T())
		:key(k)
		, data(d)
		, hashListPrev(NULL)
		, hashListNext(NULL)
		, lruListPrev(NULL)
		, lruListNext(NULL)
	{}
};

template<typename K> 
struct isEqual                 //定义key值的比较方式，相等返回true
{
	bool operator()(const K left,const K right)
	{
		if (left == right)
			return true;
		else
			return false;
	}
};


//缓存类
template<typename K, typename T,class Compare=isEqual<K> >
class lruCache
{
	typedef cacheNode<K, T> Node;
public:
	lruCache(size_t capacity);
	~lruCache();
	void lruCacheSet(K key, T data);  //向缓存中放入数据
	bool lruCacheGet(K key,T& data);  //从缓存中得到数据
private:
	void RemoveFromList(Node* post);    //从双向链表中删除指定结点
	cacheNode<K, T>* InsertToListHead(Node* node);  //向双向链表的表头插入数据
	void UpdateLRUList(Node* node);
	void DeleteList(Node* head);        //删除整个双向链表	
	int HashFunc(K key);           //获取hash值
	cacheNode<K, T>* GetValueFromHashMap(K key);  //从hashmap中获取一个缓存单元
	void InsertValueToHashMap(Node* node); //插入一个缓存单元到hashmap
	void RemoveFromeHashMap(Node* node);  //从hashmap中删除一个缓存单元
private:
	size_t _capacity;     //缓存的容量
	Node** _hashMap;      //hash数组，
	Node* _lruListHead;    //指向缓存双向链表的头部
	Node* _lruListTail;    //指向缓存双向链表的尾部
	size_t _lruListSize;   //记录双向链表结点中的个数
};


template<typename K, typename T, class Compare = isEqual<K>>
lruCache<K, T,Compare>::lruCache(size_t capacity)
:_capacity(capacity)
,_hashMap(new cacheNode<K,T>*[capacity])
, _lruListHead(NULL)
, _lruListTail(NULL)
, _lruListSize(0)
{
	memset(_hashMap, 0, sizeof(cacheNode<K, T>*)*capacity);
}

template<typename K, typename T, class Compare = isEqual<K>>
lruCache<K, T, Compare>::~lruCache()
{
	DeleteList(_lruListHead);
	delete[] _hashMap;
	_capacity = 0;
	_hashMap = NULL;
	_lruListHead = NULL;
	_lruListTail = NULL;
	_lruListSize = 0;
}

template<typename K, typename T, class Compare = isEqual<K>>
void lruCache<K, T, Compare>::lruCacheSet(K key, T data)    //向缓存中放入数据
{
	Node* node = GetValueFromHashMap(key);        //从hashmap中获取一个缓存单元
	if (NULL == node)                               //缓存不在hashmap中，创建一个新的缓存
	{
		node = new Node(key, data);
		InsertValueToHashMap(node);          //将缓存结点插入到hashmap中
		Node* lastNode = InsertToListHead(node);   //将缓存结点插入到链表头部
		if (NULL != lastNode)                       //插入链表的过程中发生数据溢出
		{
			RemoveFromeHashMap(lastNode);        //从hashmap中移除这个单元
			delete lastNode;
		}
	}
	else
	{
		UpdateLRUList(node);
	}
}

template<typename K, typename T, class Compare = isEqual<K>>
bool lruCache<K, T, Compare>::lruCacheGet(K key,T& data)  //从缓存中得到数据
{
	Node* node = GetValueFromHashMap(key);        //从hashmap中获取一个缓存单元	
	if (NULL != node)
	{
		UpdateLRUList(node);
		data=node->data;
		return true;
	}
	return false;
}

template<typename K, typename T, class Compare = isEqual<K>>
void lruCache<K, T, Compare>::RemoveFromList(Node* post)    //从双向链表中移除指定结点
{
	if (NULL == _lruListHead)
		return;

	if (_lruListHead == _lruListTail)       //链表只有一个结点
	{
		_lruListHead = _lruListTail = NULL;
	}
	else if (_lruListHead == post)
	{
		_lruListHead = _lruListHead->lruListNext;
		_lruListHead->lruListPrev = NULL;
	}
	else if (_lruListTail == post)
	{
		_lruListTail = _lruListTail->lruListPrev;
		_lruListTail->lruListNext = NULL;
	}
	else
	{
		post->lruListPrev->lruListNext = post->lruListNext;
		post->lruListNext->lruListPrev = post->lruListPrev;
	}

	--_lruListSize;
}


template<typename K, typename T, class Compare = isEqual<K>>
cacheNode<K, T>* lruCache<K, T, Compare>::InsertToListHead(Node* node)  //向双向链表的表头插入数据
{
	Node* lastNode = NULL;
	if (++_lruListSize>_capacity)
	{
		lastNode = _lruListTail;
		RemoveFromList(lastNode);
	}

	if (NULL == _lruListHead)
	{
		_lruListHead = _lruListTail = node;
		node->lruListPrev = NULL;
		node->lruListNext = NULL;
	}
	else
	{
		node->lruListNext = _lruListHead;
		_lruListHead->lruListPrev = node;
		_lruListHead = node;
		node->lruListPrev = NULL;
	}
	return lastNode;
}

template<typename K, typename T, class Compare = isEqual<K>>
void lruCache<K, T, Compare>::UpdateLRUList(Node* node)
{
	RemoveFromList(node);    //从双向链表中移除指定结点
	InsertToListHead(node);   //将缓存结点插入到链表头部	
}

template<typename K, typename T, class Compare = isEqual<K>>
void lruCache<K, T, Compare>::DeleteList(Node* head)
{
	Node* cur = head;
	while (NULL != cur)
	{
		head = head->lruListNext;
		delete cur;
		cur = head;
	}
}

template<typename K, typename T, class Compare = isEqual<K>>
int lruCache<K, T, Compare>::HashFunc(K key)
{
	return key%_capacity;
}

template<typename K, typename T, class Compare = isEqual<K>>
cacheNode<K, T>* lruCache<K, T, Compare>::GetValueFromHashMap(K key)  //从hashmap中获取一个缓存单元
{
	Node* cur = _hashMap[HashFunc(key)];
	while (NULL != cur)
	{
		if (Compare()(key, cur->key))                     
			return cur;
		cur = cur->hashListNext;
	}
	return NULL;
}


template<typename K, typename T, class Compare = isEqual<K>>
void lruCache<K, T, Compare>::InsertValueToHashMap(Node* node) //插入一个缓存单元到hashmap
{
	Node* cur = _hashMap[HashFunc(node->key)];
	if (NULL == cur)
	{
		_hashMap[HashFunc(node->key)] = node;
		node->hashListPrev = NULL;
		node->hashListNext = NULL;
	}
	else
	{
		node->hashListNext = cur;
		cur->hashListPrev = node;
		_hashMap[HashFunc(node->key)] = node;
		node->hashListPrev = NULL;
	}
}

template<typename K, typename T, class Compare = isEqual<K>>
void lruCache<K, T, Compare>::RemoveFromeHashMap(Node* node)  //从hashmap中移除一个缓存单元
{
	if (NULL != node)
	{
		if (NULL == node->hashListPrev)    //要移除的结点在链表的首部
		{
			_hashMap[HashFunc(node->key)] = node->hashListNext;
			if (node->hashListNext)
				node->hashListNext->hashListPrev = NULL;
		}
		else
		{
			node->hashListPrev->hashListNext = node->hashListNext;
			if (node->hashListNext)
				node->hashListNext->hashListPrev = node->hashListPrev;
		}
	}
}

