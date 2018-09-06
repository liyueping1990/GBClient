#pragma once
#include "DBServer.h"
#include "SipMsgHeaderParser.h"
#include "SipXmlBodyParser.h"
#include "SipSdpBodyParser.h"
#include "SipRtspBodyParser.h"

typedef class SipMsgObj
{
public:
	SipMsgObj();
	virtual ~SipMsgObj();

	int GetLocalPlatformInfo();
	int GetRemotePlatformInfo(std::string& remoteDeviceID);

	// Register
	int CreateRegister401SipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg, std::string nonce, std::string toTag = ""); // 401
	int CreateRegister200SipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 200

	// Invite, sdp
	int CreateInviteSipMsg(osip_message_t* &dstSipMsg, SdpParam* sdpParam); // ת��Invite
	int CreateInviteSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // ֱ��Invite��ֱ�Ӹ�����Ϣ��
	int CreateAckSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// Ack

	// Message
	int CreateSipMsgXml(osip_message_t* &dstSipMsg, std::string remoteDeviceID);

	// Response
	// 1xx,2xx,3xx,4xx
	int CreateXxxSipMsg(const osip_message_t* srcSipMsg, osip_message_t*& dstSipMsg, int statusCode, std::string toTag = "");

	// Parser
	int ParserSipMsg(const osip_message_t* sipMsg);
	XmlParam* GetXmlParam(const osip_message_t* sipMsg);
	SdpParam* GetSdpParam(const osip_message_t* sipMsg);
private:
	// ƽ̨������Ϣ
	int mLocalPlatformID;
	std::string mLocalSipID;		// ����ƽ̨28181�������
	std::string mLocalSipDomain;	// ����ƽ̨���������
	std::string mLocalIP;			// ����ƽ̨sip����IP��ַ
	int mLocalPort;			        // ����ƽ̨sip����˿ں�

	int mRemotePlatformID;
	std::string mRemoteSipID;		// ����ƽ̨28181�������
	std::string mRemoteSipDomain;	// ����ƽ̨���������
	std::string mRemoteIP;			// ����ƽ̨sip����IP��ַ
	int mRemotePort;		        // ����ƽ̨sip����˿ں�


	XmlParam* mXmlParam;
	SdpParam* mSdpParam;
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

	//int CreateNewSipMsg_ICT(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	//int CreateNewSipMsg_IST(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	//int CreateNewSipMsg_NICT(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	//int CreateNewSipMsg_NIST(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// Clone SipMsgBody
	int CloneSipMsgBody(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int AppendBodyForSipMsg(osip_message_t* &dstSipMsg, const char* bodyString);
	int ClearSipMsgBody(osip_message_t* &dstSipMsg);

}SipMsgBuilder, SipMsgParser;;

