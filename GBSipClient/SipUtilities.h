#ifndef _SIP_UTILITIES_H_
#define _SIP_UTILITIES_H_
#include <string>
#include <random>
#include <chrono>
#include <bitset>
#include <map>
#include <sstream> // std::istringstream所在头文件
#include <time.h>

#ifdef WIN32
#include <objbase.h>
#else
//#include <uuid/uuid.h>
#endif

#ifdef Linux
#include <string.h>
#endif

namespace SipUtilities
{
	bool CompareStrCase(const char* src, const char* dst);
	bool IsPlatform(const std::string deviceID);
	bool WordCanBeFind(const std::string srcStr, const std::string beFindStr);

	int Str2Int(std::string str, int &num);       // 将字符串转换为int，返回值: 0成功，-1失败
	int Str2Double(std::string str, double &num); // 将字符串转换为double，返回值: 0成功，-1失败
	template<typename T>
	int Str2Num(const std::string str, T& num);
	std::string Int2HexStr(int val);
	
	std::string CreatUuid();                     // 生成字符串8-4-4-4-12
	std::string RandLengthStr(const int length); // 生成随机字符长度字符串

	std::string BitsetToHex(std::bitset<8> bite);            // 将bitset<8>转换为16进制表示,每一位为两个字符共16个字符
	int HexToBitset(std::bitset<8> &bitset, std::string hex);// 将16进制表示的1或2个字符转换为bitset<8>类型
	int HexStr2Int(int &num, std::string hex);               // 将字符表示的16进制转换为10进制类型
	int GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8> &bitset);// 从字符串获取pos位置的字符
	
	int GBTime2NptTime(std::string gbTime, int & nptTime); // 将国标年月日时间转换为npt时间
	int GBTime2NptTime(std::string gbTime, std::string & nptTime); // 将国标年月日时间转换为npt时间
	int NptTime2GBTime(std::string nptTime, std::string &gbTime);

	std::string GetLocalTime();

	int32_t ParseKeyVal(const char* str, int32_t size, char delimiter, std::string& key, std::string& val);
	int32_t GetKeyValMap(std::string str, std::map<std::string, std::string> &keyValMap);
	std::string GetValByKey(const std::map<std::string, std::string> &keyValMap, const char* key);
};

#endif // _SIP_UTILITIES_H_