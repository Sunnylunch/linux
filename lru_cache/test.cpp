#include"lru.hpp"


int main()
{
	lruCache<int, char> lru(10);
	lru.lruCacheSet(1,'a');
	lru.lruCacheSet(2, 'b');	
	lru.lruCacheSet(3, 'c');
	lru.lruCacheGet(1);
	lru.lruCacheSet(4, 'd');
	lru.lruCacheSet(5, 'e');
	lru.lruCacheSet(6, 'f');
	lru.lruCacheSet(7, 'g');
	lru.lruCacheGet(2);
	

	cout << lru.lruCacheGet(3) << endl;
	cout << lru.lruCacheGet(2) << endl;
	cout << lru.lruCacheGet(3) << endl;
	cout << lru.lruCacheGet(1) << endl;
	cout << lru.lruCacheGet(8) << endl;



	system("pause");
	return 0;
}
