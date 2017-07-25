#include"skiplist.cpp"

void test()
{
	SkipList<int,int> s;
	s.Insert(1,1);
	s.Insert(2,2);
	s.Insert(3,3);
}

int main()
{
	test();
	return 0;
}
