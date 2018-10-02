#include "SipMsgHeaderParser.h"



SipMsgHeaderParser::SipMsgHeaderParser()
{
}


SipMsgHeaderParser::~SipMsgHeaderParser()
{
}

int SipMsgHeaderParser::SetUriScheme(osip_uri_t * uri, const char * scheme)
{
	int result = -1;

	if (scheme != nullptr)
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->scheme != nullptr)
		{
			osip_free(uri->scheme);
			uri->scheme = nullptr;
		}
		uri->scheme = osip_strdup(scheme);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetUriUsername(osip_uri_t * uri, const char * username)
{
	int result = -1;

	if (username != nullptr)
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->username != nullptr)
		{
			osip_free(uri->username);
			uri->username = nullptr;
		}
		uri->username = osip_strdup(username);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetUriHost(osip_uri_t * uri, const char * host)
{
	int result = -1;

	if (host != nullptr)
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->host != nullptr)
		{
			osip_free(uri->host);
			uri->host = nullptr;
		}
		uri->host = osip_strdup(host);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetUriPort(osip_uri_t * uri, const char * port)
{
	int result = -1;

	if (port != nullptr)
	{
		if (uri == nullptr)
		{
			osip_uri_init(&uri);
		}

		if (uri->port != nullptr)
		{
			osip_free(uri->port);
			uri->port = nullptr;
		}
		uri->port = osip_strdup(port);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetUri(osip_uri_t ** uri, const char * username, const char * host, const char * port, const char * scheme)
{
	int result = 0;

	if (*uri == nullptr)
	{
		osip_uri_init(uri);
	}

	SetUriScheme(*uri, scheme);

	if (port != nullptr)
	{
		SetUriPort(*uri, port);
	}

	result += SetUriUsername(*uri, username);
	result += SetUriHost(*uri, host);
	return result;
}

int SipMsgHeaderParser::SetSipMsgVersion(osip_message_t * sipMsg, const char * version)
{
	int result = -1;
	if (sipMsg != nullptr && version != nullptr)
	{
		if (sipMsg->sip_version != nullptr)
		{
			osip_free(sipMsg->sip_version);
			sipMsg->sip_version = nullptr;
		}
		sipMsg->sip_version = osip_strdup(version);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgReqUri(osip_message_t * sipMsg, const char * sipID, const char * host, const char * port)
{
	return SetUri(&sipMsg->req_uri, sipID, host, port);
}

int SipMsgHeaderParser::SetSipMsgMethod(osip_message_t * sipMsg, const char * method)
{
	int result = -1;

	if (sipMsg != nullptr && method != nullptr)
	{
		if (sipMsg->sip_method)
		{
			osip_free(sipMsg->sip_method);
			sipMsg->sip_method = nullptr;
		}
		sipMsg->sip_method = osip_strdup(method);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgStatusCode(osip_message_t * sipMsg, int statusCode)
{
	int result = -1;
	if (statusCode > 0)
	{
		sipMsg->status_code = statusCode;
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgReasonPhrase(osip_message_t * sipMsg, const char * reasonPhrase)
{
	int result = -1;
	if (sipMsg != nullptr && reasonPhrase != nullptr)
	{
		if (sipMsg->reason_phrase != nullptr)
		{
			osip_free(sipMsg->reason_phrase);
			sipMsg->reason_phrase = nullptr;
		}

		sipMsg->reason_phrase = osip_strdup(reasonPhrase);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgVia(osip_message_t* sipMsg, const char* host, const char* port, const char* branchValue, const char* protocol /*= "UDP"*/, const char* version /*= "2.0"*/)
{
	int reslut = -1;
	if (sipMsg != nullptr && protocol!=nullptr && host != nullptr && port != nullptr)
	{
		osip_via_t* via = nullptr;
		osip_via_init(&via);

		via->version = osip_strdup(version);
		via->protocol = osip_strdup(protocol);
		via->host = osip_strdup(host);
		via->port = osip_strdup(port);

		// 设置branch
		osip_generic_param_t *viaBranch = nullptr;
		osip_generic_param_init(&viaBranch);
		std::string branch = "z9hG4bK-";
		branch = branch + branchValue;
		viaBranch->gname = osip_strdup("branch");
		viaBranch->gvalue = osip_strdup(branch.c_str());

		osip_list_special_free(&via->via_params, (void(*)(void*))osip_uri_param_freelist);
		osip_list_add(&via->via_params, viaBranch, 0);

		// 不可释放内存，添加到消息中
		osip_list_special_free(&sipMsg->vias, (void(*)(void *))&osip_via_free);
		osip_list_add(&sipMsg->vias, via, -1);

		reslut = 0;
	}
	return reslut;
}

int SipMsgHeaderParser::SetSipMsgVia(osip_message_t * sipMsg, const osip_via_t * via)
{
	int result = -1;
	if (sipMsg != nullptr && via != nullptr)
	{
		osip_via_t* sipVia = nullptr;
		result = osip_via_clone(via, &sipVia);
		if (!result)
		{
			osip_list_special_free(&sipMsg->vias, (void(*)(void *))&osip_via_free);
			osip_list_add(&sipMsg->vias, sipVia, -1);
		}
	}
	return result;
}

int SipMsgHeaderParser::CloneSipMsgVias(osip_message_t * sipMsg, const osip_list_t * vias)
{
	int result = -1;
	if (sipMsg != nullptr && vias != nullptr)
	{
		result = osip_list_clone(vias, &sipMsg->vias, (clone_func)osip_via_clone);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgViaProtocal(osip_message_t * sipMsg, const char * protocol)
{
	int result = -1;
	if (sipMsg != nullptr || sipMsg->vias.node != nullptr)
	{
		osip_via_t * sipVia = nullptr;

		//sipVia = (osip_via_t*)osip_list_get(&sipMsg->vias, -1);
		sipVia = (osip_via_t*)sipMsg->vias.node->element;
		if (sipVia->protocol != nullptr)
		{
			osip_free(sipVia->protocol);
			sipVia->protocol = nullptr;
		}
		sipVia->protocol = osip_strdup(protocol);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgViaBranch(osip_message_t * sipMsg, const char * branchValue)
{
	int result = -1;
	if (sipMsg != nullptr && branchValue != nullptr)
	{
		osip_generic_param_t *viaBranch = nullptr;
		osip_generic_param_init(&viaBranch);

		std::string branch = "z9hG4bK-";
		branch = branch + branchValue;
		viaBranch->gname = osip_strdup("branch");
		viaBranch->gvalue = osip_strdup(branch.c_str());

		osip_via_t* via = (osip_via_t*)osip_list_get(&sipMsg->vias, 0);

		osip_list_special_free(&via->via_params, (void(*)(void*))osip_uri_param_free);
		result = osip_list_add(&via->via_params, viaBranch, -1);
	}
	return result;
}

int SipMsgHeaderParser::SetTag(osip_list_t * gen_params, const char* tag)
{
	int result = -1;
	if (gen_params != nullptr && tag != nullptr)
	{		
		osip_generic_param_freelist(gen_params);// 必须释放已存在tag所占内存

		// 创建新的tag字段，并进行初始化
		osip_generic_param_t *sipTag = nullptr;
		osip_generic_param_init(&sipTag);

		// 设置新tag字面值
		sipTag->gname = osip_strdup("tag");
		sipTag->gvalue = osip_strdup(tag);

		// 将新tag添加到sipMsg->from->gen_params或sipMsg->to->gen_params
		result = osip_list_add(gen_params, sipTag, -1);
	}
	return 0;
}

int SipMsgHeaderParser::SetSipMsgFrom(osip_message_t* sipMsg, const char* username, const char* host, const char* port /*= ""*/, const char* tag /*= ""*/)
{
	int result = -1;
	if (sipMsg != nullptr)
	{
		if (sipMsg->from)
		{
			osip_from_free(sipMsg->from);
			sipMsg->from = nullptr;
		}

		osip_from_t * from = nullptr;
		osip_from_init(&from);
		result = SetUri(&from->url, username, host, port, nullptr);
		if (!result)
		{
			SetTag(&from->gen_params, tag);
			sipMsg->from = from;
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgFrom(osip_message_t * sipMsg, const osip_from_t * from)
{
	int result = -1;
	if (sipMsg != nullptr && from != nullptr)
	{
		if (sipMsg->from)
		{
			osip_from_free(sipMsg->from);
			sipMsg->from = nullptr;
		}
		result = osip_from_clone(from, &sipMsg->from);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgFromTag(osip_message_t * sipMsg, const char* tag)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->from != nullptr)
	{
		result = SetTag(&sipMsg->from->gen_params, tag);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgTo(osip_message_t * sipMsg, const char * username, const char * host, const char * port, const char * tag)
{
	int result = -1;
	if (sipMsg != nullptr)
	{
		if (sipMsg->to)
		{
			osip_to_free(sipMsg->to);
			sipMsg->to = nullptr;
		}

		osip_to_t * to = nullptr;
		osip_from_init(&to);
		result = SetUri(&to->url, username, host, port, nullptr);
		if (!result)
		{
			SetTag(&to->gen_params, tag);
			sipMsg->to = to;
		}
		else
		{
			osip_from_free(to);
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgTo(osip_message_t * sipMsg, const osip_to_t * to)
{
	int result = -1;
	if (sipMsg != nullptr && to != nullptr)
	{
		if (sipMsg->to)
		{
			osip_from_free(sipMsg->to);
			sipMsg->to = nullptr;
		}

		result = osip_to_clone(to, &sipMsg->to);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgToTag(osip_message_t * sipMsg, const char* tag)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->to != nullptr)
	{
		result = SetTag(&sipMsg->to->gen_params, tag);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgCallID(osip_message_t* sipMsg, const osip_call_id_t *call_id)
{
	int result = -1;
	if (sipMsg != nullptr && call_id != nullptr)
	{
		if (sipMsg->call_id)
		{
			osip_call_id_free(sipMsg->call_id);
			sipMsg->call_id = nullptr;
		}

		result = osip_call_id_clone(call_id, &sipMsg->call_id);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgCallID(osip_message_t * sipMsg, const char * host, const char * number)
{
	int result = -1;

	if (sipMsg != nullptr && number != nullptr && host != nullptr)
	{
		if (sipMsg->call_id)
		{
			osip_call_id_free(sipMsg->call_id);
			sipMsg->call_id = nullptr;
		}

		result = osip_call_id_init(&sipMsg->call_id);
		if (!result)
		{
			sipMsg->call_id->number = osip_strdup(number);
			sipMsg->call_id->host = osip_strdup(host);
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgCallID(osip_message_t * sipMsg, const char * callID)
{
	int result = -1;

	if (sipMsg != nullptr && callID != nullptr)
	{
		if (sipMsg->call_id)
		{
			osip_call_id_free(sipMsg->call_id);
			sipMsg->call_id = nullptr;
		}
		result = osip_message_set_call_id(sipMsg, callID);
	}
	return result;
}

int SipMsgHeaderParser::GetSipMsgCallID(const osip_message_t * sipMsg, std::string & callID)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->call_id != nullptr)
	{
		char* callId = nullptr;
		result = osip_call_id_to_str(sipMsg->call_id, &callId);
		if (callId)
		{
			callID = callId;
			osip_free(callId);
			callId = nullptr;
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgContact(osip_message_t * sipMsg, const char * username, const char * host, const char * port)
{
	int result = -1;
	if (sipMsg != nullptr)
	{
		osip_contact_t* contact = nullptr;
		osip_contact_init(&contact);
		result = SetUri(&contact->url, username, host, port);

		if (!result)
		{
			osip_list_special_free(&sipMsg->contacts, (void(*)(void *)) &osip_contact_free);
			result = osip_list_add(&sipMsg->contacts, contact, -1);
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgContact(osip_message_t * sipMsg, const osip_contact_t * contact)
{
	int result = -1;
	if (sipMsg != nullptr && contact != nullptr)
	{
		osip_contact_t* sipContact = nullptr;
		result = osip_contact_clone(contact, &sipContact);
		if (!result)
		{
			osip_list_special_free(&sipMsg->contacts, (void(*)(void *))&osip_contact_free);
			result = osip_list_add(&sipMsg->contacts, sipContact, 0);
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgCseq(osip_message_t * sipMsg, const char * num, const char * method)
{
	int result = -1;

	if (sipMsg != nullptr && num != nullptr && method != nullptr)
	{
		if (sipMsg->cseq)
		{
			osip_cseq_free(sipMsg->cseq);
			sipMsg->cseq = nullptr;
		}

		osip_cseq_t* cseq = nullptr;
		osip_cseq_init(&cseq);
		cseq->number = osip_strdup(num);
		cseq->method = osip_strdup(method);
		sipMsg->cseq = cseq;

		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgCseq(osip_message_t * sipMsg, const osip_cseq_t * cseq)
{
	int result = -1;
	if (sipMsg != nullptr && cseq != nullptr)
	{
		if (sipMsg->cseq)
		{
			osip_cseq_free(sipMsg->cseq);
			sipMsg->cseq = nullptr;
		}
		osip_cseq_clone(cseq, &sipMsg->cseq);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgMaxForward(osip_message_t * sipMsg, const char * maxForward)
{
	int result = -1;
	if (sipMsg != nullptr && maxForward != nullptr)
	{
		result = osip_message_set_max_forwards(sipMsg, maxForward);
	}
	return result;
}

int SipMsgHeaderParser::GetSipMsgMaxForward(const osip_message_t * sipMsg, int & maxForward)
{
	return 0;
}

int SipMsgHeaderParser::SetSipMsgExpires(osip_message_t * sipMsg, const char* expires)
{
	int result = -1;
	if (sipMsg != nullptr && expires !=nullptr)
	{
		result = osip_message_set_expires(sipMsg, expires);
	}
	return result;
}

int SipMsgHeaderParser::GetSipMsgExpires(const osip_message_t * sipMsg, int & expires)
{
	return 0;
}

int SipMsgHeaderParser::SetSipMsgContentType(osip_message_t * sipMsg, const char * contentType)
{
	int result = -1;
	if (sipMsg != nullptr && contentType != nullptr)
	{
		osip_content_type_t *content_type = nullptr;
		osip_content_type_init(&content_type);
		result = osip_content_type_parse(content_type, contentType);

		if (!result && content_type)
		{
			if (sipMsg->content_type)
			{
				osip_content_type_free(sipMsg->content_type);
				sipMsg->content_type = nullptr;
			}
			sipMsg->content_type = content_type;
			result = 0;
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgContentType(osip_message_t * sipMsg, const char * type, const char * subType)
{
	int result = -1;
	if (sipMsg != nullptr && type != nullptr && subType != nullptr)
	{
		osip_content_type_t *content_type = nullptr;
		osip_content_type_init(&content_type);
		content_type->type = osip_strdup(type);
		content_type->subtype = osip_strdup(subType);

		if (sipMsg->content_type)
		{
			osip_content_type_free(sipMsg->content_type);
			sipMsg->content_type = nullptr;
		}
		sipMsg->content_type = content_type;
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgUserAgent(osip_message_t * sipMsg, const char * user)
{
	int result = -1;
	if (sipMsg != nullptr && user != nullptr)
	{
		osip_header_t* usrAgent = nullptr;
		osip_message_get_user_agent(sipMsg, 0, &usrAgent);
		if (usrAgent)
		{
			osip_free(usrAgent->hvalue);
			usrAgent->hvalue = osip_strdup(user);
		}
		else
		{
			result = osip_message_set_user_agent(sipMsg, user);
		}
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgSubject(osip_message_t* sipMsg, const char* senderSipID, const char* senderNum, const char* receiverSipID, const char* receiverNum)
{
	char subject[128] = { 0 };
	sprintf("%s:%s,%s:%s", senderSipID, senderNum, receiverSipID, receiverNum);

	int result = -1;
	if (sipMsg != nullptr)
	{
		result = osip_message_set_subject(sipMsg, subject);
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgDate(osip_message_t* sipMsg, const char* date)
{
	int result = -1;
	if (sipMsg != nullptr && date != nullptr)
	{
		osip_message_set_date(sipMsg, date);
		result = 0;
	}
	return result;
}

int SipMsgHeaderParser::SetSipMsgWWW_Authrnticate(osip_message_t * sipMsg, const char * realm, const char * nonce, const char * type)
{
	int result = -1;

	if (sipMsg != nullptr && type != nullptr && realm != nullptr && nonce != nullptr)
	{
		osip_www_authenticate_t *www_authenticate = nullptr;
		osip_www_authenticate_init(&www_authenticate);

		www_authenticate->nonce = osip_strdup(nonce);
		www_authenticate->realm = osip_strdup(realm);
		www_authenticate->auth_type = osip_strdup(type);

		if (sipMsg->www_authenticates.nb_elt > 0)
		{
			osip_list_special_free(&sipMsg->www_authenticates, (void(*)(void*))&osip_www_authenticate_free);
		}

		result = osip_list_add(&sipMsg->www_authenticates, www_authenticate, -1);
	}
	return result;
}


int SipMsgHeaderParser::SetSipMsgAuthorization(osip_message_t * sipMsg, const osip_list_t www_authenticates, const char* password)
{
	int result = -1;
	if (sipMsg != nullptr && sipMsg->sip_method != nullptr && www_authenticates.node != nullptr && password != nullptr)
	{
		osip_list_iterator_t iterator;
		osip_www_authenticate_t* www_authenticate = nullptr;
		www_authenticate = (osip_www_authenticate_t*)osip_list_get_first(&www_authenticates, &iterator);

		// usename
		char username[32] = { 0 };
		sprintf(username, "\"%s\"", sipMsg->from->url->username);

		// uri
		char* uri = nullptr;
		osip_uri_to_str(sipMsg->req_uri, &uri);
		char strUri[64] = { 0 };
		sprintf(strUri, "\"%s\"", uri);
		osip_free(uri);
		uri = nullptr;

		osip_authorization_t* authorization = nullptr;
		osip_authorization_init(&authorization);
		authorization->auth_type = osip_strdup(www_authenticate->auth_type);
		authorization->username = osip_strdup(username);
		authorization->realm = osip_strdup(www_authenticate->realm);
		authorization->nonce = osip_strdup(www_authenticate->nonce);
		authorization->uri = osip_strdup(strUri);
		authorization->algorithm = osip_strdup("MD5");

		// response
		std::string response = "\"";
		//response = response + SipCredentials::GetCredentials(sipMsg->sip_method, authorization, password) + "\"";
		authorization->response = osip_strdup(response.c_str());

		if (sipMsg->authorizations.nb_elt)
		{
			osip_list_special_free(&sipMsg->authorizations, (void(*)(void *))&osip_authorization_free);
		}
		result = osip_list_add(&sipMsg->authorizations, authorization, -1);
	}
	return result;
}
