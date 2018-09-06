#include "SipSdpBodyParser.h"



//SipMsgSdpBody::SipMsgSdpBody()
//{
//}
//
//
//SipMsgSdpBody::~SipMsgSdpBody()
//{
//}
//
//int SipMsgSdpBody::CreateSipMsgBody(void * bodyParameter, std::string & bodyString)
//{
//	bodyString =
//		"v=0\r\n"\
//		"o=34010000002005000001 0 0 IN IP4 192.168.1.110\r\n"\
//		"s=Play\r\n"\
//		"c=IN IP4 192.168.1.110\r\n"\
//		"t=0 0\r\n"\
//		"m=video 8268 RTP/AVP 96 98 97\r\n"\
//		"a=recvonly\r\n"\
//		"a=rtpmap:96 PS/90000\r\n"\
//		"a=rtpmap:98 H264/90000\r\n"\
//		"a=rtpmap:97 MPEG4/90000\r\n"\
//		"y=0100000001\r\n"\
//		"f=v/////a///\r\n";
//	return 0;
//}
//
//int SipMsgSdpBody::ParserSipMsgBody(const char * bodyString, void * bodyParameter)
//{
//	return 0;
//}


SipSdpBodyParser::SipSdpBodyParser()
{
}


SipSdpBodyParser::~SipSdpBodyParser()
{
}

int SipSdpBodyParser::CreateSipMsgBody(void * bodyParameter, std::string & bodyString)
{
	SdpMediaParam& sdpMediaParam = *(SdpMediaParam*)bodyParameter;

	sdp_message_t* sdpMsg = nullptr;
	sdp_message_init(&sdpMsg);

	// v
	SetV_Version(sdpMsg);

	// o
	SetO_Origin(sdpMsg, sdpMediaParam);

	// S
	SetS_Name(sdpMsg, sdpMediaParam.sName);

	// c
	SetC_Conection(sdpMsg, sdpMediaParam);

	// u
	SetU_Uri(sdpMsg, sdpMediaParam);

	// t
	SetT_Descrs(sdpMsg, sdpMediaParam.tParam.startTime, sdpMediaParam.tParam.endTime);

	// m
	SetM_Medias(sdpMsg, sdpMediaParam);

	char* sdpBody = nullptr;
	sdp_message_to_str(sdpMsg, &sdpBody);
	sdp_message_free(sdpMsg); // 释放内存，防止内存泄漏
	sdpMsg = nullptr;

	if (sdpBody)
	{
		bodyString = sdpBody;
		osip_free(sdpBody);
		sdpBody = nullptr;
	}

	// y
	SetY(bodyString, sdpMediaParam.y);

	// f
	SetF(bodyString, sdpMediaParam.f);

	return 0;
}

int SipSdpBodyParser::ParserSipMsgBody(const char *bodyString, void * bodyParameter)
{
	/*SdpMediaParam & mediaParam = *(SdpMediaParam*)bodyParameter;

	sdp_message_t* sdpMsg = nullptr;
	sdp_message_init(&sdpMsg);
	sdp_message_parse(sdpMsg, bodyString.c_str());

	// v

	// o
	GetOwner(sdpMsg, mediaParam);

	// s, 确定视频点播类型
	GetS_Name(sdpMsg, mediaParam);
	GetT_Descrs(sdpMsg, mediaParam);

	// c
	GetConnection(sdpMsg, mediaParam);

	// u
	GetU_Uri(sdpMsg, mediaParam);

	// 解析媒体信息
	GetM_Medias(sdpMsg, mediaParam);

	// 解析Y字段
	GetY(bodyString, mediaParam.y);*/

	// 解析k字段

	// 释放内存
	//sdp_message_free(sdpMsg);
	//sdpMsg = nullptr;

	return 0;
}

int SipSdpBodyParser::SetV_Version(sdp_message_t * sdpMsg, const std::string version)
{
	int reslult = -1;
	if (sdpMsg != nullptr && version.size())
	{
		if (sdpMsg->v_version != nullptr)
		{
			osip_free(sdpMsg->v_version);
			sdpMsg->v_version = nullptr;
		}
		sdpMsg->v_version = osip_strdup(version.c_str());
		reslult = 0;
	}

	return reslult;
}

int SipSdpBodyParser::SetO_UserName(sdp_message_t * sdpMsg, const std::string userName)
{
	if (sdpMsg->o_username != nullptr)
	{
		osip_free(sdpMsg->o_username);
		sdpMsg->o_username = nullptr;
	}
	sdpMsg->o_username = osip_strdup(userName.c_str());

	return 0;
}

int SipSdpBodyParser::SetO_SessID(sdp_message_t * sdpMsg, const std::string sessID)
{
	if (sdpMsg->o_sess_id != nullptr)
	{
		osip_free(sdpMsg->o_sess_id);
		sdpMsg->o_sess_id = nullptr;
	}

	sdpMsg->o_sess_id = osip_strdup(sessID.c_str());

	return 0;
}

int SipSdpBodyParser::SetO_SessVersion(sdp_message_t * sdpMsg, const std::string sessVersion)
{
	if (sdpMsg->o_sess_version != nullptr)
	{
		osip_free(sdpMsg->o_sess_version);
	}

	sdpMsg->o_sess_version = osip_strdup(sessVersion.c_str());

	return 0;
}

int SipSdpBodyParser::SetO_NetType(sdp_message_t * sdpMsg, const std::string netType)
{
	if (sdpMsg->o_nettype != nullptr)
	{
		osip_free(sdpMsg->o_nettype);
	}

	sdpMsg->o_nettype = osip_strdup(netType.c_str());

	return 0;
}

int SipSdpBodyParser::SetO_AddrType(sdp_message_t* sdpMsg, const std::string addrType /*= "IP4"*/)
{
	if (sdpMsg->o_addrtype != nullptr)
	{
		osip_free(sdpMsg->o_addrtype);
		sdpMsg->o_addrtype = nullptr;
	}

	sdpMsg->o_addrtype = osip_strdup(addrType.c_str());

	return 0;
}

int SipSdpBodyParser::SetO_Addr(sdp_message_t * sdpMsg, const std::string addr)
{
	if (sdpMsg->o_addr != nullptr)
	{
		osip_free(sdpMsg->o_addr);
		sdpMsg = nullptr;
	}
	sdpMsg->o_addr = osip_strdup(addr.c_str());
	return 0;
}

int SipSdpBodyParser::SetO_Origin(sdp_message_t * sdpMsg, const SdpMediaParam & sdpMediaParam)
{
	int result = -1;
	if (sdpMsg != nullptr)
	{
		SetO_UserName(sdpMsg, sdpMediaParam.oParam.userName);
		SetO_SessID(sdpMsg, sdpMediaParam.oParam.sessID);
		SetO_SessVersion(sdpMsg, sdpMediaParam.oParam.sessVersion);
		SetO_NetType(sdpMsg, sdpMediaParam.oParam.netType);
		SetO_AddrType(sdpMsg, sdpMediaParam.oParam.addrType);
		SetO_Addr(sdpMsg, sdpMediaParam.oParam.addr);
		result = 0;

		// 		char* userName = osip_strdup(sdpMediaParam.oParam.userName.c_str());
		// 		char* sessID = osip_strdup(sdpMediaParam.oParam.sessID.c_str());
		// 		char* sessVersion = osip_strdup(sdpMediaParam.oParam.sessVersion.c_str());
		// 		char* netType = osip_strdup(sdpMediaParam.oParam.netType.c_str());
		// 		char* addrType = osip_strdup(sdpMediaParam.oParam.addrType.c_str());
		// 		char* addr = osip_strdup(sdpMediaParam.oParam.addr.c_str());
		// 		result = sdp_message_o_origin_set(sdpMsg, userName, sessID, sessVersion, netType, addrType, addr);
	}

	return 0;
}

int SipSdpBodyParser::GetOwner(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	if (sdpMsg->o_addr)
	{
		sdpMediaParam.oParam.addr = sdpMsg->o_addr;
	}
	if (sdpMsg->o_username)
	{
		sdpMediaParam.oParam.userName = sdpMsg->o_username;
	}

	return 0;
}

int SipSdpBodyParser::SetS_Name(sdp_message_t * sdpMsg, const std::string name)
{
	if (sdpMsg->s_name != nullptr)
	{
		osip_free(sdpMsg->s_name);
		sdpMsg->s_name = nullptr;
	}
	sdpMsg->s_name = osip_strdup(name.c_str());
	return 0;
}

int SipSdpBodyParser::GetS_Name(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	if (sdpMsg && sdpMsg->s_name)
	{
		sdpMediaParam.sName = sdpMsg->s_name;
	}
	return 0;
}


int SipSdpBodyParser::SetU_Uri(sdp_message_t* sdpMsg, SdpMediaParam &sdpMediaParam)
{
	if (!sdpMediaParam.uParam.deviceID.empty())
	{
		std::string uri = sdpMediaParam.uParam.deviceID + ":" + std::to_string(sdpMediaParam.uParam.num);
		if (sdpMsg->u_uri)
		{
			osip_free(sdpMsg->u_uri);
			sdpMsg->u_uri = nullptr;
		}
		sdpMsg->u_uri = osip_strdup(uri.c_str());
	}
	return 0;
}

int SipSdpBodyParser::GetU_Uri(sdp_message_t* sdpMsg, SdpMediaParam &sdpMediaParam)
{
	std::string num = "";
	if (sdpMsg->u_uri)
	{
		//SipCommon::ParseKeyVal(sdpMsg->u_uri, sizeof(sdpMsg->u_uri) + 1, ':', sdpMediaParam.uParam.deviceID, num);
		if (!num.empty())
		{
			SipCommon::Str2Int(num, sdpMediaParam.uParam.num);
		}
	}
	return 0;
}

int SipSdpBodyParser::SetC_NetType(sdp_message_t* sdpMsg, const std::string netType /*= "IN"*/)
{
	if (sdpMsg->c_connection == nullptr)
	{
		sdp_connection_init(&sdpMsg->c_connection);
	}

	if (sdpMsg->c_connection->c_nettype != nullptr)
	{
		osip_free(sdpMsg->c_connection->c_nettype);
		sdpMsg->c_connection->c_nettype = nullptr;
	}

	sdpMsg->c_connection->c_nettype = osip_strdup(netType.c_str());

	return 0;
}

int SipSdpBodyParser::SetC_AddrType(sdp_message_t* sdpMsg, const std::string addrType /*= "IP4"*/)
{
	if (sdpMsg->c_connection->c_addrtype != nullptr)
	{
		osip_free(sdpMsg->c_connection->c_addrtype);
		sdpMsg->c_connection->c_addrtype = nullptr;
	}

	sdpMsg->c_connection->c_addrtype = osip_strdup(addrType.c_str());

	return 0;
}

int SipSdpBodyParser::SetC_Addr(sdp_message_t* sdpMsg, const std::string addr)
{
	if (sdpMsg->c_connection->c_addr != nullptr)
	{
		osip_free(sdpMsg->c_connection->c_addr);
		sdpMsg->c_connection->c_addr = nullptr;
	}

	sdpMsg->c_connection->c_addr = osip_strdup(addr.c_str());

	return 0;
}

int SipSdpBodyParser::SetC_Conection(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam)
{
	if (sdpMsg->c_connection != nullptr)
	{
		osip_free(sdpMsg->c_connection);
		sdpMsg->c_connection = nullptr;
	}
	SetC_NetType(sdpMsg, sdpMediaParam.cParam.netType);
	SetC_AddrType(sdpMsg, sdpMediaParam.cParam.addrType);
	SetC_Addr(sdpMsg, sdpMediaParam.cParam.addr);

	return 0;
}

int SipSdpBodyParser::GetConnection(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	if (sdpMsg->c_connection && sdpMsg->c_connection->c_addr)
	{
		sdpMediaParam.cParam.addr = sdpMsg->c_connection->c_addr;
	}
	return 0;
}

int SipSdpBodyParser::SetT_Descrs(sdp_message_t * sdpMsg, const std::string startTime, const std::string endTime)
{
	char* sipStart = osip_strdup(startTime.c_str());
	char* sipEnd = osip_strdup(endTime.c_str());

	osip_list_special_free(&sdpMsg->t_descrs, (void(*)(void*))sdp_time_descr_free);

	sdp_message_t_time_descr_add(sdpMsg, sipStart, sipEnd);
	return 0;
}

int SipSdpBodyParser::GetT_Descrs(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	char* startTime = sdp_message_t_start_time_get(sdpMsg, 0);
	char* endTime = sdp_message_t_stop_time_get(sdpMsg, 0);
	if (startTime)
	{
		sdpMediaParam.tParam.startTime = startTime;
	}
	if (endTime)
	{
		sdpMediaParam.tParam.endTime = endTime;
	}
	return 0;
}

int SipSdpBodyParser::SetM_Medias(sdp_message_t* sdpMsg, const SdpMediaParam &sdpMediaParam)
{
	if (sdpMsg->m_medias.node)
	{
		osip_list_special_free(&sdpMsg->m_medias, (void(*)(void*))&sdp_media_free);
	}

	sdp_media_t* sdpMedia = nullptr;
	sdp_media_init(&sdpMedia);

	sdpMedia->m_media = osip_strdup(sdpMediaParam.mParam.mediaType.c_str());
	sdpMedia->m_port = osip_strdup(std::to_string(sdpMediaParam.mParam.port).c_str());
	sdpMedia->m_proto = osip_strdup(sdpMediaParam.mParam.rtpType.c_str());

	if (sdpMediaParam.mParam.payloads.empty())
	{
		osip_list_add(&sdpMedia->m_payloads, osip_strdup("96"), -1);
		osip_list_add(&sdpMedia->m_payloads, osip_strdup("98"), -1);
		osip_list_add(&sdpMedia->m_payloads, osip_strdup("97"), -1);
	}
	else
	{
		char* sipPlayload = nullptr;
		for (std::string payload : sdpMediaParam.mParam.payloads)
		{
			sipPlayload = osip_strdup(payload.c_str());
			osip_list_add(&sdpMedia->m_payloads, sipPlayload, -1);
		}
	}

	// 添加m中的a字段
	SetM_Attributes(sdpMedia, sdpMediaParam);
	osip_list_add(&sdpMsg->m_medias, sdpMedia, -1);
	return 0;
}

int SipSdpBodyParser::SetM_Attributes(sdp_media_t * sdpMedia, const SdpMediaParam & sdpMediaParam)
{
	osip_list_special_free(&sdpMedia->a_attributes, (void(*)(void*))sdp_attribute_free);

	sdp_attribute_t* attribute = nullptr;

	// recvonly、sendonly
	if (sdpMediaParam.mParam.isRecvOnly)
	{
		SetAttribute(attribute, "recvonly", "");
	}
	else
	{
		SetAttribute(attribute, "sendonly", "");
	}
	osip_list_add(&sdpMedia->a_attributes, attribute, -1);

	// tcp连接主动、被动方式
	if (sdpMediaParam.mParam.transType == "active" || sdpMediaParam.mParam.transType == "passive")
	{
		SetAttribute(attribute, "setup", sdpMediaParam.mParam.transType);
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);

		SetAttribute(attribute, "connection", "new");
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);
	}


	if (sdpMediaParam.mParam.attributes.empty())
	{
		SetAttribute(attribute, "rtpmap", "96 PS/90000");
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);

		SetAttribute(attribute, "rtpmap", "98 H264/90000");
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);

		SetAttribute(attribute, "rtpmap", "97 MPEG4/90000");
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);
	}
	else
	{
		// "96 PS/90000"、"rtpmap", "98 H264/90000"、"97 MPEG4/90000"
		for (std::string payload : sdpMediaParam.mParam.attributes)
		{
			SetAttribute(attribute, "rtpmap", payload);
			osip_list_add(&sdpMedia->a_attributes, attribute, -1);
		}
	}

	if (!sdpMediaParam.mParam.downloadSpeed.empty() &&
		SipCommon::CompareStr(sdpMediaParam.sName.c_str(), "Download"))// 下载是添加默认下载速度
	{
		SetAttribute(attribute, "downloadspeed", sdpMediaParam.mParam.downloadSpeed);
		osip_list_add(&sdpMedia->a_attributes, attribute, -1);
	}

	return 0;
}

int SipSdpBodyParser::SetAttribute(sdp_attribute_t*& attribute, const std::string field, const std::string value)
{
	sdp_attribute_init(&attribute);

	if (!field.empty())
	{
		attribute->a_att_field = osip_strdup(field.c_str());
	}

	if (!value.empty())
	{
		attribute->a_att_value = osip_strdup(value.c_str());
	}

	return 0;
}

int SipSdpBodyParser::GetM_Medias(sdp_message_t * sdpMsg, SdpMediaParam & sdpMediaParam)
{
	int result = -1;

	sdp_media_t* sdpMedia = (sdp_media_t*)osip_list_get(&sdpMsg->m_medias, 0);
	if (sdpMedia == nullptr)
	{
		return result;
	}

	if (sdpMedia->m_media == "audio") // 下放后回复携带多个sdpMedia
	{
	}

	if (sdpMedia->m_port)
	{
		result = SipCommon::Str2Int(sdpMedia->m_port, sdpMediaParam.mParam.port);
		if (result)
		{
			return result;
		}
	}

	// 解析媒体类型
	if (sdpMedia->m_media)
	{
		sdpMediaParam.mParam.mediaType = sdpMedia->m_media;
	}

	if (sdpMedia->m_proto)
	{
		sdpMediaParam.mParam.rtpType = sdpMedia->m_proto;
	}

	// 解析media中的payload
	osip_list_t& m_payloads = sdpMedia->m_payloads;
	while (m_payloads.node)
	{
		sdpMediaParam.mParam.payloads.push_back((char*)m_payloads.node->element);
		m_payloads.node = m_payloads.node->next;
	}

	// 解析media中的attribute
	osip_list_t m_attributes = sdpMedia->a_attributes;
	while (m_attributes.node)
	{
		sdp_attribute_t* attribute = (sdp_attribute_t*)m_attributes.node->element;

		if (!strcmp(attribute->a_att_field, "recvonly"))
		{
			sdpMediaParam.mParam.isRecvOnly = true;
		}
		else if (!strcmp(attribute->a_att_field, "sendonly"))
		{
			sdpMediaParam.mParam.isRecvOnly = false;
		}
		else if (!strcmp(attribute->a_att_field, "setup"))
		{
			sdpMediaParam.mParam.transType = attribute->a_att_value;
		}
		else if (!strcmp(attribute->a_att_field, "rtpmap"))
		{
			sdpMediaParam.mParam.attributes.push_back(attribute->a_att_value);
		}

		m_attributes.node = m_attributes.node->next;
	}

	return 0;
}

int SipSdpBodyParser::SetY(std::string &sdpMsg, std::string y)
{
	if (!y.empty())
	{
		sdpMsg = sdpMsg + "y=" + y + "\r\n";
	}
	return 0;
}

int SipSdpBodyParser::GetY(std::string sdpMsg, std::string & y)
{
	std::string ssrcY = "";
	int ssrc = 0;
	//for (int i = 0; i < sdpMsg.size(); i++)
	//{
	//	if (sdpMsg[i] == 'y' || sdpMsg[i] == 'Y')
	//	{
	//		if (sdpMsg.size() - i > 2 && (sdpMsg[i + 1] == '=' || sdpMsg[i + 2] == '='))
	//		{
	//			ssrcY = sdpMsg.substr(i, sdpMsg.size() - i);
	//			break;
	//		}
	//	}
	//}
	int a = sdpMsg.find("y=", 0);
	if (a < 0)
	{
		a = sdpMsg.find("y =", 0);
	}

	if (a > 0)
	{
		std::string subY = sdpMsg.substr(a, y.size() - a - 1);
		sscanf(subY.c_str(), "y=%d", &ssrc);
		y = std::to_string(ssrc);
	}
	else
	{
		y = "";
	}

	return 0;
}


int SipSdpBodyParser::SetF(std::string &sdpMsg, std::string f)
{
	if (!f.empty())
	{
		sdpMsg = sdpMsg + "f=" + f + "\r\n";
	}
	return 0;
}

int SipSdpBodyParser::GetF(std::string & sdpMsg, std::string f)
{
	return 0;
}


