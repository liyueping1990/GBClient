#ifndef _SIP_UTILITIES_H_
#define _SIP_UTILITIES_H_
#include <string>
#include <random>
#include <chrono>
#include <bitset>
#include <map>
#include <sstream> // std::istringstream����ͷ�ļ�
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

	int Str2Int(std::string str, int &num);       // ���ַ���ת��Ϊint������ֵ: 0�ɹ���-1ʧ��
	int Str2Double(std::string str, double &num); // ���ַ���ת��Ϊdouble������ֵ: 0�ɹ���-1ʧ��
	template<typename T>
	int Str2Num(const std::string str, T& num);
	std::string Int2HexStr(int val);
	
	std::string CreatUuid();                     // �����ַ���8-4-4-4-12
	std::string RandLengthStr(const int length); // ��������ַ������ַ���

	std::string BitsetToHex(std::bitset<8> bite);            // ��bitset<8>ת��Ϊ16���Ʊ�ʾ,ÿһλΪ�����ַ���16���ַ�
	int HexToBitset(std::bitset<8> &bitset, std::string hex);// ��16���Ʊ�ʾ��1��2���ַ�ת��Ϊbitset<8>����
	int HexStr2Int(int &num, std::string hex);               // ���ַ���ʾ��16����ת��Ϊ10��������
	int GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8> &bitset);// ���ַ�����ȡposλ�õ��ַ�
	
	int GBTime2NptTime(std::string gbTime, int & nptTime); // ������������ʱ��ת��Ϊnptʱ��
	int GBTime2NptTime(std::string gbTime, std::string & nptTime); // ������������ʱ��ת��Ϊnptʱ��
	int NptTime2GBTime(std::string nptTime, std::string &gbTime);

	std::string GetLocalTime();

	int32_t ParseKeyVal(const char* str, int32_t size, char delimiter, std::string& key, std::string& val);
	int32_t GetKeyValMap(std::string str, std::map<std::string, std::string> &keyValMap);
	std::string GetValByKey(const std::map<std::string, std::string> &keyValMap, const char* key);
};

#endif // _SIP_UTILITIES_H_