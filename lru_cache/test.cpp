int main()
{
	lruCache<int, char> lru(10);
	lru.lruCacheSet(1,'a');
	lru.lruCacheSet(2, 'b');	
	lru.lruCacheSet(3, 'c');
	lru.lruCacheSet(4, 'd');
	lru.lruCacheSet(5, 'e');
	lru.lruCacheSet(6, 'f');
	lru.lruCacheSet(7, 'g');

	char c = 0;

	cout << lru.lruCacheGet(3,c) << endl;
	cout << lru.lruCacheGet(2,c) << endl;
	cout << lru.lruCacheGet(3,c) << endl;
	cout << lru.lruCacheGet(1,c) << endl;
	cout << lru.lruCacheGet(8,c) << endl;


	system("pause");
	return 0;
}
