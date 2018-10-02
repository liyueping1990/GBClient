#include "SipUtilities.h"


bool SipUtilities::CompareStrCase(const char * src, const char * dst)
{
	bool isEquel = false;

#if 0
#ifdef WIN32
	if (_strcmpi(src, dst) == 0)
#else
	if (strcasecmp(src, dst) == 0)
#endif
	{
		isEquel = true;
	}

#else
	if (src != nullptr && dst != nullptr)
	{
		int c1, c2;
		do {
			c1 = tolower(*src++);
			c2 = tolower(*dst++);
		} while (c1 == c2 && c1 != 0);
		isEquel = !(c1 - c2);
	}
	else
	{
		if (src == dst)
		{
			isEquel = true;
		}
	}
#endif
	return isEquel;
}


bool SipUtilities::IsPlatform(const std::string deviceID)
{
	bool isPlatform = false;
	if (deviceID.size() == 20)
	{
		int type = 0;
		SipUtilities::Str2Int(deviceID.substr(10, 3), type);
		if (type == 200 || type >= 500)
		{
			isPlatform = true;
		}
	}
	return isPlatform;
}


bool SipUtilities::WordCanBeFind(const std::string srcStr, const std::string word)
{
	bool beFind = false;

	int pos = -1;
	pos = srcStr.find(word);
	if (pos >= 0)
	{
		beFind = true;
	}
	return beFind;
}

int SipUtilities::Str2Int(std::string str, int & num)
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
	//return Str2Num(str, num);
}

int SipUtilities::Str2Double(std::string str, double & num)
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
	//Str2Num(str, num);
}

template<typename T>
int SipUtilities::Str2Num(const std::string str, T& num)
{
	int result = -1;
	std::istringstream ss(str);
	if (ss >> num)
	{
		result = 0;
	}
	return result;
}

std::string SipUtilities::Int2HexStr(int val)
{
	char buff[16] = { 0 };
	sprintf(buff, "0x%04X", val);
	return buff;
}

std::string SipUtilities::CreatUuid()
{
	char dstString[64] = { 0 };
#ifdef WIN32
	GUID uuid;
	::CoCreateGuid(&uuid);
	_snprintf_s(dstString, 64 - 1, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.Data1, uuid.Data2, uuid.Data3, uuid.Data4[0], uuid.Data4[1], uuid.Data4[2],
		uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);

	return std::string(dstString);
#else
	// 	// 1
	// 	//TmUUID_t    uuid;
	// 	//TmMsg_CreateUUID(&uuid);
	// 
	// 	// 2
	// // 	uuid_t uu;
	// // 	uuid_generate(uu);
	std::string str = "";
	str = RandLengthStr(8);
	str = str + "\-";
	str = str + RandLengthStr(4);
	str = str + "\-";
	str = str + RandLengthStr(4);
	str = str + "\-";
	str = str + RandLengthStr(4);
	str = str + "\-";
	str = str + RandLengthStr(8);
	return str;
#endif
}

std::string SipUtilities::RandLengthStr(const int length)
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

std::string SipUtilities::BitsetToHex(std::bitset<8> bite)
{
	char a[8] = { 0 };
	sprintf(a, "%02X", bite);
	return a;
}

int SipUtilities::HexToBitset(std::bitset<8>& bitset, std::string hex)
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

int SipUtilities::HexStr2Int(int & num, std::string hex)
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

int SipUtilities::GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8>& bitset)
{
	std::string hex = cmd.substr((2 * pos - 2), 2);
	return HexToBitset(bitset, hex);
}

int SipUtilities::GBTime2NptTime(std::string gbTime, std::string & nptTime)
{
	int result = -1;
	int time = 0;
	GBTime2NptTime(gbTime, time);
	nptTime = std::to_string(time);
	return result;
}

int SipUtilities::NptTime2GBTime(std::string nptTime, std::string & gbTime)
{
	int result = -1;

	time_t rawTime = 0;
	result = SipUtilities::Str2Num(nptTime, rawTime);
	if (result == 0)
	{
		tm* t=localtime(&rawTime);

		char tmp[64] = { 0 };
		strftime(tmp, sizeof(tmp), "%Y-%m-%dT%X", t);
		gbTime = tmp;
	}
	return result;
}


int SipUtilities::GBTime2NptTime(std::string gbTime, int & nptTime)
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

std::string SipUtilities::GetLocalTime()
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

int32_t SipUtilities::ParseKeyVal(const char* str, int32_t size, char delimiter, std::string& key, std::string& val)
{
	key.clear();
	val.clear();
	int32_t result = -1;
	int32_t idx;
	for (idx = 0; idx < size; idx++)
	{
		if (str[idx] == delimiter)
		{
			break;
		}
		else
		{
			if (str[idx] != ' ' && str[idx] != '\t')
			{
				key = key + str[idx];
			}
		}
	}
	bool hasQuotation = false;
	for (idx = idx + 1; idx < size; idx++)
	{
		if (str[idx] == '\"')
		{
			if (hasQuotation)
			{
				break;
			}
			else
			{
				hasQuotation = true;
			}
		}
		else
		{
			if (hasQuotation)
			{
				val = val + str[idx];
			}
			else
			{
				if (str[idx] != ' ' && str[idx] != '\t')
				{
					val = val + str[idx];
				}
			}
		}
	}

	return 0;
}

int32_t SipUtilities::GetKeyValMap(std::string str, std::map<std::string, std::string>& keyValMap)
{
	std::string param;
	std::string nextparam;
	std::vector<std::string> params;
	while (!str.empty())
	{
		ParseKeyVal(str.c_str(), str.length(), '&', param, nextparam);
		params.push_back(param);
		str = std::move(nextparam);
	}

	std::string key;
	std::string value;
	for (auto& var : params)
	{
		ParseKeyVal(var.c_str(), var.length(), '=', key, value);
		keyValMap[key] = value;
	}
	return 0;
}

std::string SipUtilities::GetValByKey(const std::map<std::string, std::string>& keyValMap, const char * key)
{
	std::string value = "";

	for (auto & var : keyValMap)
	{
		if (SipUtilities::CompareStrCase(var.first.c_str(), key))
		{
			value = var.second;
			break;
		}
	}
	return value;
}

