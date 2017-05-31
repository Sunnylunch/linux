#define _CRT_SECURE_NO_WARNINGS
#include"BigData.h"
using namespace std;

BigData::BigData(INIT64  value)
	:_value(value)
{
	char buf[128] = { 0 };
	sprintf(buf,"%lld",_value);
	_strData =buf;
}


BigData::BigData(string strData)
	:_value(0)
	, _strData("+0")
{
	char* pData = (char*)strData.c_str();
	while (isspace(*pData))              //过滤空白
		pData++;
	if ('\0' == *pData)
		return;

	char symbol = *pData;
	if (('+' == *pData) || ('-' == *pData))    //过滤符号
		pData++;
	else if (isdigit(*pData))
		symbol = '+';
	else
		return;

	while('0' == *pData)         //过滤前面的0
		pData++;
	if ('\0' == *pData)
		return;

	_strData.resize(strlen(pData)+1);
	_strData[0] = symbol;
	int count = 1;
	while (*pData>='0' && *pData<='9')
	{
		_value = _value * 10 + *pData - '0';
		_strData[count++] = *pData;
		pData++;
	}
	if (*pData)
		_strData.resize(count);

	if (symbol == '-')
		_value = 0 - _value;
}

ostream& operator<<(ostream& os, const BigData& b)
{
	const char *str = b._strData.c_str();
	if ('+' == *str)
		cout << str + 1;
	else
		cout << str;
	return os;
}

bool BigData::isINIT64overflow(const string& strData)
{
	const string max_value= "+9223372036854775807";
	const string min_value= "-9223372036854775808";
	if (strData.size() < max_value.size())
		return false;
	else if (strData.size() == max_value.size())
	{
		if (('+'==strData[0]&&strData<=max_value)\
			||('-'==strData[0]&&strData>=min_value))
			return false;
	}
	return true;
}


BigData BigData::operator+(BigData b)
{
	if (!isINIT64overflow(_strData) && !isINIT64overflow(b._strData))   //如果都没溢出
	{
		if (_strData[0] != b._strData[0])     //异号直接相加，不会溢出
			return _value + b._value;
		else if ('+' == _strData[0] && '+' == b._strData[0])
		{
			INIT64 max_value = 9223372036854775807;
			if (max_value - _value >= b._value)       //两个正数相加不会溢出
			{
				return _value + b._value;
			}
		}
		else if ('-' == _strData[0] && '-' == b._strData[0])
		{
			INIT64 min_value =0-9223372036854775808;
			if (min_value - _value <= b._value)   //两个负数相加不会溢出
			{
				return _value + b._value;
			}
		}
	}

	if (_strData[0] == b._strData[0])      //同号相加
		return BigData(Add(_strData, b._strData));
	else                               
	{
		string Left = _strData;       //异号相加转换成正号相减
		string Right = b._strData;
		Left[0] = '+';
		Right[0] = '+';
		if ('-' == _strData[0])        
			swap(Left,Right);
		return BigData(Sub(Left,Right));
	}

}

BigData BigData::operator-(BigData b)
{
	if (!isINIT64overflow(_strData) && !isINIT64overflow(b._strData))   //如果都没溢出
	{
		if (_strData[0] == b._strData[0])     //同号直接相减，不会溢出
			return _value -b._value;
		else if ('+' == _strData[0] && '-' == b._strData[0])     //正数减负数
		{
			INIT64 max_value = 9223372036854775807;
			if ((max_value - _value) >= (0-b._value))     
			{
				return _value - b._value;
			}
		}
		else if ('-' == _strData[0] && '+' == b._strData[0])      //负数减正数
		{
			INIT64 min_value = 0 - 9223372036854775808;
			if ((min_value - _value) <= (0-b._value))        
			{
				return _value + b._value;
			}
		}
	}

	if (_strData[0]==b._strData[0])      //同号相减
		return BigData(Sub(_strData,b._strData));  
	else								//异号相减
	{
		string left = _strData;
		string right = b._strData;

		//异号相减，转换为相加
		left[0] = '+';
		right[0] = '+';
		string strTemp = Add(left, right);
		if('-' == _strData[0])
			strTemp[0] = '-';
		return BigData(strTemp);
	}
}


BigData BigData::operator * (BigData b)
{
	if ("+0" == _strData || "+0" == b._strData)
		return BigData(0);

	if (!isINIT64overflow(_strData) && !isINIT64overflow(b._strData))
	{
		INIT64 max_value = 9223372036854775807;
		INIT64 min_value = 0 - 9223372036854775808;
		if (_strData[0] == b._strData[0])
		{
			if (('+' == _strData[0] && max_value / _value >= b._value) || \
				('-' == _strData[0] && max_value / _value <= b._value))
				return _value*b._value;
		}
		else
		{
			if (('+' == _strData[0] && min_value / _value <= b._value) || \
				('-' == _strData[0] && min_value / _value >= b._value))
				return BigData(_value*b._value);
		}
	}

	//判断运算数中有没有为正负1的特殊情况
	if ("+1"==_strData)
		return BigData(b._strData);
	if ("+1" == b._strData)
		return BigData(_strData);
	if ("-1" == _strData)
	{
		string ret = b._strData;
		if ('+' == b._strData[0])
			ret[0] = '-';
		else
			ret[0] = '+';
		return BigData(ret);
	}
	if ("-1" == b._strData)
	{
		string ret =_strData;
		if ('+' ==_strData[0])
			ret[0] = '-';
		else
			ret[0] = '+';
		return BigData(ret);
	}
	return BigData(Mul(_strData, b._strData));
}

BigData BigData::operator/(BigData b)
{
	if ("+0" == b._strData)
	{
		cout << "exception" << endl;
		return BigData("");
	}

	if (!isINIT64overflow(_strData) && !isINIT64overflow(b._strData))
	{
		return _value / b._value;
	}

	if ("+0" == _strData || _strData.size()<b._strData.size() || \
		(_strData.size()==b._strData.size() && strncmp(_strData.c_str()+1,b._strData.c_str()+1,_strData.size()-1)<0))
		return BigData(0);

	if (_strData.size() == b._strData.size() && strncmp(_strData.c_str() + 1, b._strData.c_str() + 1, _strData.size() - 1)==0)
	{
		if (_strData[0]==b._strData[0])
			return BigData(1);
		else
			return BigData(-1);
	}

	if ("+1" == b._strData)
		return BigData(_strData);
	else if ("-1" == b._strData)
	{
		string ret= _strData;
		ret[0] = '+';
		if ('+' == _strData[0])
			ret[0] = '-';
		return BigData(ret);
	}

	return BigData(Div(_strData,b._strData));
}



string BigData::Div(string strLeft, string strRight)
{
	char symbol = '+';
	if (strLeft[0] != strRight[0])
		symbol = '-';
	size_t LSize = strLeft.size();
	size_t RSize = strRight.size();
	char *pLeft = (char*)strLeft.c_str()+1;
	char *pRight = (char*)strRight.c_str()+1;
	size_t dataLen =0;
	string strTemp;
	strTemp.append(1,symbol);

	while ('\0'!=(*(pLeft+dataLen-1)))
	{
		if ('0' == *pLeft)
		{
			pLeft++;
			strTemp.append(1, '0');
			continue;
		}

		if (!isLgtR(pLeft, dataLen, pRight, RSize-1))
		{
			strTemp.append(1,'0');
			dataLen++;
		}
		else
		{
			strTemp.append (1,SubLoop(pLeft, dataLen, pRight, RSize)+'0');
			dataLen++;
		}
	}
	return strTemp;
}

char BigData::SubLoop(char* &pLeft, size_t& dataLen, const char *pRight,const size_t RSize)
{
	char count = 0;
	while (isLgtR(pLeft, dataLen, pRight, RSize-1))
	{
		for (size_t index = 1; index <= dataLen; index++)
		{
			char ret = *(pLeft + dataLen - index) - '0';
			if (RSize>index)
			{
				ret -= (*(pRight + RSize - index-1) - '0');
			}
			if (ret < 0)
			{
				*(pLeft + dataLen - index - 1) -= 1;
				ret += 10;
			}
			*(pLeft + dataLen - index) = ret + '0';
		}
		count++;
		while ('0' == *pLeft&&dataLen>0)
		{
			pLeft++;
			dataLen--;
		}
	}
	return count;
}

//判断以pLeft开始dataLen长度所表示的数的大小是不是大于除数
bool BigData::isLgtR(const char* pLeft,const size_t dataLen, const char *pRight, const size_t RSize)   
{
	if (dataLen > RSize)
		return true;
	else if (dataLen == RSize)
	{
		if (strncmp(pLeft, pRight, dataLen) >= 0)
			return true;
	}
	return false;
}


string BigData::Mul(string strLeft, string strRight)
{
	size_t LSize = strLeft.size();
	size_t RSize = strRight.size();
	
	//乘法相乘的时候，保证*号左边的数的长度小于等于*号右边的数
	if(LSize > RSize)
	{
		swap(LSize,RSize);
		swap(strLeft,strRight);
	}
	char symbol = '+';
	if (strLeft[0] != strRight[0])         //异号相乘为负
		symbol='-';

	string strTemp;
	strTemp.resize(LSize+RSize-1,'0');      //两数相乘，结果的位数最长是两个运算数位数之和
	strTemp[0] = symbol;

	//因为两个数相乘，即乘数的每一位都要和被乘数相运算，所以必须用两个循环
	for (size_t i = 1; i < LSize;i++)
	{
		if ('0' == strLeft[LSize - i])
			continue;
		char step=0;                         //记录进位
		for (size_t j=1;j<RSize;j++)
		{
			char ret = strLeft[LSize-i]-'0';
			ret*=(strRight[RSize-j]-'0');
			ret += step+strTemp[LSize + RSize - j - i] - '0';
			strTemp[LSize + RSize - j - i]= ret%10+'0';
			step = ret / 10;
		}
		strTemp[LSize- i] += step;
	}
	return strTemp;
}

string BigData::Sub(string strLeft, string strRight)      //同号相减
{
	size_t LSize = strLeft.size();
	size_t RSize = strRight.size();

	string left = strLeft;
	string right = strRight;
	left[0] = '+';
	right[0] = '+';

	char symbol='+';
	if ((LSize > RSize)||(LSize==RSize && strLeft>=strRight))     //左边的数比右边的数大,可以减过
	{
		if ('-' == strLeft[0])     //负数减负数
			symbol = '-';
	}
	else            //右边数比左边大，减不过
	{
		if ('+' == strLeft[0])   //正数减正数
			symbol = '-';
		swap(left,right);
		swap(LSize,RSize);
	}

	//统一转换成 a-b或-(a-b)的形式,且(a>=b>=0)

	string strTemp;
	strTemp.resize(LSize);
	strTemp[0] = symbol;

	for (size_t index = 1; index < LSize;index++)
	{
		char ret = left[LSize-index]-'0';
		if (RSize>index)
		{
			ret = ret - (right[RSize - index] - '0');
		}
		if (ret < 0)                   //减不过要借位
		{
			left[LSize - index - 1] -= 1;     
			ret += 10;
		}
		strTemp[LSize - index] = ret+'0';
	}
	return strTemp;
}


string BigData::Add(string strLeft,string strRight)
{
	size_t LSize = strLeft.size();
	size_t RSize = strRight.size();
	if (LSize < RSize)                 //为了方便，我们保证+号左边的数的位数不小于加号右边的数
	{
		swap(strLeft,strRight);
		swap(LSize,RSize);
	}

	string strTemp;
	strTemp.resize(LSize+1);                 //两个数相加，结果的位数最长是两个运算数中位数最长的运算数加+1
	strTemp[0] = strLeft[0];                 
	char step = 0;                            //记录进位
	for (size_t index = 1; index < LSize;index++)
	{
		char ret = strLeft[LSize - index]-'0';
		ret += step;
		if (RSize>index)
		{
			ret=ret+strRight[RSize-index] - '0';
		}
		step = ret / 10;                     //保存进位情况
		strTemp[LSize + 1 - index] = ret%10+'0';  
	}
	strTemp[1] = step+'0';                   //向最高位进位，取决于step的值
	return strTemp;
}


