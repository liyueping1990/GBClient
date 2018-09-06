#include "SipMsgObj.h"



SipMsgObj::SipMsgObj()
{
	mSipMsgHeader = new SipMsgHeaderParser;
	mSipMsgBody = nullptr;
	mXmlParam = nullptr;
	mSdpParam = nullptr;

	mLocalPlatformID = 1;
	mLocalSipID = "34020000002000000001";		// 本级平台28181国标编码
	mLocalSipDomain = "3402000000";		// 本级平台所在域编码
	mLocalIP = "192.168.1.110";		// 本级平台sip信令IP地址
	mLocalPort = 8060;			    // 本级平台sip信令端口号

	mRemotePlatformID = 2;
	mRemoteSipID = "34020000001320000001";			// 级联平台28181国标编码
	mRemoteSipDomain = "123";		// 级联平台所在域编码
	mRemoteIP = "192.168.1.108";		// 级联平台sip信令IP地址
	mRemotePort = 5060;		        // 级联平台sip信令端口号
}


SipMsgObj::~SipMsgObj()
{
	if (mSipMsgBody)
	{
		delete mSipMsgBody;
	}
	if (mXmlParam)
	{
		delete mXmlParam;
	}

	delete mSipMsgHeader;
}

int SipMsgObj::GetLocalPlatformInfo()
{
	return 0;
}

int SipMsgObj::GetRemotePlatformInfo(std::string& remoteDeviceID)
{
	return 0;
}

int SipMsgObj::CreateRegister401SipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg, std::string nonce, std::string toTag)
{
	int result = -1;
	result = CreateXxxSipMsg(srcSipMsg, dstSipMsg, 401, toTag);
	if (result == 0)
	{
		if (!nonce.empty())// www-Authenticate
		{
			char realm[64] = { 0 };
			sprintf(realm, "\"%s\"", mLocalSipDomain.c_str());
			char authNonce[64] = { 0 };
			sprintf(authNonce, "\"%s\"", nonce.c_str());
			mSipMsgHeader->SetSipMsgWWW_Authrnticate(dstSipMsg, realm, authNonce);
		}
		else
		{
			result = -1;
		}
	}
	return result;
}

int SipMsgObj::CreateRegister200SipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg)
{
	int result = -1;
	result = CreateXxxSipMsg(srcSipMsg, dstSipMsg, 200);
	if (result == 0)
	{
		result = mSipMsgHeader->SetSipMsgDate(dstSipMsg, SipCommon::GetLocalTimeInMS().c_str());

		// 添加toTag
		mSipMsgHeader->SetSipMsgToTag(dstSipMsg, SipCommon::CreatUuid().c_str());
	}
	return result;
}

int SipMsgObj::CreateInviteSipMsg(osip_message_t *& dstSipMsg, SdpParam* sdpParam)
{
	osip_message_init(&dstSipMsg);
	if (dstSipMsg == nullptr)
	{
		return -1;
	}

	// 获取本平台相关信息
	//GetLocalPlatformInfo();

	// 获取对方平台相关信息
	int result = GetRemotePlatformInfo(sdpParam->deviceID);
	if (result == -1)
	{
		osip_message_free(dstSipMsg);
		return result;
	}

	// start-line
	SetSipMsgRequestStartLine(dstSipMsg, "INVITE");

	// via、from、to、call-id、CSeq、Contact、max-forward
	CreateNewSipMsg(nullptr, dstSipMsg, ICT);

	// Content_Type
	mSipMsgHeader->SetSipMsgContentType(dstSipMsg, "APPLICATION/SDP");

	// BODY
	mSipMsgBody = new SipSdpBodyParser;
	mSipMsgBody->CreateSipMsgBody(sdpParam, sdpParam->bodyString);
	AppendBodyForSipMsg(dstSipMsg, sdpParam->bodyString.c_str());
	delete mSipMsgBody;
	mSipMsgBody = nullptr;
	return 0;
}

int SipMsgObj::CreateInviteSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{	
	return 0;
}

int SipMsgObj::CreateAckSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	osip_message_clone(srcSipMsg, &dstSipMsg);

	// set start line
	SetSipMsgRequestStartLine(dstSipMsg, "ACK");
	mSipMsgHeader->SetSipMsgCseq(dstSipMsg, "1", "ACK");

	// clear body
	ClearSipMsgBody(dstSipMsg);

	return 0;
}

int SipMsgObj::CreateSipMsgXml(osip_message_t* &dstSipMsg, std::string remoteDeviceID)
{
	int result = -1;

	if (dstSipMsg)
	{
		osip_message_free(dstSipMsg);
		dstSipMsg = nullptr;
	}

	osip_message_init(&dstSipMsg);

	// 获取本平台相关信息
	//GetLocalPlatformInfo();

	// 获取对方平台相关信息
	result = GetRemotePlatformInfo(remoteDeviceID);
	if (result == -1)
	{
		osip_message_free(dstSipMsg);
		dstSipMsg = nullptr;
		return result;
	}

	// start-line
	SetSipMsgRequestStartLine(dstSipMsg, "MESSAGE");

	// via、from、to、call-id、CSeq、Contact、max-forward
	CreateNewSipMsg(nullptr, dstSipMsg, NICT);

	// Content_Type
	mSipMsgHeader->SetSipMsgContentType(dstSipMsg, "Application/MANSCDP+xml");

	if (mXmlParam == nullptr)
	{
		mXmlParam =  new XmlParam;
	}

	// Body
	//mXmlParam.localSipID = mLocalSipID;
	//mXmlParam.remoteSipID = mRemoteSipID;
	//mXmlParam.deviceID = mRemoteSipID;
	//mXmlParam.manscdpType = QUERY_DEVICE_CATALOG;

	//mSipMsgBody = new SipXmlBodyParser;
	//mSipMsgBody->CreateSipMsgBody(&mXmlParam, mXmlParam.bodyString);
	//AppendBodyForSipMsg(dstSipMsg, mXmlParam.bodyString.c_str());
	delete mSipMsgBody;
	mSipMsgBody = nullptr;
	return 0;
}

int SipMsgObj::CreateXxxSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg, int statusCode, std::string toTag)
{
	int result = -1;
	result = CreateBaseResponseSipMsg(srcSipMsg, dstSipMsg);

	if (toTag.empty())
	{
		mSipMsgHeader->SetSipMsgToTag(dstSipMsg, SipCommon::CreatUuid().c_str());
	}
	else
	{
		mSipMsgHeader->SetSipMsgToTag(dstSipMsg, toTag.c_str());
	}

	if (result == 0)
	{
		switch (statusCode)
		{
		case 100:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Trying");
			break;
		case 200:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "OK");
			break;
		case 400:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Bad Request");
			break;
		case 401:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Unauthorized");
			break;
		case 403:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Forbidden");
			break;
		case 404:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Not Found");
			break;
		case 486:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Busy Here");
			break;
		case 488:
			result = SetSipMsgResponseStartLine(dstSipMsg, statusCode, "Not Acceptable Here");
			break;
		default:
			result = SetSipMsgResponseStartLine(dstSipMsg, 500, "Camera Unavailable");
			break;
		}
	}
	return result;
}

int SipMsgObj::ParserSipMsg(const osip_message_t * sipMsg)
{
	return 0;
}

XmlParam* SipMsgObj::GetXmlParam(const osip_message_t * sipMsg)
{
	if (sipMsg != nullptr)
	{
		mSipMsgBody = new SipXmlBodyParser;
		mXmlParam = new XmlParam;
		osip_body_t* body = nullptr;
		body = (osip_body_t*)osip_list_get(&sipMsg->bodies, 0);
		mSipMsgBody->ParserSipMsgBody(body->body, mXmlParam);
	}
	return mXmlParam;
}

int SipMsgObj::SetSipMsgRequestStartLine(osip_message_t *& dstSipMsg, const char * method)
{
	int result = 0;
	if (dstSipMsg != nullptr)
	{
		result = mSipMsgHeader->SetSipMsgVersion(dstSipMsg);
		result = result + mSipMsgHeader->SetSipMsgMethod(dstSipMsg, method);
		result = result + mSipMsgHeader->SetSipMsgReqUri(dstSipMsg, mRemoteSipID.c_str(), mRemoteIP.c_str(), std::to_string(mRemotePort).c_str());
	}
	return result;
}

int SipMsgObj::FreeSipMsgRequestStartLine(osip_message_t *& dstSipMsg)
{
	osip_free(dstSipMsg->sip_version);
	dstSipMsg->sip_version = nullptr;
	osip_free(dstSipMsg->sip_method);
	dstSipMsg->sip_method = nullptr;
	osip_uri_free(dstSipMsg->req_uri);
	dstSipMsg->req_uri = nullptr;
	return 0;
}

int SipMsgObj::SetSipMsgResponseStartLine(osip_message_t *& dstSipMsg, int statusCode, const char * reasonPhrase)
{
	int result = -1;
	if (dstSipMsg != nullptr)
	{
		mSipMsgHeader->SetSipMsgStatusCode(dstSipMsg, statusCode);
		result = mSipMsgHeader->SetSipMsgReasonPhrase(dstSipMsg, reasonPhrase);
	}
	return result;
}

int SipMsgObj::CreateBaseResponseSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	if (srcSipMsg != nullptr)
	{
		if (dstSipMsg)
		{
			osip_message_free(dstSipMsg);
			dstSipMsg = nullptr;
		}
		result = osip_message_clone(srcSipMsg, &dstSipMsg);

		if (result == 0)
		{
			FreeSipMsgRequestStartLine(dstSipMsg);
			ClearSipMsgBody(dstSipMsg);
			mSipMsgHeader->SetSipMsgUserAgent(dstSipMsg);
		}
	}
	return result;
}

int SipMsgObj::CreateNewSipMsg(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg, osip_fsm_type_t msgType)
{
	int result = -1;
	switch (msgType)
	{
	case ICT:
	case NICT:
		result = CreateNewSipMsg_C(srcSipMsg, dstSipMsg);
		break;
	case IST:
	case NIST:
		result = CreateNewSipMsg_S(srcSipMsg, dstSipMsg);
		break;
	default:
		break;
	}
	return result;
}

int SipMsgObj::CreateNewSipMsg_C(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	mSipMsgHeader->SetSipMsgVia(dstSipMsg, mLocalIP.c_str(), std::to_string(mLocalPort).c_str(), SipCommon::CreatUuid().c_str());

	//mSipMsgHeader->SetSipMsgFrom(dstSipMsg, mLocalSipID, mLocalSipDomain, SipCommon::CreatUuid());
	mSipMsgHeader->SetSipMsgFrom(dstSipMsg, mLocalSipID.c_str(), mLocalIP.c_str(), std::to_string(mLocalPort).c_str(), SipCommon::CreatUuid().c_str());

	//mSipMsgHeader->SetSipMsgTo(dstSipMsg, mRemoteSipID, mRemoteSipDomain);
	mSipMsgHeader->SetSipMsgTo(dstSipMsg, mRemoteSipID.c_str(), mRemoteIP.c_str(), std::to_string(mRemotePort).c_str());

	mSipMsgHeader->SetSipMsgCallID(dstSipMsg, mLocalIP.c_str(), SipCommon::CreatUuid().c_str());
	mSipMsgHeader->SetSipMsgCseq(dstSipMsg, "1", dstSipMsg->sip_method);
	mSipMsgHeader->SetSipMsgContact(dstSipMsg, mLocalSipID.c_str(), mLocalIP.c_str(), std::to_string(mLocalPort).c_str());
	mSipMsgHeader->SetSipMsgMaxForward(dstSipMsg);
	// User-Agent
	mSipMsgHeader->SetSipMsgUserAgent(dstSipMsg);
	return 0;
}

int SipMsgObj::CreateNewSipMsg_S(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	// User-Agent
	mSipMsgHeader->SetSipMsgUserAgent(dstSipMsg);
	return 0;
}


int SipMsgObj::CloneSipMsgBody(const osip_message_t * srcSipMsg, osip_message_t *& dstSipMsg)
{
	int result = -1;
	osip_body* srcBody = (osip_body*)osip_list_get(&srcSipMsg->bodies, 0);

	osip_body_t* dstBody = nullptr;
	result = osip_body_clone(srcBody, &dstBody);
	if (!result)
	{
		result = osip_list_add(&dstSipMsg->bodies, dstBody, 0);
	}
	return result;
}

int SipMsgObj::AppendBodyForSipMsg(osip_message_t *& dstSipMsg, const char * bodyString)
{
	int reslut = -1;

	osip_body_t* body;
	osip_body_init(&body);
	osip_body_parse(body, bodyString, strlen(bodyString));
	if (body != nullptr)
	{
		reslut = osip_list_add(&dstSipMsg->bodies, body, -1);
	}
	return reslut;
}

int SipMsgObj::ClearSipMsgBody(osip_message_t *& dstSipMsg)
{
	if (dstSipMsg && dstSipMsg->bodies.node > 0)
	{
		osip_list_special_free(&dstSipMsg->bodies, (void(*)(void *))&osip_body_free);
	}
	return 0;
}
