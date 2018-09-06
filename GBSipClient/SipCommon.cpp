#include "SipCommon.h"

bool SipCommon::CompareStr(const char * src, const char * dst, bool ignore)
{
	bool isEquel = false;
#ifdef WIN32
	if (_strcmpi(src, dst) == 0)
#else
	if (strcasecmp(src, dst) == 0)
#endif
	{
		isEquel = true;
	}
	return isEquel;
}

std::string SipCommon::CreatUuid()
{
	char dstString[64] = { 0 };

	GUID uuid;
	::CoCreateGuid(&uuid);
	_snprintf_s(dstString, 64 - 1, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.Data1, uuid.Data2, uuid.Data3, uuid.Data4[0], uuid.Data4[1], uuid.Data4[2],
		uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);

	return std::string(dstString);
}

std::string SipCommon::RandLengthStr(const int length)
{
	std::string randStr = "";

	static std::default_random_engine e(time(NULL));
	static std::uniform_int_distribution<unsigned> u(0, 26);

	for (int i = 0; i < length; ++i)
	{
		switch ((u(e) % 3))
		{
		case 1:
			randStr = randStr + (char)('A' + u(e) % 26);
			break;
		case 2:
			randStr = randStr + (char)('a' + u(e) % 26);
			break;
		default:
			randStr = randStr + (char)('0' + u(e) % 10);
			break;
		}
	}
	return randStr;
}

std::string SipCommon::GetLocalTimeInMS()
{
	auto timeNow = std::chrono::system_clock::now();
	auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow.time_since_epoch());
	auto timeMs = timeInMs - std::chrono::duration_cast<std::chrono::seconds>(timeInMs);

	time_t rawTime = std::chrono::system_clock::to_time_t(timeNow);

	tm* t = localtime(&rawTime);

	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%dT%X.", t);
	std::string timeString = tmp + std::to_string(timeMs.count());
	return timeString;
}

int SipCommon::GBTime2NptTime(std::string gbTime, int & nptTime)
{
	int result = -1;
	int year, month, day, hour, min, sec;
	result = sscanf(gbTime.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &min, &sec);
	if (result == 6)
	{
		tm t;
		memset(&t, 0, sizeof(t));
		t.tm_year = year - 1900;
		t.tm_mon = month - 1;
		t.tm_mday = day;
		t.tm_hour = hour;
		t.tm_min = min;
		t.tm_sec = sec;
		nptTime = mktime(&t);
		result = 0;
	}
	return result;
}

int SipCommon::GBTime2NptTime(std::string gbTime, std::string & nptTime)
{
	int result = -1;
	int time = 0;
	result = GBTime2NptTime(gbTime, time);
	nptTime = std::to_string(time);
	return result;
}

int SipCommon::NptTime2GBTime(std::string nptTime, std::string & gbTime)
{
	int result = -1;

	time_t rawTime = 0;
	result = Str2Num(nptTime, rawTime);
	if (result == 0)
	{
		tm* t = localtime(&rawTime);

		char tmp[64] = { 0 };
		strftime(tmp, sizeof(tmp), "%Y-%m-%dT%X", t);
		gbTime = tmp;
	}
	return result;
}


int SipCommon::Str2Int(std::string str, int &num)
{
	int result = -1;
	try
	{
		num = std::stoi(str);
		result = 0;
	}
	catch (const std::exception& p)
	{
		printf(p.what());
	}
	return result;
}

int SipCommon::Str2Double(std::string str, double & num)
{
	int result = -1;

	try
	{
		num = std::stod(str);
		result = 0;
	}
	catch (const std::exception & e)
	{
		printf(e.what());
	}
	return result;
}

std::string SipCommon::Int2HexStr(int val)
{
	char buff[16] = { 0 };
	sprintf(buff, "0x%04X", val);
	return buff;
}

std::string SipCommon::BitsetToHex(std::bitset<8> bite)
{
	char a[8] = { 0 };
	sprintf(a, "%02X", bite.to_ulong());
	return a;
}

int SipCommon::HexToBitset(std::bitset<8>& bitset, std::string hex)
{
	if (hex.size() >= 3)
	{
		return -1;
	}
	for (int i = 0; i < hex.size(); i++)
	{
		if (hex[i] < 48 || hex[i] > 122)
		{
			return -1;
		}
		else if (hex[i] > 58 && hex[i] < 65)
		{
			return -1;
		}
		else if (hex[i] > 91 && hex[i] < 97)
		{
			return -1;
		}
	}
	bitset = stoi(hex, 0, 16);
	return 0;
}

int SipCommon::HexStr2Int(int & num, std::string hex)
{
	if (hex.size() <= 2)
	{
		return -1;
	}
	else
	{
		if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
		{
			for (size_t i = 2; i < hex.size(); i++)
			{
				if (hex[i] < 48 || hex[i] > 122)
				{
					return -1;
				}
				else if (hex[i] > 58 && hex[i] < 65)
				{
					return -1;
				}
				else if (hex[i] > 91 && hex[i] < 97)
				{
					return -1;
				}
			}
			num = stoi(hex, 0, 16);
		}
	}
	return 0;
}

int SipCommon::GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8> &bitset)
{
	std::string hex = cmd.substr((2 * pos - 2), 2);
	return HexToBitset(bitset, hex);
}
