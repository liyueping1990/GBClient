#pragma once
#include <mutex>
#include "SipDefine.h"
#include "SipCommon.h"

class SipMsgBodyParser
{
public:
	SipMsgBodyParser();
	virtual ~SipMsgBodyParser();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString) = 0;
	virtual int ParserSipMsgBody(const char* bodyString, void* bodyParameter) = 0;
};

