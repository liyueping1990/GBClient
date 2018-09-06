#include "SipXmlBodyParser.h"

std::once_flag SipXmlBodyParser::mOnceFlag;
CreateBodyCallback SipXmlBodyParser::CreateBodyCallbacks[XML_COUNT];

SipXmlBodyParser::SipXmlBodyParser()
{
}


SipXmlBodyParser::~SipXmlBodyParser()
{
}

int SipXmlBodyParser::CreateSipMsgBody(void * bodyParameter, std::string & bodyString)
{
	std::call_once(mOnceFlag, RegisterCreateCallback);

	XmlCmdParam * xmlCmdParam = (XmlCmdParam*)bodyParameter;

	// xml文档指针
	tinyxml2::XMLDocument* xmlDocument = new tinyxml2::XMLDocument;

	// 创建xml声明
	tinyxml2::XMLDeclaration* declaration = xmlDocument->NewDeclaration();
	xmlDocument->InsertFirstChild(declaration);

	// xml根节点
	tinyxml2::XMLElement* rootElement = xmlDocument->NewElement("Control");
	xmlDocument->InsertEndChild(rootElement);

	// 通用节点
	tinyxml2::XMLElement* cmdTypeElement = xmlDocument->NewElement("CmdType");
	rootElement->InsertEndChild(cmdTypeElement);
	InsertChildElement(xmlDocument, rootElement, "SN", xmlCmdParam->sn);
	InsertChildElement(xmlDocument, rootElement, "DeviceID", xmlCmdParam->deviceID);

	// 调用注册回调函数,设置xml文档
	int result = -1;
	result = CreateBodyCallbacks[xmlCmdParam->manscdpType](xmlDocument, xmlCmdParam);
	if (!result)
	{
		// 设置节点名称和值，element: <value>text</value>
		rootElement->SetValue(xmlCmdParam->reqRespType.c_str());
		cmdTypeElement->SetText(xmlCmdParam->cmdType.c_str());

		// 将xml文件转换为字符串
		tinyxml2::XMLPrinter printer;
		xmlDocument->Accept(&printer);
		bodyString = printer.CStr();
	}
	else
	{
		bodyString = "";
	}

	// 释放内存
	delete xmlDocument;
	xmlDocument = nullptr;

	return result;
}

int SipXmlBodyParser::ParserSipMsgBody(const char * bodyString, void* bodyParameter)
{
	XmlParam* xmlCmdParam = (XmlParam*)bodyParameter;

	tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument;
	xmlDoc->Parse(bodyString);

	int result = -1;
	tinyxml2::XMLElement* xmlRootElement = xmlDoc->RootElement();
	if (xmlRootElement)
	{
		// 获取消息类型
		xmlCmdParam->reqRespType = xmlRootElement->Value();// Control、Query、Notify、Response
		
				// sip消息所携带的消息体具体分类解析
		if (xmlCmdParam->reqRespType == "Control") // 控制
		{
			result = ParserControl(xmlRootElement, xmlCmdParam);
		}
		else if (xmlCmdParam->reqRespType == "Query") // 查询
		{
			result = ParserQuery(xmlRootElement, xmlCmdParam);
		}
		else if (xmlCmdParam->reqRespType == "Notify") // 通知
		{
			result = ParserNotify(xmlRootElement, xmlCmdParam);
		}
		else if (xmlCmdParam->reqRespType == "Response") // 应答
		{
			result = ParserResponse(xmlRootElement, xmlCmdParam);
		}
	}



	xmlDoc->Clear();
	delete xmlDoc;
	xmlDoc = nullptr;

	return 0;
}

void SipXmlBodyParser::SetCreateCallback(int type, CreateBodyCallback cb)
{
	CreateBodyCallbacks[type] = cb;
}

void SipXmlBodyParser::RegisterCreateCallback()
{
	// Control
	SetCreateCallback(CONTROL_DEVICE_PTZ, ControlPtzCmd);
	SetCreateCallback(CONTROL_TELE_BOOT, ControlTeleBoot);
	SetCreateCallback(CONTROL_RECORD, ControlRecordCmd);
	SetCreateCallback(CONTROL_GUARD, ControlGuardCmd);
	SetCreateCallback(CONTROL_ALARM, ControlAlarmCmd);
	SetCreateCallback(CONTROL_IFAME, ControlIFameCmd);
	SetCreateCallback(CONTROL_DRAG_ZOOM, ControlDragZoom);
	SetCreateCallback(CONTROL_HOME_POSITION, ControlHomePosition);
	SetCreateCallback(CONTROL_DEVICE_CONFIG, ControlDeviceConfig);

	// Query
	SetCreateCallback(QUERY_DEVICE_STATUS, QueryDeviceStatus);
	SetCreateCallback(QUERY_DEVICE_CATALOG, QueryCatalog);
	SetCreateCallback(QUERY_DEVICE_INFO, QueryDeviceInfo);
	SetCreateCallback(QUERY_RECORD_INFO, QueryRecordInfo);
	SetCreateCallback(QUERY_ALARM, QueryAlarm);
	SetCreateCallback(QUERY_CONFIG_DOWNLOAD, QueryConfigDownload);
	SetCreateCallback(QUERY_PRESET_QUERY, QueryPreset);
	SetCreateCallback(QUERY_MOBILE_POSITION, QueryMobilePosition);

	// Notify
	SetCreateCallback(NOTIFY_KEEPALIVE, NotifyKeepalive);
	SetCreateCallback(NOTIFY_ALARM, NotifyAlarm);
	SetCreateCallback(NOTIFY_MEDIA_STATUS, NotifyMediaStutas);
	SetCreateCallback(NOTIFY_BROADCAST, NotifyBroadcast);
	SetCreateCallback(NOTIFY_MOBILE_POSITION, NotifyMobilePosition);
	SetCreateCallback(NOTIFY_CATALOG, NotifyCatalog);

	// Response
	SetCreateCallback(RESPONSE_DEVICE_CONTROL, ResponseDeviceControl);
	SetCreateCallback(RESPONSE_ALARM_NOTIFY, ResponseAlarmNotify);
	SetCreateCallback(RESPONSE_DEVICE_CATALOG_QUERY, ResponseCatalog);
	SetCreateCallback(RESPONSE_CATALOG_QUERY_RECEIVED, ResponseRcvCatalogQuery);
	SetCreateCallback(RESPONSE_CATALOG_RECEIVED, ResponseRcvCatalog);
	SetCreateCallback(RESPONSE_DEVICE_INFO_QUERY, ResponseDeviceInfoQuery);
	SetCreateCallback(RESPONSE_DEVICE_STATUS_QUERY, ResponseDeviceStatusQuery);
	SetCreateCallback(RESPONSE_DOCUMENT_RECORD_INFO, ResponseRecordInfo);
	SetCreateCallback(RESPONSE_DEVICE_CONFIG, ResponseDeviceConfig);
	SetCreateCallback(RESPONSE_DEVICE_CONFIG_DOWNLOAD, ResponseConfigDownload);
	SetCreateCallback(RESPONSE_DEVICE_PRESET_QUERY, ResponsePresetQuery);
	SetCreateCallback(RESPONSE_BROADCAST, ResponseBroadcast);
}

int SipXmlBodyParser::ControlPtzCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlTeleBoot(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlRecordCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlGuardCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlAlarmCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlIFameCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlDragZoom(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlHomePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ControlDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryDeviceStatus(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	xmlParam->reqRespType = "Query";
	xmlParam->cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// 获取根节点指针

	InsertChildElement(xmlDoc, rootElement, "StartTime", xmlParam->queryParam.queryCatalogParam.startTime);
	InsertChildElement(xmlDoc, rootElement, "EndTime", xmlParam->queryParam.queryCatalogParam.endTime);

	return 0;
}

int SipXmlBodyParser::QueryDeviceInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryPreset(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::QueryMobilePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::NotifyKeepalive(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::NotifyAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::NotifyMediaStutas(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::NotifyBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::NotifyMobilePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::NotifyCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseDeviceControl(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseAlarmNotify(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseRcvCatalogQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseRcvCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseDeviceInfoQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseDeviceStatusQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponsePresetQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ResponseBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam* xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ParserControl(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam * xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ParserQuery(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam * xmlParam)
{
	return 0;
}

int SipXmlBodyParser::ParserNotify(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam * xmlParam)
{
	int result = -1;
	if (!GetChildElementValue(xmlRootElement, "CmdType", xmlParam->cmdType) &&
		!GetChildElementValue(xmlRootElement, "SN", xmlParam->sn))
	{
		if (xmlParam->cmdType == "Broadcast") // 语音广播通知
		{
		} 
		else if (!GetChildElementValue(xmlRootElement, "DeviceID", xmlParam->deviceID))
		{
			if (xmlParam->cmdType == "Keepalive") // 状态信息报送
			{
				xmlParam->manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_KEEPALIVE;
				// Info扩展暂时未使用
			}
			else if (true)
			{

			}
		}
	} 
	return result;
}

int SipXmlBodyParser::ParserResponse(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam * xmlParam)
{
	return 0;
}

void SipXmlBodyParser::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement* &parentElement, std::string childElementName, std::string childElementValue)
{
	if (xmlDoc && parentElement && !childElementValue.empty())
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		childElement->SetText(childElementValue.c_str());
	}
}

int SipXmlBodyParser::GetChildElementValue(tinyxml2::XMLElement* parentElement, std::string elementName, std::string &elementValue)
{
	int result = -1;
	elementValue = "";
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element && element->GetText())
	{
		elementValue = element->GetText();
		result = 0;
	}
	return result;
}

int SipXmlBodyParser::GetChildElementValue(tinyxml2::XMLElement* parentElement, std::string elementName, int &elementValue)
{
	int result = -1;
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element && element->GetText())
	{
		result = element->QueryIntText(&elementValue);
	}
	return result;
}
