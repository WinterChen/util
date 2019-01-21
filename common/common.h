#ifndef UTIL_COMMON_H
#define UTIL_COMMON_H
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
namespace utilcommon{


//num <-> string
//模板函数：将string类型变量转换为常用的数值类型
template <class Type>
Type StringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;    
}
template <class Type>
string NumToString(Type i)
{
    stringstream ss;
    ss<<i;
    return ss.str();
}


//ip: num <-> string
unsigned IpStringToU32(const char* pAddr);
unsigned IpStringToU32(const string& strAddr);
string IpU32ToString(unsigned ipv4);

//time
int GetTimeElapsed(long long tMs);
long long NowMs() ;

//parse string
void SplitString(const string& s, vector<string>& v, const string &c);
void ParseKeyValue(const string& s, string &key, string &value,const string &c);

//sys
string GetInnerIp();
string GetHostName();

}
#endif // !COMMON_H
