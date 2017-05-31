#include<string>
#include<iostream>
#include<cstring>
using namespace std;

typedef long long INIT64;

class BigData
{
	friend ostream& operator<<(ostream& os,const BigData& b);
public:
	BigData(INIT64  value=0);
	BigData(string strData);
	BigData operator+(BigData b);
	BigData operator-(BigData b);
	BigData operator*(BigData b);
	BigData operator/(BigData b);
private:
	bool isINIT64overflow(const string& strData);
	string Add(string strLeft, string strRight);
	string Sub(string strLeft, string strRight);
	string Mul(string strLeft, string strRight);
	string Div(string strLeft, string strRight);
	char SubLoop(char* &pLeft, size_t& dataLen, const char *pRight, const size_t RSize);
	bool isLgtR(const char* pLeft, const size_t dataLen, const char *pRight, const size_t RSize);
private:
	INIT64 _value;
	string _strData;
};
