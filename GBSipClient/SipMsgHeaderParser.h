#pragma once

#include <iostream>
#include <string>
#include "osip2/osip.h"

#define osip_message_set_event(sip,value)	 osip_message_set_header((osip_message_t *)sip,(const char *)"Event",value)
#define osip_message_get_event(sip,pos,dest) osip_message_header_get_byname(( osip_message_t *)sip,(const char *)"event",pos,(osip_header_t **)dest)

#define osip_message_set_Subscription_State(sip,value)	 osip_message_set_header((osip_message_t *)sip,(const char *)"Subscription-State",value)
#define osip_message_get_Subscription_State(sip,pos,dest) osip_message_header_get_byname(( osip_message_t *)sip,(const char *)"subscription-state",pos,(osip_header_t **)dest)

class SipMsgHeaderParser
{
public:
	SipMsgHeaderParser();
	virtual ~SipMsgHeaderParser();

	// Uri◊÷∂Œ Ùœ‡…Ë÷√
	int SetUriScheme(osip_uri_t* uri, const char* scheme = "sip");
	int SetUriUsername(osip_uri_t* uri, const char* username);
	int SetUriHost(osip_uri_t* uri, const char* host);
	int SetUriPort(osip_uri_t* uri, const char* port);
	int SetUri(osip_uri_t** uri,
		const char* username,
		const char* host,
		const char* port,
		const char* scheme = "sip");

	// SIP request only
	int SetSipMsgVersion(osip_message_t* sipMsg, const char* version = "SIP/2.0");
	int SetSipMsgReqUri(osip_message_t* sipMsg,
		const char* sipID,
		const char* host,
		const char* port);
	int SetSipMsgMethod(osip_message_t *sipMsg, const char* method);	// …Ë÷√sipMegµƒmethod◊÷∂Œ

	// SIP answer only
	int SetSipMsgStatusCode(osip_message_t* sipMsg, int statusCode);
	int SetSipMsgReasonPhrase(osip_message_t* sipMsg, const char* reasonPhrase);
	
	// via
	int SetSipMsgVia(osip_message_t* sipMsg,
		const char* host,
		const char* port,
		const char* branchValue,
		const char* protocol = "UDP",
		const char* version = "2.0");
	int SetSipMsgVia(osip_message_t* sipMsg, const osip_via_t * via);
	int CloneSipMsgVias(osip_message_t* sipMsg, const osip_list_t* vias);
	int SetSipMsgViaProtocal(osip_message_t* sipMsg, const char* protocol = "TCP");
	int SetSipMsgViaBranch(osip_message_t* sipMsg, const char* branchValue);

	// tag
	int SetTag(osip_list_t* gen_params, const char* tag);
	int GetTag(osip_list_t* gen_params, std::string &tag);
	int GetSipMsgFromTag(const osip_message_t* sipMsg, std::string &tag);
	int GetSipMsgToTag(const osip_message_t* sipMsg, std::string &tag);

	// from
	int SetSipMsgFrom(osip_message_t* sipMsg,
		const char* username,
		const char* host, // domain
		const char* port = nullptr,
		const char* tag = nullptr);
	int SetSipMsgFrom(osip_message_t* sipMsg, const osip_from_t* from);
	int SetSipMsgFromTag(osip_message_t* sipMsg, const char* tag);

	// to
	int SetSipMsgTo(osip_message_t* sipMsg,
		const char* username,
		const char* host, // domain
		const char* port = nullptr,
		const char* tag = nullptr);
	int SetSipMsgTo(osip_message_t* sipMsg, const osip_to_t* to);
	int SetSipMsgToTag(osip_message_t* sipMsg, const char* tag);

	// callID
	int SetSipMsgCallID(osip_message_t* sipMsg, const osip_call_id_t *call_id);
	int SetSipMsgCallID(osip_message_t* sipMsg, const char* host, const char* number);
	int SetSipMsgCallID(osip_message_t* sipMsg, const char* callID);
	int GetSipMsgCallID(const osip_message_t* sipMsg, std::string &callID);

	// contact
	int SetSipMsgContact(osip_message_t* sipMsg,
		const char* username,
		const char* host,
		const char* port);
	int SetSipMsgContact(osip_message_t* sipMsg, const osip_contact_t* contact);

	// Cseq
	int SetSipMsgCseq(osip_message_t* sipMsg, const char* num, const char* method);
	int SetSipMsgCseq(osip_message_t* sipMsg, const osip_cseq_t *cseq);

	/* Header */
	// Max-Forwards
	int SetSipMsgMaxForward(osip_message_t* sipMsg, const char* maxForward = "70");
	int GetSipMsgMaxForward(const osip_message_t* sipMsg, int &maxForward);

	// Expires
	int SetSipMsgExpires(osip_message_t* sipMsg, const char* expires);
	int GetSipMsgExpires(const osip_message_t* sipMsg, int & expires);

	// Event

	// Content-Type
	int SetSipMsgContentType(osip_message_t* sipMsg, const char* contentType);
	int SetSipMsgContentType(osip_message_t* sipMsg,
		const char* type,
		const char* subType);

	// User-Agent
	int SetSipMsgUserAgent(osip_message_t* sipMsg, const char* user = "GB_Client");

	// subject
	int SetSipMsgSubject(osip_message_t* sipMsg,
		const char* senderSipID,
		const char* senderNum,
		const char* receiverSipID,
		const char* receiverNum);

	// Date
	int SetSipMsgDate(osip_message_t* sipMsg, const char* date);

	// www-authenticate
	int SetSipMsgWWW_Authrnticate(osip_message_t* sipMsg,
		const char* realm,
		const char* nonce,
		const char* type = "Digest");

	// Authorization
	int SetSipMsgAuthorization(osip_message_t* sipMsg,
		const osip_list_t www_authenticates,
		const char* password);

private:
	typedef int(*clone_func)(void *, void **);
};

