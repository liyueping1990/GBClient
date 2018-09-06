#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <bitset>
#include <map>
#include <sstream>

#ifdef WIN32
#include <objbase.h>
#else
//#include <uuid/uuid.h>
#endif

namespace SipCommon
{
	bool CompareStr(const char* src, const char* dst, bool ignore = true);
	
	std::string CreatUuid();
	std::string RandLengthStr(const int length); // ��������ַ������ַ���

	std::string GetLocalTimeInMS();
	int GBTime2NptTime(std::string gbTime, int & nptTime); // ������������ʱ��ת��Ϊnptʱ��
	int GBTime2NptTime(std::string gbTime, std::string & nptTime); // ������������ʱ��ת��Ϊnptʱ��
	int NptTime2GBTime(std::string nptTime, std::string &gbTime);

	int Str2Int(std::string str, int &num);       // ���ַ���ת��Ϊint������ֵ: 0�ɹ���-1ʧ��
	int Str2Double(std::string str, double &num); // ���ַ���ת��Ϊdouble������ֵ: 0�ɹ���-1ʧ��
	std::string Int2HexStr(int val);

	std::string BitsetToHex(std::bitset<8> bite);            // ��bitset<8>ת��Ϊ16���Ʊ�ʾ,ÿһλΪ�����ַ���16���ַ�
	int HexToBitset(std::bitset<8> &bitset, std::string hex);// ��16���Ʊ�ʾ��1��2���ַ�ת��Ϊbitset<8>����
	int HexStr2Int(int &num, std::string hex);               // ���ַ���ʾ��16����ת��Ϊ10��������
	int GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8> &bitset);
	
	template<typename T>
	int Str2Num(const std::string str, T & num)
	{
		int result = -1;
		std::istringstream ss(str);
		if (ss >> num)
		{
			result = 0;
		}
		return result;
	}
};
