#include "SipMsgObj.h"

std::shared_ptr<PlatformInfo> SipMsgObj::mLocalPlatformInfo;

SipMsgObj::SipMsgObj()
{
	mSipMsgHeader = new SipMsgHeaderParser;
	mSipMsgBody = nullptr;

	mXmlParam = nullptr;
	mSdpParam = nullptr;
}


SipMsgObj::~SipMsgObj()
{
	delete mSipMsgHeader;
	mSipMsgHeader = nullptr;

	if (mSipMsgBody)
	{
		delete mSipMsgBody;
		mSipMsgBody = nullptr;
	}

}

int SipMsgObj::SetPlatformInfo(std::weak_ptr<PlatformInfo> platformInfo, bool isLocal)
{
	int result = -1;
	if (auto info = platformInfo.lock())
	{
		if (isLocal)
		{
			mLocalPlatformInfo = info;
		}
		else
		{
			mRemotePlatformInfo = info;
		}
		result = 0;
	}
	return result;
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
			sprintf(realm, "\"%s\"", mLocalPlatformInfo->domain.c_str());
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

		// 耶紗toTag
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
	if (srcSipMsg)
	{
	}
	else
	{
		osip_message_init(&dstSipMsg);
		if (dstSipMsg == nullptr)
		{
			return -1;
		}

		// start-line
		SetSipMsgRequestStartLine(dstSipMsg, "INVITE");

		// via、from、to、call-id、CSeq、Contact、max-forward
		CreateNewSipMsg(nullptr, dstSipMsg, ICT);

		// Content_Type
		mSipMsgHeader->SetSipMsgContentType(dstSipMsg, "APPLICATION/SDP");

		// BODY
		mSipMsgBody = new SipSdpBodyParser;
		mSipMsgBody->CreateSipMsgBody(mSdpParam.get(), mSdpParam->bodyString);
		AppendBodyForSipMsg(dstSipMsg, mSdpParam->bodyString.c_str());
		delete mSipMsgBody;
		mSipMsgBody = nullptr;
	}

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

int SipMsgObj::CreateSipMsgXml(osip_message_t* &dstSipMsg)
{
	int result = -1;

	if (dstSipMsg)
	{
		osip_message_free(dstSipMsg);
		dstSipMsg = nullptr;
	}

	result = osip_message_init(&dstSipMsg);
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

	// Body
	mSipMsgBody = new SipXmlBodyParser;
	mSipMsgBody->CreateSipMsgBody(mXmlParam.get(), mXmlParam->bodyString);
	AppendBodyForSipMsg(dstSipMsg, mXmlParam->bodyString.c_str());
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

std::shared_ptr<XmlParam> SipMsgObj::GetXmlParam(const osip_message_t * sipMsg)
{
	if (sipMsg != nullptr)
	{
		mSipMsgBody = new SipXmlBodyParser;
		mXmlParam = std::make_shared<XmlParam>();
		osip_body_t* body = nullptr;
		body = (osip_body_t*)osip_list_get(&sipMsg->bodies, 0);
		mSipMsgBody->ParserSipMsgBody(body->body, mXmlParam.get());
	}
	return mXmlParam;
}

int SipMsgObj::SetXmlParam(std::weak_ptr<XmlParam> xmlParam)
{
	int result = -1;
	if (auto param = xmlParam.lock())
	{
		mXmlParam = param;
		result = 0;
	}
	return result;
}

std::shared_ptr<SdpParam> SipMsgObj::GetSdpParam(const osip_message_t * sipMsg)
{
	return nullptr;
}

int SipMsgObj::SetSdpParam(std::weak_ptr<SdpParam> sdpParam)
{
	int result = -1;
	if (auto param = sdpParam.lock())
	{
		mSdpParam = param;
		result = 0;
	}
	return result;
}

int SipMsgObj::SetSipMsgRequestStartLine(osip_message_t *& dstSipMsg, const char * method)
{
	int result = 0;
	if (dstSipMsg != nullptr)
	{
		result = mSipMsgHeader->SetSipMsgVersion(dstSipMsg);
		result = result + mSipMsgHeader->SetSipMsgMethod(dstSipMsg, method);
		result = result + mSipMsgHeader->SetSipMsgReqUri(dstSipMsg, mRemotePlatformInfo->deviceID.c_str(), mRemotePlatformInfo->ip.c_str(), std::to_string(mRemotePlatformInfo->port).c_str());
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
	mSipMsgHeader->SetSipMsgVia(dstSipMsg, mLocalPlatformInfo->ip.c_str(), std::to_string(mLocalPlatformInfo->port).c_str(), SipCommon::CreatUuid().c_str());

	//mSipMsgHeader->SetSipMsgFrom(dstSipMsg, mLocalSipID, mLocalSipDomain, SipCommon::CreatUuid());
	mSipMsgHeader->SetSipMsgFrom(dstSipMsg, mLocalPlatformInfo->deviceID.c_str(), mLocalPlatformInfo->ip.c_str(), std::to_string(mLocalPlatformInfo->port).c_str(), SipCommon::CreatUuid().c_str());

	//mSipMsgHeader->SetSipMsgTo(dstSipMsg, mRemoteSipID, mRemoteSipDomain);
	mSipMsgHeader->SetSipMsgTo(dstSipMsg, mRemotePlatformInfo->deviceID.c_str(), mRemotePlatformInfo->ip.c_str(), std::to_string(mRemotePlatformInfo->port).c_str());

	mSipMsgHeader->SetSipMsgCallID(dstSipMsg, mLocalPlatformInfo->ip.c_str(), SipCommon::CreatUuid().c_str());
	mSipMsgHeader->SetSipMsgCseq(dstSipMsg, "1", dstSipMsg->sip_method);
	mSipMsgHeader->SetSipMsgContact(dstSipMsg, mLocalPlatformInfo->deviceID.c_str(), mLocalPlatformInfo->ip.c_str(), std::to_string(mLocalPlatformInfo->port).c_str());
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
