#pragma once
#include "SipMsgHeaderParser.h"
#include "SipXmlBodyParser.h"
#include "SipSdpBodyParser.h"
#include "SipRtspBodyParser.h"

typedef class SipMsgObj
{
public:
	SipMsgObj();
	virtual ~SipMsgObj();
	static std::shared_ptr<PlatformInfo> mLocalPlatformInfo;
	int SetPlatformInfo(std::weak_ptr<PlatformInfo> platformInfo, bool isLocal = false);

	// Register
	int CreateRegister401SipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg, std::string nonce, std::string toTag = ""); // 401
	int CreateRegister200SipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 200

	// Invite, sdp
	int CreateInviteSipMsg(osip_message_t* &dstSipMsg, SdpParam* sdpParam); // 转发Invite
	int CreateInviteSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 直连Invite，直接复制消息体
	int CreateAckSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// Ack

	// Message
	int CreateSipMsgXml(osip_message_t* &dstSipMsg, std::string remoteDeviceID);
	int CreateSipMsgXml(osip_message_t* &dstSipMsg);

	// Response
	// 1xx,2xx,3xx,4xx
	int CreateXxxSipMsg(const osip_message_t* srcSipMsg, osip_message_t*& dstSipMsg, int statusCode, std::string toTag = "");

	// Parser
	int ParserSipMsg(const osip_message_t* sipMsg);

	std::shared_ptr<XmlParam> GetXmlParam(const osip_message_t* sipMsg);
	int SetXmlParam(std::weak_ptr<XmlParam> xmlParam);

	std::shared_ptr<SdpParam> GetSdpParam(const osip_message_t* sipMsg);
	int SetSdpParam(std::weak_ptr<SdpParam> sdpParam);

private:
	// 平台参数信息
	//int mLocalPlatformID;
	//std::string mLocalSipID;		// 本级平台28181国标编码
	//std::string mLocalSipDomain;	// 本级平台所在域编码
	//std::string mLocalIP;			// 本级平台sip信令IP地址
	//int mLocalPort;			        // 本级平台sip信令端口号

	//int mRemotePlatformID;
	//std::string mRemoteSipID;		// 级联平台28181国标编码
	//std::string mRemoteSipDomain;	// 级联平台所在域编码
	//std::string mRemoteIP;			// 级联平台sip信令IP地址
	//int mRemotePort;		        // 级联平台sip信令端口号

	std::shared_ptr<PlatformInfo> mRemotePlatformInfo;

	std::shared_ptr<XmlParam> mXmlParam;
	std::shared_ptr<SdpParam> mSdpParam;

	//XmlParam* mXmlParam;
	//SdpParam* mSdpParam;
	//RtspParam* mRtspParam;
	SipMsgBodyParser*   mSipMsgBody;
	SipMsgHeaderParser* mSipMsgHeader;

	// Set Start-Line
	int SetSipMsgRequestStartLine(osip_message_t* &dstSipMsg, const char* method);
	int FreeSipMsgRequestStartLine(osip_message_t* &dstSipMsg);
	int SetSipMsgResponseStartLine(osip_message_t* &dstSipMsg, int statusCode, const char* reasonPhrase);
	int CreateBaseResponseSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// CreateNewSipMsg
	int CreateNewSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg, osip_fsm_type_t msgType);
	int CreateNewSipMsg_C(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_S(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// Clone SipMsgBody
	int CloneSipMsgBody(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int AppendBodyForSipMsg(osip_message_t* &dstSipMsg, const char* bodyString);
	int ClearSipMsgBody(osip_message_t* &dstSipMsg);

}SipMsgBuilder, SipMsgParser;;

