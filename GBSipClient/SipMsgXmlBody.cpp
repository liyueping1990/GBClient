#include "SipMsgXmlBody.h"
#include "SipUtilities.h"

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

	XmlCmdParam & xmlCmdParam = *(XmlCmdParam*)bodyParameter;

	// xml�ĵ�ָ��
	tinyxml2::XMLDocument* xmlDocument = new tinyxml2::XMLDocument;

	// ����xml����
	tinyxml2::XMLDeclaration* declaration = xmlDocument->NewDeclaration();
	xmlDocument->InsertFirstChild(declaration);

	// xml���ڵ�
	tinyxml2::XMLElement* rootElement = xmlDocument->NewElement(xmlCmdParam.reqRespType.c_str());
	xmlDocument->InsertEndChild(rootElement);

	// ͨ�ýڵ�
	tinyxml2::XMLElement* cmdTypeElement = xmlDocument->NewElement("CmdType");
	rootElement->InsertEndChild(cmdTypeElement);
	InsertChildElement(xmlDocument, rootElement, "SN", xmlCmdParam.sn);
	InsertChildElement(xmlDocument, rootElement, "DeviceID", xmlCmdParam.deviceID);


	// ����ע��ص�����,����xml�ĵ�
	int result = -1;
	result = CreateBodyCallbacks[xmlCmdParam.manscdpType](xmlDocument, xmlCmdParam);
	if (!result)
	{
		// ���ýڵ����ƺ�ֵ��element: <value>text</value>
		rootElement->SetValue(xmlCmdParam.reqRespType.c_str());
		cmdTypeElement->SetText(xmlCmdParam.cmdType.c_str());

		// ��xml�ļ�ת��Ϊ�ַ���
		tinyxml2::XMLPrinter printer;
		xmlDocument->Accept(&printer);
		bodyString = printer.CStr();
	}
	else
	{
		bodyString = "";
	}

	// �ͷ��ڴ�
	delete xmlDocument;
	xmlDocument = nullptr;

	return result;
}

/*std::string utf8ToGb2312(const char* utf8)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wideStr = conv.from_bytes(utf8);

	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
		converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
	std::string narrowStr = converter.to_bytes(wideStr);
	return narrowStr;
}*/

int SipXmlBodyParser::ParserSipMsgBody(const std::string bodyString, void * bodyParameter)
{
	XmlParam & xmlCmdParam = *(XmlParam*)bodyParameter;

	tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument;
	xmlDoc->Parse(bodyString.c_str());
	//if (xmlDoc->HasBOM())
	//{
	//	std::string gb2312 = utf8ToGb2312(bodyString.c_str());
	//	xmlDoc->Parse(gb2312.c_str());
	//}

	int result = -1;
	tinyxml2::XMLElement* xmlRootElement = xmlDoc->RootElement();
	if (xmlRootElement != nullptr)
	{		
		// ��ȡ��Ϣ����
		xmlCmdParam.reqRespType = xmlRootElement->Value();// Control��Query��Notify��Response

		// sip��Ϣ��Я������Ϣ�����������
		if (xmlCmdParam.reqRespType == "Control") // ����
		{
			result = ParserControl(xmlRootElement, xmlCmdParam);
		}
		else if (xmlCmdParam.reqRespType == "Query") // ��ѯ
		{
			result = ParserQuery(xmlRootElement, xmlCmdParam);
		}
		else if (xmlCmdParam.reqRespType == "Notify") // ֪ͨ
		{
			result = ParserNotify(xmlRootElement, xmlCmdParam);
		}
		else if (xmlCmdParam.reqRespType == "Response") // Ӧ��
		{
			result = ParserResponse(xmlRootElement, xmlCmdParam);
		}
	}

	// �ͷ��ڴ�
	delete xmlDoc;
	xmlDoc = nullptr;

	return result;
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

int SipXmlBodyParser::ControlPtzCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	GenerateControlCmd(xmlParam.controlParam.ptzParam, xmlParam.controlParam.ptzParam.ptzCmdValue);
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();
	InsertChildElement(xmlDoc, rootElement, "PTZCmd", xmlParam.controlParam.ptzParam.ptzCmdValue);
	
	return 0;
}

int SipXmlBodyParser::ControlTeleBoot(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();   // ��ȡ���ڵ�ָ��	
	InsertChildElement(xmlDoc, rootElement, "TeleBoot", "Boot"); // ���Զ�����������ֶ�
	return 0;
}

int SipXmlBodyParser::ControlRecordCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // ��ȡ���ڵ�ָ��	
	InsertChildElement(xmlDoc, rootElement, "Record", xmlParam.controlParam.recordType); // ����ֶ�¼�������ֶ�
	return 0;
}

int SipXmlBodyParser::ControlGuardCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // ��ȡ���ڵ�ָ��	
	InsertChildElement(xmlDoc, rootElement, "GuardCmd", xmlParam.controlParam.guardType); // ���***�����ֶ�
	return 0;
}

int SipXmlBodyParser::ControlAlarmCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement(); // ��ȡ���ڵ�ָ��	
	InsertChildElement(xmlDoc, rootElement, "AlarmCmd", xmlParam.controlParam.alarmParam.resetAlarm); // ���***�����ֶ�

	tinyxml2::XMLElement* info = xmlDoc->NewElement("Info");
	InsertChildElement(xmlDoc, info, "AlarmMethod", xmlParam.controlParam.alarmParam.alarmMethod);
	InsertChildElement(xmlDoc, info, "AlarmType", xmlParam.controlParam.alarmParam.alarmType);

	return 0;
}

int SipXmlBodyParser::ControlIFameCmd(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
	InsertChildElement(xmlDoc, rootElement, "IFameCmd", "Send");// ���ǿ�ƹؼ�֡��������
	return 0;
}

int SipXmlBodyParser::ControlDragZoom(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	tinyxml2::XMLElement* dragZoom = nullptr;
	if (xmlParam.controlParam.dragZoom.isZoomOut)
	{
		dragZoom = xmlDoc->NewElement("DragZoomOut");
	}
	else
	{
		dragZoom = xmlDoc->NewElement("DragZoomIn");
	}
	rootElement->InsertEndChild(dragZoom);

	InsertChildElement(xmlDoc, dragZoom, "Length", std::to_string(xmlParam.controlParam.dragZoom.length));// �������Ŵ����
	InsertChildElement(xmlDoc, dragZoom, "Width", std::to_string(xmlParam.controlParam.dragZoom.width));// �������Ŵ����
	InsertChildElement(xmlDoc, dragZoom, "MidPointX", std::to_string(xmlParam.controlParam.dragZoom.midPointX));// �������Ŵ����
	InsertChildElement(xmlDoc, dragZoom, "MidPointY", std::to_string(xmlParam.controlParam.dragZoom.midPointY));// �������Ŵ����
	InsertChildElement(xmlDoc, dragZoom, "LengthX", std::to_string(xmlParam.controlParam.dragZoom.lengthX));// �������Ŵ����
	InsertChildElement(xmlDoc, dragZoom, "LengthY", std::to_string(xmlParam.controlParam.dragZoom.lengthY));// �������Ŵ����
	return 0;
}

int SipXmlBodyParser::ControlHomePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	tinyxml2::XMLElement* homePosition = xmlDoc->NewElement("HomePosition");
	rootElement->InsertEndChild(homePosition);

	InsertChildElement(xmlDoc, homePosition, "Enabled", xmlParam.controlParam.homePositionParam.enabled);
	InsertChildElement(xmlDoc, homePosition, "ResetTime", xmlParam.controlParam.homePositionParam.resetTime);
	InsertChildElement(xmlDoc, homePosition, "PresetIndex", xmlParam.controlParam.homePositionParam.presetIndex);
	return 0;
}

int SipXmlBodyParser::ControlDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Control";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	tinyxml2::XMLElement* basicParam = xmlDoc->NewElement("BasicParam");
	rootElement->InsertEndChild(basicParam);

	// �豸����
	InsertChildElement(xmlDoc, basicParam, "Name", xmlParam.controlParam.deviceConfigParam.name);
	InsertChildElement(xmlDoc, basicParam, "Expiration", xmlParam.controlParam.deviceConfigParam.expiration);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatInterval", xmlParam.controlParam.deviceConfigParam.heartBeatInterval);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatCount", xmlParam.controlParam.deviceConfigParam.heartBeatCount);

	return 0;
}

int SipXmlBodyParser::QueryDeviceStatus(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "DeviceStatus";

	return 0;
}

int SipXmlBodyParser::QueryCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "StartTime", xmlParam.queryParam.queryCatalogParam.startTime);
	InsertChildElement(xmlDoc, rootElement, "EndTime", xmlParam.queryParam.queryCatalogParam.endTime);

	return 0;
}

int SipXmlBodyParser::QueryDeviceInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "DeviceInfo";

	return 0;
}

int SipXmlBodyParser::QueryRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "RecordInfo";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	QueryRecordInfoParam &queryRecordInfoParam = xmlParam.queryParam.recordInfoParam;// �ļ�Ŀ¼��������
	InsertChildElement(xmlDoc, rootElement, "StartTime", queryRecordInfoParam.startTime);
	InsertChildElement(xmlDoc, rootElement, "EndTime", queryRecordInfoParam.endTime);
	InsertChildElement(xmlDoc, rootElement, "FilePath", queryRecordInfoParam.filePath);
	InsertChildElement(xmlDoc, rootElement, "Address", queryRecordInfoParam.address);
	InsertChildElement(xmlDoc, rootElement, "Secrdcy", queryRecordInfoParam.secrecy);
	InsertChildElement(xmlDoc, rootElement, "Type", queryRecordInfoParam.type);
	InsertChildElement(xmlDoc, rootElement, "RecorderID", queryRecordInfoParam.recorderID);

	return 0;
}

int SipXmlBodyParser::QueryAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "Alarm";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	QueryAlarmParam &queryAlarmParam = xmlParam.queryParam.queryAlarmParam;		// ������ѯ
	InsertChildElement(xmlDoc, rootElement, "StartAlarmPriority", queryAlarmParam.startAlarmPriority);
	InsertChildElement(xmlDoc, rootElement, "EndAlarmProitity", queryAlarmParam.endAlarmPriority);
	InsertChildElement(xmlDoc, rootElement, "AlarmMethod", queryAlarmParam.alarmMethod);
	InsertChildElement(xmlDoc, rootElement, "AlarmType", queryAlarmParam.alarmType);
	InsertChildElement(xmlDoc, rootElement, "StartAlarmTime", queryAlarmParam.startAlarmTime);
	InsertChildElement(xmlDoc, rootElement, "EndAlarmTime", queryAlarmParam.endAlarmTime);

	return 0;
}

int SipXmlBodyParser::QueryConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "ConfigDownload";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "ConfigType", xmlParam.queryParam.configType);

	return 0;
}

int SipXmlBodyParser::QueryPreset(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "PresetQuery";

	return 0;
}

int SipXmlBodyParser::QueryMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam)
{
	xmlParam.reqRespType = "Query";
	xmlParam.cmdType = "MobilePosition";

	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
	InsertChildElement(xmlDoc, rootElement, "Interval", xmlParam.queryParam.mobileInterval);

	return 0;
}

int SipXmlBodyParser::NotifyKeepalive(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Keepalive";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
	InsertChildElement(xmlDoc, rootElement, "Status", "OK");
	xmlParam.notifyParam.keepalive.info;

	return 0;
}

int SipXmlBodyParser::NotifyAlarm(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Alarm";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	NotifyAlarmParam &alarm = xmlParam.notifyParam.alarm;			// ����֪ͨ// ��ӱ���֪ͨ��Ϣ�����ֶ�																	
	InsertChildElement(xmlDoc, rootElement, "AlarmPriority", alarm.alarmPriority);
	InsertChildElement(xmlDoc, rootElement, "AlarmMethod", alarm.alarmTime);
	InsertChildElement(xmlDoc, rootElement, "AlarmTime", alarm.alarmTime);
	InsertChildElement(xmlDoc, rootElement, "AlarmDescription", alarm.alarmDescription);
	InsertChildElement(xmlDoc, rootElement, "Longitude", alarm.longitude);
	InsertChildElement(xmlDoc, rootElement, "Latitude", alarm.latitude);

	alarm.alarmInfo;

	return 0;
}

int SipXmlBodyParser::NotifyMediaStutas(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "MediaStatus";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
	InsertChildElement(xmlDoc, rootElement, "NotifyType", xmlParam.notifyParam.mediaStatus); // 121 ��ʾ��ʷý���ļ����ͽ���
	return 0;
}

int SipXmlBodyParser::NotifyBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Broadcast";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
																	
	rootElement->DeleteChild(rootElement->FirstChildElement("DeviceID"));// ��Ҫɾ��deviceID�ֶ�
	InsertChildElement(xmlDoc, rootElement, "SourceID", xmlParam.notifyParam.broadcast.sourceID);
	InsertChildElement(xmlDoc, rootElement, "TargetID", xmlParam.notifyParam.broadcast.targetID);
	return 0;
}

int SipXmlBodyParser::NotifyMobilePosition(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "MobilePosition";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	NotifyMobilePositionParam &mobilePosition = xmlParam.notifyParam.mobilePosition; // �ƶ��豸λ������֪ͨ
	InsertChildElement(xmlDoc, rootElement, "Time", mobilePosition.time);
	InsertChildElement(xmlDoc, rootElement, "Longitude", mobilePosition.longtitude);
	InsertChildElement(xmlDoc, rootElement, "Latitude", mobilePosition.latitude);
	InsertChildElement(xmlDoc, rootElement, "Speed", mobilePosition.speed);
	InsertChildElement(xmlDoc, rootElement, "Direction", mobilePosition.direction);
	InsertChildElement(xmlDoc, rootElement, "Altitude", mobilePosition.altitude);

	return 0;
}

int SipXmlBodyParser::NotifyCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Notify";
	xmlParam.cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Status", "OK");
	InsertChildElement(xmlDoc, rootElement, "SumNum", (int)xmlParam.notifyParam.deviceList.size());

	tinyxml2::XMLElement* deviceList = xmlDoc->NewElement("DeviceList");
	deviceList->SetAttribute("Num", (int)xmlParam.notifyParam.deviceList.size());
	rootElement->InsertEndChild(deviceList);
	InsertChildElementWithAttr(xmlDoc, rootElement, "DeviceList", (int)xmlParam.notifyParam.deviceList.size());

	for (int i = 0; i < xmlParam.notifyParam.deviceList.size(); i++)
	{
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		deviceList->InsertEndChild(item);

		InsertChildElement(xmlDoc, item, "DeviceID", xmlParam.notifyParam.deviceList[i].deviceID);
		InsertChildElement(xmlDoc, item, "Event", xmlParam.notifyParam.deviceList[i].event);
	}

	return 0;
}

int SipXmlBodyParser::ResponseDeviceControl(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	return 0;
}

int SipXmlBodyParser::ResponseAlarmNotify(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "Alarm";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	return 0;
}

int SipXmlBodyParser::ResponseCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "Catalog";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
	
	InsertChildElement(xmlDoc, rootElement, "SumNum", xmlParam.sumNum);

	// ���DeviceList�ֶ�
	tinyxml2::XMLElement* deviceList = xmlDoc->NewElement("DeviceList");
	rootElement->InsertEndChild(deviceList);
	deviceList->SetAttribute("Num", (int)xmlParam.responseParam.catalogParam.size());
		
	for (CatalogItemAndInfo catalogItemInfo : xmlParam.responseParam.catalogParam)
	{
		// �����Դ��28181ͨ����Ϣ
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		deviceList->InsertEndChild(item);

		// ��˳��д��
		if (catalogItemInfo.resItem.valuesMap.size())
		{
			std::string deviceID = catalogItemInfo.resItem.valuesMap["DeviceID"];
			if (deviceID.size() <= 10) // ��������
			{
				InsertChildElement(xmlDoc, item, "DeviceID", catalogItemInfo.resItem.valuesMap["DeviceID"]);
				InsertChildElement(xmlDoc, item, "Name", catalogItemInfo.resItem.valuesMap["Name"]);
				InsertChildElement(xmlDoc, item, "ParentID", catalogItemInfo.resItem.valuesMap["ParentID"]);
				InsertChildElement(xmlDoc, item, "BusinessGroupID", catalogItemInfo.resItem.valuesMap["BusinessGroupID"]);
			} 
			else
			{
				int type = 0;
				SipUtilities::Str2Int(deviceID.substr(10, 3), type);// ������֯��ҵ�����
				if (type == 215 || type == 216)
				{
					InsertChildElement(xmlDoc, item, "DeviceID", catalogItemInfo.resItem.valuesMap["DeviceID"]);
					InsertChildElement(xmlDoc, item, "Name", catalogItemInfo.resItem.valuesMap["Name"]);
					InsertChildElement(xmlDoc, item, "ParentID", catalogItemInfo.resItem.valuesMap["ParentID"]);
					InsertChildElement(xmlDoc, item, "BusinessGroupID", catalogItemInfo.resItem.valuesMap["BusinessGroupID"]);
				} 
				else
				{
					InsertChildElement(xmlDoc, item, "DeviceID", catalogItemInfo.resItem.valuesMap["DeviceID"]);
					InsertChildElement(xmlDoc, item, "Name", catalogItemInfo.resItem.valuesMap["Name"]);
					InsertChildElement(xmlDoc, item, "Manufacturer", catalogItemInfo.resItem.valuesMap["Manufacturer"]);
					InsertChildElement(xmlDoc, item, "Model", catalogItemInfo.resItem.valuesMap["Model"]);
					InsertChildElement(xmlDoc, item, "Owner", catalogItemInfo.resItem.valuesMap["Owner"]);
					InsertChildElement(xmlDoc, item, "CivilCode", catalogItemInfo.resItem.valuesMap["CivilCode"]);
					//InsertChildElement(xmlDoc, item, "Block", catalogItemInfo.resItem.valuesMap["Block"]);
					InsertChildElement(xmlDoc, item, "Address", catalogItemInfo.resItem.valuesMap["Address"]);
					InsertChildElement(xmlDoc, item, "Parental", catalogItemInfo.resItem.valuesMap["Parental"]);
					InsertChildElement(xmlDoc, item, "ParentID", catalogItemInfo.resItem.valuesMap["ParentID"]);
					//InsertChildElement(xmlDoc, item, "SafetyWay", catalogItemInfo.resItem.valuesMap["SafetyWay"]);
					InsertChildElement(xmlDoc, item, "RegisterWay", catalogItemInfo.resItem.valuesMap["RegisterWay"]);
					//InsertChildElement(xmlDoc, item, "CertNum", catalogItemInfo.resItem.valuesMap["CertNum"]);
					InsertChildElement(xmlDoc, item, "Certifiable", catalogItemInfo.resItem.valuesMap["Certifiable"]);
					InsertChildElement(xmlDoc, item, "ErrCode", catalogItemInfo.resItem.valuesMap["ErrCode"]);
					InsertChildElement(xmlDoc, item, "EndTime", catalogItemInfo.resItem.valuesMap["EndTime"]);
					InsertChildElement(xmlDoc, item, "Secrecy", catalogItemInfo.resItem.valuesMap["Secrecy"]);
					//InsertChildElement(xmlDoc, item, "IPAddress", catalogItemInfo.resItem.valuesMap["IPAddress"]);
					//InsertChildElement(xmlDoc, item, "Port", catalogItemInfo.resItem.valuesMap["Port"]);
					//InsertChildElement(xmlDoc, item, "Password", catalogItemInfo.resItem.valuesMap["Password"]);
					InsertChildElement(xmlDoc, item, "Status", catalogItemInfo.resItem.valuesMap["Status"]);
					//InsertChildElement(xmlDoc, item, "Longitude", catalogItemInfo.resItem.valuesMap["Longitude"]);
					//InsertChildElement(xmlDoc, item, "Latitude", catalogItemInfo.resItem.valuesMap["Latitude"]);

					if (xmlParam.isMust) // Ϊ��ʱ��tcp����ʱ�����ϼ��ظ�������Ϣ
					{
						InsertChildElement(xmlDoc, item, "Block", catalogItemInfo.resItem.valuesMap["Block"]);
						InsertChildElement(xmlDoc, item, "SafetyWay", catalogItemInfo.resItem.valuesMap["SafetyWay"]);
						InsertChildElement(xmlDoc, item, "CertNum", catalogItemInfo.resItem.valuesMap["CertNum"]);
						InsertChildElement(xmlDoc, item, "IPAddress", catalogItemInfo.resItem.valuesMap["IPAddress"]);
						InsertChildElement(xmlDoc, item, "Port", catalogItemInfo.resItem.valuesMap["Port"]);
						InsertChildElement(xmlDoc, item, "Password", catalogItemInfo.resItem.valuesMap["Password"]);						
						InsertChildElement(xmlDoc, item, "Longitude", catalogItemInfo.resItem.valuesMap["Longitude"]);
						InsertChildElement(xmlDoc, item, "Latitude", catalogItemInfo.resItem.valuesMap["Latitude"]);
					}
				}	
			} 
		}	

		// ���28181�������չ��Ϣ
		if (catalogItemInfo.cameraInfo.valuesMap.size() && xmlParam.isMust)
		{
			tinyxml2::XMLElement* info = xmlDoc->NewElement("Info");
			item->InsertEndChild(info);

			InsertChildElement(xmlDoc, info, "PTZType", catalogItemInfo.cameraInfo.valuesMap["PTZType"]);
			InsertChildElement(xmlDoc, info, "PositionType", catalogItemInfo.cameraInfo.valuesMap["PositionType"]);
			InsertChildElement(xmlDoc, info, "RoomType", catalogItemInfo.cameraInfo.valuesMap["RoomType"]);
			InsertChildElement(xmlDoc, info, "UseType", catalogItemInfo.cameraInfo.valuesMap["UseType"]);
			InsertChildElement(xmlDoc, info, "SupplyLightType", catalogItemInfo.cameraInfo.valuesMap["SupplyLightType"]);
			InsertChildElement(xmlDoc, info, "DirectionType", catalogItemInfo.cameraInfo.valuesMap["DirectionType"]);
			InsertChildElement(xmlDoc, info, "Resolution", catalogItemInfo.cameraInfo.valuesMap["Resolution"]);
			InsertChildElement(xmlDoc, info, "BusinessGroupID", catalogItemInfo.cameraInfo.valuesMap["BusinessGroupID"]);
			InsertChildElement(xmlDoc, info, "DownloadSpeed", catalogItemInfo.cameraInfo.valuesMap["DownloadSpeed"]);
			InsertChildElement(xmlDoc, info, "SVCSpaceSupportMode", catalogItemInfo.cameraInfo.valuesMap["SVCSpaceSupportMode"]);
			InsertChildElement(xmlDoc, info, "SVCTimeSupportMode", catalogItemInfo.cameraInfo.valuesMap["SVCTimeSupportMode"]);
		}
	}

	return 0;
}

int SipXmlBodyParser::ResponseRcvCatalogQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);

	return 0;
}

int SipXmlBodyParser::ResponseRcvCatalog(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceControl";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);

	return 0;
}

int SipXmlBodyParser::ResponseDeviceInfoQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceInfo";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	RespDeviceInfoParam &devInfo = xmlParam.responseParam.devInfoParam;		// �豸��Ϣ��ѯӦ��
	InsertChildElement(xmlDoc, rootElement, "DeviceName", devInfo.deviceName);
	InsertChildElement(xmlDoc, rootElement, "Result", devInfo.result);
	InsertChildElement(xmlDoc, rootElement, "Manufacturter", devInfo.manufacturer);
	InsertChildElement(xmlDoc, rootElement, "Model", devInfo.model);
	InsertChildElement(xmlDoc, rootElement, "Firmware", devInfo.firmware);
	InsertChildElement(xmlDoc, rootElement, "Channel", devInfo.channel);

	return 0;
}

int SipXmlBodyParser::ResponseDeviceStatusQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceStatus";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	RespDeviceStatusParam &devStatus = xmlParam.responseParam.devStatusParam; // �豸״̬��Ϣ��ѯӦ��
	InsertChildElement(xmlDoc, rootElement, "Result", devStatus.result);
	InsertChildElement(xmlDoc, rootElement, "Online", devStatus.online);
	InsertChildElement(xmlDoc, rootElement, "Status", devStatus.status);
	if (devStatus.status == "ERROR")
	{
		InsertChildElement(xmlDoc, rootElement, "Reason", devStatus.reason);
	}
	InsertChildElement(xmlDoc, rootElement, "Encode", devStatus.encode);
	InsertChildElement(xmlDoc, rootElement, "Record", devStatus.record);
	InsertChildElement(xmlDoc, rootElement, "DeviceTime", devStatus.deviceTime);

	// ����״̬�б�
	if (devStatus.alarmStatus.size())
	{
		tinyxml2::XMLElement* alarmstatus = xmlDoc->NewElement("Alarmstatus");
		alarmstatus->SetAttribute("num", (int)devStatus.alarmStatus.size());
		for (size_t i = 0; i < devStatus.alarmStatus.size(); i++)
		{
			tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
			InsertChildElement(xmlDoc, item, "DeviceID", devStatus.alarmStatus[i].deviceID);
			InsertChildElement(xmlDoc, item, "DutyStatus", devStatus.alarmStatus[i].dutyStatus);
		}
	}

	// info��չ��Ϣ

	return 0;
}

int SipXmlBodyParser::ResponseRecordInfo(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "RecordInfo";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��
	
	RespRecordInfoParam &recordInfo = xmlParam.responseParam.recordInfoParam;// �ļ�Ŀ¼����Ӧ��
	InsertChildElement(xmlDoc, rootElement, "Name", recordInfo.name);
	InsertChildElement(xmlDoc, rootElement, "SumNum", recordInfo.sumNum);
	InsertChildElementWithAttr(xmlDoc, rootElement, "RecordList", (int)recordInfo.recordInfoItems.size());

	tinyxml2::XMLElement* recordList = rootElement->FirstChildElement("RecordList");
	for (size_t i = 0; i < recordInfo.recordInfoItems.size(); i++)
	{
		// ����Item�ֶ�
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		recordList->InsertEndChild(item);

		// �������DeviceID���������
		InsertChildElement(xmlDoc, item, "DeviceID", xmlParam.deviceID);
		InsertChildElement(xmlDoc, item, "Name", recordInfo.name);

		// ������ʷ�ļ���Ϣ
		InsertChildElement(xmlDoc, item, "StartTime", recordInfo.recordInfoItems[i].startTime);
		InsertChildElement(xmlDoc, item, "EndTime", recordInfo.recordInfoItems[i].endTime);
		InsertChildElement(xmlDoc, item, "FilePath", recordInfo.recordInfoItems[i].filePath);
		InsertChildElement(xmlDoc, item, "Address", recordInfo.recordInfoItems[i].address);
		InsertChildElement(xmlDoc, item, "Secrdcy", recordInfo.recordInfoItems[i].secrecy);
		InsertChildElement(xmlDoc, item, "Type", recordInfo.recordInfoItems[i].type);
		InsertChildElement(xmlDoc, item, "RecorderID", recordInfo.recordInfoItems[i].recorderID);
	}

	return 0;
}

int SipXmlBodyParser::ResponseDeviceConfig(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "DeviceConfig";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	
	return 0;
}

int SipXmlBodyParser::ResponseConfigDownload(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "ConfigDownload";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	RespConfigDownloadParam &configDownload = xmlParam.responseParam.configDownloadParam;// �豸���ò�ѯӦ��
	InsertChildElement(xmlDoc, rootElement, "Result", configDownload.result);
	tinyxml2::XMLElement* basicParam = xmlDoc->NewElement("BasicParam");
	rootElement->InsertEndChild(basicParam);
	InsertChildElement(xmlDoc, basicParam, "Name", configDownload.name);
	InsertChildElement(xmlDoc, basicParam, "Expiration", configDownload.expiration);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatInterval", configDownload.heartBeatInterval);
	InsertChildElement(xmlDoc, basicParam, "HeartBeatCount", configDownload.heartBeatCount);
	InsertChildElement(xmlDoc, basicParam, "PositionCapability", configDownload.positionCapaility);
	InsertChildElement(xmlDoc, basicParam, "Longitude", configDownload.Longitude);
	InsertChildElement(xmlDoc, basicParam, "Latitude", configDownload.Latitude);

	return 0;
}

int SipXmlBodyParser::ResponsePresetQuery(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "PresetQuery";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��


	RespPresetParams &presetList = xmlParam.responseParam.presetList; // �豸Ԥ��λ��ѯӦ��
	tinyxml2::XMLElement* presetListEle = xmlDoc->NewElement("PresetList");
	rootElement->InsertEndChild(presetListEle);
	presetListEle->SetAttribute("Num", (int)presetList.size());

	for (size_t i = 0; i < presetList.size(); i++)
	{
		tinyxml2::XMLElement* item = xmlDoc->NewElement("Item");
		rootElement->InsertEndChild(item);

		InsertChildElement(xmlDoc, item, "PresetID", presetList[i].presetID);
		InsertChildElement(xmlDoc, item, "presetName", presetList[i].presetName);
	}
	return 0;
}

int SipXmlBodyParser::ResponseBroadcast(tinyxml2::XMLDocument *& xmlDoc, XmlCmdParam & xmlParam)
{
	xmlParam.reqRespType = "Response";
	xmlParam.cmdType = "Broadcast";
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();// ��ȡ���ڵ�ָ��

	InsertChildElement(xmlDoc, rootElement, "Result", xmlParam.responseParam.result);
	return 0;
}

int SipXmlBodyParser::ParserControl(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = -1;

	// ��ȡ�������͡�sn�ͻ�ȡdeviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn) ||
		GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "DeviceControl") // �豸����
		{
			result = ParserDeviceControl(xmlRootElement, xmlParam);
		}
		else if (xmlParam.cmdType == "DeviceConfig")// �豸����
		{
			result = ParserDeviceConfig(xmlRootElement, xmlParam);
		}
	}
	return result;
}

int SipXmlBodyParser::ParserDeviceControl(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// �ж���������
	if (!GetChildElementValue(xmlRootElement, "PTZCmd", xmlParam.controlParam.ptzParam.ptzCmdValue)) // Ptz��������
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_PTZ;
		ParserControlCmd(xmlRootElement, xmlParam);
	}
	else if (!GetChildElementValue(xmlRootElement, "TeleBoot", xmlParam.controlParam.teleBootParam)) // Զ����������
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_TELE_BOOT;
	}
	else if (!GetChildElementValue(xmlRootElement, "RecordCmd", xmlParam.controlParam.recordType)) // ¼���������
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_RECORD;
	}
	else if (!GetChildElementValue(xmlRootElement, "GuardCmd", xmlParam.controlParam.guardType)) // ����/��������
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_GUARD;
	}
	else if (xmlRootElement->FirstChildElement("AlarmCmd")) // ������λ����
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_ALARM;

		// ������λ������չ��
		tinyxml2::XMLElement* info = xmlRootElement->FirstChildElement("Info");
		if (info)
		{
			GetChildElementValue(info, "AlarmMethod", xmlParam.controlParam.alarmParam.alarmMethod);
			GetChildElementValue(info, "AlarmType", xmlParam.controlParam.alarmParam.alarmType);
		}
	}
	else if (!GetChildElementValue(xmlRootElement, "IFameCmd", xmlParam.controlParam.iFame)) // ǿ�ƹؼ�֡����
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_IFAME;
	}
	else if (xmlRootElement->FirstChildElement("HomePosition")) // ����λ��������
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_HOME_POSITION;

		// ����λ����������չ��
		tinyxml2::XMLElement* homePosition = xmlRootElement->FirstChildElement("HomePosition");
		GetChildElementValue(homePosition, "Enabled", xmlParam.controlParam.homePositionParam.enabled);
		GetChildElementValue(homePosition, "ResetTime", xmlParam.controlParam.homePositionParam.resetTime);
		GetChildElementValue(homePosition, "PresetIndex", xmlParam.controlParam.homePositionParam.presetIndex);
	}
	else if (xmlRootElement->FirstChildElement("DragZoomIn") || xmlRootElement->FirstChildElement("DragZoomOut")) // ����λ��������
	{		
		xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::CONTROL_DRAG_ZOOM;

		// ����Ŵ�
		tinyxml2::XMLElement* dragZoom = xmlRootElement->FirstChildElement("DragZoomIn");
		if (dragZoom)
		{
			xmlParam.controlParam.dragZoom.isZoomOut = false;
		} 
		else
		{
			dragZoom = xmlRootElement->FirstChildElement("DragZoomOut");
			xmlParam.controlParam.dragZoom.isZoomOut = true;
		}

		GetChildElementValue(dragZoom, "Length", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "Width", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "MidPointX", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "MidPointY", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "LengthX", xmlParam.controlParam.dragZoom.length);
		GetChildElementValue(dragZoom, "LengthY", xmlParam.controlParam.dragZoom.length);
	}
	else
	{
		xmlParam.manscdpType = MESSAGE_MANSCDP_ERROR;
		result = -1;
	}
	return result;
}

int SipXmlBodyParser::ParserDeviceConfig(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	xmlParam.manscdpType = CONTROL_DEVICE_CONFIG;

	// �������ò�����ȡ
	tinyxml2::XMLElement* basicParam = xmlRootElement->FirstChildElement("BasicParam");
	if (basicParam != nullptr)
	{
		GetChildElementValue(basicParam, "Name", xmlParam.controlParam.deviceConfigParam.name);
		GetChildElementValue(basicParam, "Expiration", xmlParam.controlParam.deviceConfigParam.expiration);
		GetChildElementValue(basicParam, "HeartBeatInterval", xmlParam.controlParam.deviceConfigParam.heartBeatInterval);
		GetChildElementValue(basicParam, "HeartBeatCount", xmlParam.controlParam.deviceConfigParam.heartBeatCount);
	}
	return 0;
}

int SipXmlBodyParser::ParserQuery(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// ��ȡ�������͡�sn�ͻ�ȡdeviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn) ||
		GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "DeviceStatus") // �豸״̬��ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_STATUS;
		}
		else if (xmlParam.cmdType == "Catalog") // �豸Ŀ¼��ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_CATALOG;
			GetChildElementValue(xmlRootElement, "StartTime", xmlParam.queryParam.queryCatalogParam.startTime);
			GetChildElementValue(xmlRootElement, "EndTime", xmlParam.queryParam.queryCatalogParam.endTime);
		}
		else if (xmlParam.cmdType == "DeviceInfo") // �豸��Ϣ��ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_INFO;
		}
		else if (xmlParam.cmdType == "RecordInfo") // �ļ�Ŀ¼��������
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_RECORD_INFO;

			if (GetChildElementValue(xmlRootElement, "StartTime", xmlParam.queryParam.recordInfoParam.startTime) ||
				GetChildElementValue(xmlRootElement, "EndTime", xmlParam.queryParam.recordInfoParam.endTime))
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "FilePath", xmlParam.queryParam.recordInfoParam.filePath);
				GetChildElementValue(xmlRootElement, "Address", xmlParam.queryParam.recordInfoParam.address);
				GetChildElementValue(xmlRootElement, "Secrecy", xmlParam.queryParam.recordInfoParam.secrecy);
				GetChildElementValue(xmlRootElement, "Type", xmlParam.queryParam.recordInfoParam.type);
				GetChildElementValue(xmlRootElement, "RecorderID", xmlParam.queryParam.recordInfoParam.recorderID);
			}
		}
		else if (xmlParam.cmdType == "Alarm") // ������ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_ALARM;

			GetChildElementValue(xmlRootElement, "StartAlarmPriority", xmlParam.queryParam.queryAlarmParam.startAlarmPriority);
			GetChildElementValue(xmlRootElement, "EndAlarmPriority", xmlParam.queryParam.queryAlarmParam.endAlarmPriority);
			GetChildElementValue(xmlRootElement, "AlarmMethod", xmlParam.queryParam.queryAlarmParam.alarmMethod);
			GetChildElementValue(xmlRootElement, "AlarmType", xmlParam.queryParam.queryAlarmParam.alarmType);
			GetChildElementValue(xmlRootElement, "StartAlarmTime", xmlParam.queryParam.queryAlarmParam.startAlarmTime);
			GetChildElementValue(xmlRootElement, "EndAlarmTime", xmlParam.queryParam.queryAlarmParam.endAlarmTime);
		}
		else if (xmlParam.cmdType == "ConfigDownload") // �豸���ò�ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_CONFIG_DOWNLOAD;
			if (GetChildElementValue(xmlRootElement, "ConfigType", xmlParam.queryParam.configType))
			{
				result = -1;
			}
		}
		else if (xmlParam.cmdType == "PresetQuery") // �豸Ԥ��λ��ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_PRESET_QUERY;
		}
		else if (xmlParam.cmdType == "MobilePosition") // �ƶ��豸λ�����ݲ�ѯ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_MOBILE_POSITION;
			GetChildElementValue(xmlRootElement, "Interval", xmlParam.queryParam.mobileInterval);
		}
		else
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::MESSAGE_MANSCDP_ERROR;
			result = -1;
		}
	}
	return result;
}

int SipXmlBodyParser::ParserNotify(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// ��ȡ�������͡�sn�ͻ�ȡdeviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "Broadcast") // �����㲥֪ͨ
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_BROADCAST;
			if (GetChildElementValue(xmlRootElement, "SourceID", xmlParam.notifyParam.broadcast.sourceID) ||
				GetChildElementValue(xmlRootElement, "TargetID", xmlParam.notifyParam.broadcast.targetID))
			{
				result = -1;
			}
		}
		else if (!GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
		{
			if (xmlParam.cmdType == "Keepalive") // ״̬��Ϣ����
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_KEEPALIVE;
				// Info��չ��ʱδʹ��
			}
			else if (xmlParam.cmdType == "Alarm") // ����֪ͨ
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_ALARM;
				GetChildElementValue(xmlRootElement, "AlarmPriority", xmlParam.notifyParam.alarm.alarmPriority);
				GetChildElementValue(xmlRootElement, "AlarmMethod", xmlParam.notifyParam.alarm.alarmMethod);
				GetChildElementValue(xmlRootElement, "AlarmTime", xmlParam.notifyParam.alarm.alarmTime);
				GetChildElementValue(xmlRootElement, "AlarmType", xmlParam.notifyParam.alarm.alarmType);
				GetChildElementValue(xmlRootElement, "AlarmTypeParam", xmlParam.notifyParam.alarm.alarmTypeParam);
				GetChildElementValue(xmlRootElement, "AlarmDescription", xmlParam.notifyParam.alarm.alarmDescription);
				GetChildElementValue(xmlRootElement, "AlarmLongitude", xmlParam.notifyParam.alarm.longitude);
				GetChildElementValue(xmlRootElement, "AlarmLatitude", xmlParam.notifyParam.alarm.latitude);
				GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.notifyParam.alarm.deviceID);

				tinyxml2::XMLElement* firstInfo = xmlRootElement->FirstChildElement("Info");
				NotifyAlarmInfo alarmInfo;
				while (firstInfo)
				{
					GetChildElementValue(firstInfo, "AlarmType", alarmInfo.alarmType);

					tinyxml2::XMLElement* alarmTypeParam = firstInfo->FirstChildElement("AlarmTypeParam");
					if (alarmTypeParam)
					{
						GetChildElementValue(firstInfo, "EventType", alarmInfo.eventType);
					}
					xmlParam.notifyParam.alarm.alarmInfo.push_back(alarmInfo);
					firstInfo = firstInfo->NextSiblingElement();
				}
			}
			else if (xmlParam.cmdType == "MediaStatus") // ý��֪ͨ,
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_MEDIA_STATUS;
				GetChildElementValue(xmlRootElement, "NotifyType", xmlParam.notifyParam.mediaStatus);
			}
			else if (xmlParam.cmdType == "MobilePosition") // �ƶ��豸λ������֪ͨ
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_MOBILE_POSITION;
				if (GetChildElementValue(xmlRootElement, "Time", xmlParam.notifyParam.mobilePosition.time) ||
					GetChildElementValue(xmlRootElement, "Longitude", xmlParam.notifyParam.mobilePosition.longtitude) ||
					GetChildElementValue(xmlRootElement, "Latitude", xmlParam.notifyParam.mobilePosition.latitude))
				{
					result = -1;
				}
				else
				{
					GetChildElementValue(xmlRootElement, "Speed", xmlParam.notifyParam.mobilePosition.speed);
					GetChildElementValue(xmlRootElement, "Direction", xmlParam.notifyParam.mobilePosition.direction);
					GetChildElementValue(xmlRootElement, "Altitude", xmlParam.notifyParam.mobilePosition.altitude);
				}
			}
			else if (xmlParam.cmdType == "Catalog")
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::NOTIFY_CATALOG;

				tinyxml2::XMLElement* deviceList = xmlRootElement->FirstChildElement("DeviceList");
				if (deviceList)
				{
					NotifyCatalogInfo catalogInfo;
					tinyxml2::XMLElement* firstInfo = deviceList->FirstChildElement("Item");
					while (firstInfo)
					{
						GetChildElementValue(firstInfo, "DeviceID", catalogInfo.deviceID);
						GetChildElementValue(firstInfo, "Event", catalogInfo.event);
						xmlParam.notifyParam.deviceList.push_back(catalogInfo);
						firstInfo = firstInfo->NextSiblingElement();
					}
				}
			}
		}
		else
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::MESSAGE_MANSCDP_ERROR;
			result = -1;
		}
	}
	return result;
}

int SipXmlBodyParser::ParserResponse(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// ��ȡ�������͡�sn�ͻ�ȡdeviceID
	if (GetChildElementValue(xmlRootElement, "CmdType", xmlParam.cmdType) ||
		GetChildElementValue(xmlRootElement, "SN", xmlParam.sn) ||
		GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		if (xmlParam.cmdType == "DeviceControl")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONTROL;
			GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result);
		}
		else if (xmlParam.cmdType == "Alarm")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_ALARM_NOTIFY;
			GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result);
		}
		else if (xmlParam.cmdType == "Catalog") // �豸Ŀ¼��Ϣ��ѯӦ��Ŀ¼��Ϣ��ѯ�յ�Ӧ��Ŀ¼�յ�Ӧ��
		{
			if (xmlParam.deviceID == xmlParam.localSipID)// ����Ŀ¼���յ��¼��ظ���200OK
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_RECEIVED;
			}
			else
			{
				xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CATALOG_QUERY;
				ParserResponseCatalog(xmlRootElement, xmlParam);
			}
		}
		else if (xmlParam.cmdType == "DeviceInfo")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_INFO_QUERY;

			RespDeviceInfoParam &devInfo = xmlParam.responseParam.devInfoParam;		// �豸��Ϣ��ѯӦ��
			if (GetChildElementValue(xmlRootElement, "Result", devInfo.result))
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "DeviceName", devInfo.deviceName);
				GetChildElementValue(xmlRootElement, "Manufacturer", devInfo.manufacturer);
				GetChildElementValue(xmlRootElement, "Model", devInfo.model);
				GetChildElementValue(xmlRootElement, "Fireware", devInfo.firmware);
				GetChildElementValue(xmlRootElement, "Channel", devInfo.channel);
				GetChildElementValue(xmlRootElement, "Info", devInfo.info);
			}

		}
		else if (xmlParam.cmdType == "DeviceStatus")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_STATUS_QUERY;

			RespDeviceStatusParam &devStatus = xmlParam.responseParam.devStatusParam; // �豸״̬��Ϣ��ѯӦ��
			if (GetChildElementValue(xmlRootElement, "Result", devStatus.result) ||
				GetChildElementValue(xmlRootElement, "Online", devStatus.online) ||
				GetChildElementValue(xmlRootElement, "Status", devStatus.status))
			{
				result = -1;
			}
			else
			{
				GetChildElementValue(xmlRootElement, "Reason", devStatus.reason);
				GetChildElementValue(xmlRootElement, "Encode", devStatus.encode);
				GetChildElementValue(xmlRootElement, "Record", devStatus.record);
				GetChildElementValue(xmlRootElement, "DeviceTime", devStatus.deviceTime);

				tinyxml2::XMLElement* alarmstatusEle = xmlRootElement->FirstChildElement("Alarmstatus");
				if (alarmstatusEle)
				{
					tinyxml2::XMLElement* firstItem = alarmstatusEle->FirstChildElement("Item");
					while (firstItem)
					{
						Alarmstatus alarmStaues;
						GetChildElementValue(firstItem, "DeviceID", alarmStaues.deviceID);
						GetChildElementValue(firstItem, "DutyStatus", alarmStaues.dutyStatus);
						devStatus.alarmStatus.push_back(alarmStaues);
						firstItem = firstItem->NextSiblingElement();
					}
				}
			}
		}
		else if (xmlParam.cmdType == "RecordInfo")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DOCUMENT_RECORD_INFO;

			RespRecordInfoParam &recordInfo = xmlParam.responseParam.recordInfoParam;		// �ļ�Ŀ¼����Ӧ��	

			//if (GetChildElementValue(xmlRootElement, "Name", recordInfo.name))// ��ѡ�ֶΣ�������֤,2017.09.20������ʱ����
			//{
			//	result = -1;
			//}
			//else
			{
				GetChildElementValue(xmlRootElement, "SumNum", recordInfo.sumNum);
				tinyxml2::XMLElement* recordList = xmlRootElement->FirstChildElement("RecordList");
				if (recordList)
				{
					tinyxml2::XMLElement* recordListFirstItem = recordList->FirstChildElement("Item"); // ��ȡRecordList�ĵ�һ��Item
					RespRecordInfoItem recordInfoItems;
					while (recordListFirstItem)
					{
						GetChildElementValue(recordListFirstItem, "DeviceID", recordInfoItems.deviceID);
						GetChildElementValue(recordListFirstItem, "Name", recordInfoItems.name);
						GetChildElementValue(recordListFirstItem, "FilePath", recordInfoItems.filePath);
						GetChildElementValue(recordListFirstItem, "Address", recordInfoItems.address);
						GetChildElementValue(recordListFirstItem, "StartTime", recordInfoItems.startTime);
						GetChildElementValue(recordListFirstItem, "EndTime", recordInfoItems.endTime);
						GetChildElementValue(recordListFirstItem, "Secrecy", recordInfoItems.secrecy);
						GetChildElementValue(recordListFirstItem, "Type", recordInfoItems.type);
						GetChildElementValue(recordListFirstItem, "RecorderID", recordInfoItems.recorderID);

						// �������������
						recordInfo.recordInfoItems.push_back(recordInfoItems);

						// ָ����һ��Ԫ��
						recordListFirstItem = recordListFirstItem->NextSiblingElement();
					}
				}
			}			
		}
		else if (xmlParam.cmdType == "DeviceConfig")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG;
			if (GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result))
			{
				result = -1;
			}
		}
		else if (xmlParam.cmdType == "ConfigDownload")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG_DOWNLOAD;

			RespConfigDownloadParam &configDownload = xmlParam.responseParam.configDownloadParam;
			if (GetChildElementValue(xmlRootElement, "Result", configDownload.result))
			{
				result = -1;
			}
			else
			{
				tinyxml2::XMLElement* basicParam = xmlRootElement->FirstChildElement("BasicParam");
				GetChildElementValue(basicParam, "Name", configDownload.name);
				GetChildElementValue(basicParam, "Expiration", configDownload.expiration);
				GetChildElementValue(basicParam, "HeartBeatInterval", configDownload.heartBeatInterval);
				GetChildElementValue(basicParam, "HeartBeatCount", configDownload.heartBeatCount);
				GetChildElementValue(basicParam, "PositionCapability", configDownload.positionCapaility);
				GetChildElementValue(basicParam, "Longitude", configDownload.Longitude);
				GetChildElementValue(basicParam, "Latitude", configDownload.Latitude);
			}
		}
		else if (xmlParam.cmdType == "PresetQuery")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_PRESET_QUERY;

			RespPresetParam presetItem;

			tinyxml2::XMLElement* presetList = xmlRootElement->FirstChildElement("PresetList");
			if (presetList)
			{
				tinyxml2::XMLElement* presetListFirstItem = presetList->FirstChildElement("Item"); // ��ȡRecordList�ĵ�һ��Item
				while (presetListFirstItem)
				{
					GetChildElementValue(presetListFirstItem, "PresetID", presetItem.presetID);
					GetChildElementValue(presetListFirstItem, "PresetName", presetItem.presetName);

					// �������������
					xmlParam.responseParam.presetList.push_back(presetItem);

					// ָ����һ��Ԫ��
					presetListFirstItem = presetListFirstItem->NextSiblingElement();
				}
			}
		}
		else if (xmlParam.cmdType == "Broadcast")
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::RESPONSE_BROADCAST;
			GetChildElementValue(xmlRootElement, "Result", xmlParam.responseParam.result);
		}
		else
		{
			xmlParam.manscdpType = MESSAGE_MANSCDP_TYPE::MESSAGE_MANSCDP_ERROR;
			return -1;
		}
	}
	return result;
}

std::string U2G(const char* utf8)
{
	std::string resString = utf8;
#if 0
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	resString = str;
	delete[] str;
#endif
	return resString;
}

int SipXmlBodyParser::ParserResponseCatalog(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int  result = 0;

	tinyxml2::XMLElement* xmlDeviceList = xmlRootElement->FirstChildElement("DeviceList");	// ��ȡĿ¼��Ϣ���б�ָ��
	if (xmlDeviceList == nullptr)
	{
		result = -1;
	}
	else
	{
		CatalogItemAndInfo catalogItemInfo;
		tinyxml2::XMLElement* firstItem = xmlDeviceList->FirstChildElement(); // ��ȡ��һ��itemָ��
		while (firstItem)
		{
			std::string itemDeviceID = "";
			result = GetChildElementValue(firstItem, "DeviceID", itemDeviceID);
			if (result) // ����Ŀ�в�����deviceID��Դֱ����������Ŀ�����ֶ�
			{
				firstItem = firstItem->NextSiblingElement();
				continue;
			}
			else
			{
				// �ж�itemDeviceID����
				if (itemDeviceID.size() <= 10 && itemDeviceID.size() >= 2) // ����������룬���ݲ���tbl_group
				{
					// ��Ҫ��ȡDeviceID, Name			
					std::string itemName = "";
					if (GetChildElementValue(firstItem, "Name", itemName))
					{
						firstItem = firstItem->NextSiblingElement();
						continue;
					}
					itemName = U2G(itemName.c_str());

					// �������ݶ���
					catalogItemInfo.resItem.tableName = TBL_Group;
					catalogItemInfo.resItem.valuesMap["VirtualOrgID"] = itemDeviceID;
					catalogItemInfo.resItem.valuesMap["Name"] = itemName;
					catalogItemInfo.resItem.valuesMap["Type"] = "1";// ��ʾ��������
				}
				else if (itemDeviceID.size() <= 20 && itemDeviceID.size() >= 13)
				{
					int type = 0;
					SipUtilities::Str2Int(itemDeviceID.substr(10, 3), type);
					if (type == 0)
					{
						firstItem = firstItem->NextSiblingElement();
						continue;
					}
					else if (type >= 200 && type <= 211) // ����
					{
						
					} 
					else if (type == 215 || type == 216)// ҵ����顢������֯
					{
						// ��Ҫ��ȡDeviceID��Name��ParentID				
						std::string itemName = "";
						std::string itemParentID = "";
						std::string itemBusinessGroupID = "";	// ������֯����ҵ�����

						if (GetChildElementValue(firstItem, "Name", itemName))
						{
							firstItem = firstItem->NextSiblingElement();
							continue;
						}
						itemName = U2G(itemName.c_str());

						GetChildElementValue(firstItem, "ParentID", itemParentID);
						GetChildElementValue(firstItem, "BusinessGroupID", itemBusinessGroupID);

						// �������ݶ���
						catalogItemInfo.resItem.tableName = TBL_Group;
						catalogItemInfo.resItem.valuesMap["VirtualOrgID"] = itemDeviceID;
						catalogItemInfo.resItem.valuesMap["Name"] = itemName;
						catalogItemInfo.resItem.valuesMap["ParentOrgID"] = itemParentID;
						catalogItemInfo.resItem.valuesMap["BusinessGroupID"] = itemBusinessGroupID;
						if (type == 215)
						{
							catalogItemInfo.resItem.valuesMap["Type"] = "2";// ��ʾҵ�����
						}
						else
						{
							catalogItemInfo.resItem.valuesMap["Type"] = "3";// ��ʾ������֯
						}
						
					} 
					else if (type >= 111 && type <= 199)// �豸
					{
						std::string elementValue = "";

						// �������������Ϣ
						GetChildElementValue(firstItem, "DeviceID", catalogItemInfo.resItem.valuesMap["DeviceID"]);
						GetChildElementValue(firstItem, "Name", catalogItemInfo.resItem.valuesMap["Name"]);	
						catalogItemInfo.resItem.valuesMap["Name"] = U2G(catalogItemInfo.resItem.valuesMap["Name"].c_str());

						GetChildElementValue(firstItem, "Manufacturer", catalogItemInfo.resItem.valuesMap["Manufacturer"]);
						GetChildElementValue(firstItem, "Model", catalogItemInfo.resItem.valuesMap["Model"]);
						GetChildElementValue(firstItem, "Owner", catalogItemInfo.resItem.valuesMap["Owner"]);
						GetChildElementValue(firstItem, "CivilCode", catalogItemInfo.resItem.valuesMap["CivilCode"]);
						GetChildElementValue(firstItem, "Block", catalogItemInfo.resItem.valuesMap["Block"]);
						GetChildElementValue(firstItem, "Address", catalogItemInfo.resItem.valuesMap["Address"]);
						GetChildElementValue(firstItem, "Parental", catalogItemInfo.resItem.valuesMap["Parental"]);
						GetChildElementValue(firstItem, "ParentID", catalogItemInfo.resItem.valuesMap["ParentID"]);
						if (catalogItemInfo.resItem.valuesMap["ParentID"].empty())
						{
							catalogItemInfo.resItem.valuesMap["ParentID"] = xmlParam.deviceID;
						}
						GetChildElementValue(firstItem, "SafetyWay", catalogItemInfo.resItem.valuesMap["SafetyWay"]);
						GetChildElementValue(firstItem, "RegisterWay", catalogItemInfo.resItem.valuesMap["RegisterWay"]);
						GetChildElementValue(firstItem, "CertNum", catalogItemInfo.resItem.valuesMap["CertNum"]);
						GetChildElementValue(firstItem, "Certifiable", catalogItemInfo.resItem.valuesMap["Certifiable"]);
						GetChildElementValue(firstItem, "ErrCode", catalogItemInfo.resItem.valuesMap["ErrCode"]);
						GetChildElementValue(firstItem, "EndTime", catalogItemInfo.resItem.valuesMap["EndTime"]);
						GetChildElementValue(firstItem, "Secrecy", catalogItemInfo.resItem.valuesMap["Secrecy"]);
						GetChildElementValue(firstItem, "IPAddress", catalogItemInfo.resItem.valuesMap["IPAddress"]);
						GetChildElementValue(firstItem, "Port", catalogItemInfo.resItem.valuesMap["Port"]);
						GetChildElementValue(firstItem, "Password", catalogItemInfo.resItem.valuesMap["Password"]);
						GetChildElementValue(firstItem, "Status", catalogItemInfo.resItem.valuesMap["Status"]);
						GetChildElementValue(firstItem, "Longitude", catalogItemInfo.resItem.valuesMap["Longitude"]);
						GetChildElementValue(firstItem, "Latitude", catalogItemInfo.resItem.valuesMap["Latitude"]);

						// ���������չ��Ϣ
						catalogItemInfo.cameraInfo.tableName = TBL_Camera;
						tinyxml2::XMLElement* info = firstItem->FirstChildElement("Info");
						if (info)
						{
							GetChildElementValue(info, "PTZType", catalogItemInfo.cameraInfo.valuesMap["PTZType"]);
							GetChildElementValue(info, "PositionType", catalogItemInfo.cameraInfo.valuesMap["PositionType"]);
							GetChildElementValue(info, "RoomType", catalogItemInfo.cameraInfo.valuesMap["RoomType"]);
							GetChildElementValue(info, "UseType", catalogItemInfo.cameraInfo.valuesMap["UseType"]);
							GetChildElementValue(info, "SupplyLightType", catalogItemInfo.cameraInfo.valuesMap["SupplyLightType"]);
							GetChildElementValue(info, "DirectionType", catalogItemInfo.cameraInfo.valuesMap["DirectionType"]);
							GetChildElementValue(info, "Resolution", catalogItemInfo.cameraInfo.valuesMap["Resolution"]);
							GetChildElementValue(info, "BusinessGroupID", catalogItemInfo.cameraInfo.valuesMap["BusinessGroupID"]);
							GetChildElementValue(info, "DownloadSpeed", catalogItemInfo.cameraInfo.valuesMap["DownloadSpeed"]);
							GetChildElementValue(info, "SVCSpaceSupportMode", catalogItemInfo.cameraInfo.valuesMap["SVCSpaceSupportMode"]);
							GetChildElementValue(info, "SVCTimeSupportMode", catalogItemInfo.cameraInfo.valuesMap["SVCTimeSupportMode"]);
						}

						// ��28181�ֶΣ��Զ��岹���ֶ�
						GetChildElementValue(firstItem, "DeviceID", elementValue);
						catalogItemInfo.resItem.valuesMap["ResType"] = elementValue.substr(10, 3);// ��28181�ֶΣ��Զ��岹���ֶ�
						catalogItemInfo.resItem.valuesMap["ProtocolType"] = "SIP";                // ��28181�ֶΣ��Զ��岹���ֶ�
	
						catalogItemInfo.resItem.tableName = TBL_Res_Attr;
					}
				}
			}

			// �����ݼ���������ݶ���
			xmlParam.responseParam.catalogParam.push_back(catalogItemInfo);

			// �������
			catalogItemInfo.resItem.tableName = "";
			catalogItemInfo.resItem.valuesMap.clear();
			catalogItemInfo.cameraInfo.tableName = "";
			catalogItemInfo.cameraInfo.valuesMap.clear();

			// ��ȡ��һ��Item
			firstItem = firstItem->NextSiblingElement();
		}
	}

	return result;
}

int SipXmlBodyParser::ParserResponseRecordInfo(tinyxml2::XMLElement *& xmlRootElement, XmlCmdParam & xmlParam)
{
	int result = 0;

	// ��ѡ�ֶΣ�������֤
	if (GetChildElementValue(xmlRootElement, "DeviceID", xmlParam.deviceID)||
	    GetChildElementValue(xmlRootElement, "Name", xmlParam.deviceID))
	{
		result = -1;
	}
	else
	{
		
		RespRecordInfoItem recordListItem;
		tinyxml2::XMLElement* recordList = xmlRootElement->FirstChildElement("RecordList");
		//GetElementAttr(recordList, "Num", )
		tinyxml2::XMLElement* recordListFirstItem = recordList->FirstChildElement("Item"); // ��ȡRecordList�ĵ�һ��Item
		while (recordListFirstItem)
		{
			GetChildElementValue(recordListFirstItem, "FilePath", recordListItem.filePath);
			GetChildElementValue(recordListFirstItem, "Address", recordListItem.address);
			GetChildElementValue(recordListFirstItem, "StartTime", recordListItem.startTime);
			GetChildElementValue(recordListFirstItem, "EndTime", recordListItem.endTime);
			GetChildElementValue(recordListFirstItem, "Secrecy", recordListItem.secrecy);
			GetChildElementValue(recordListFirstItem, "Type", recordListItem.type);
			GetChildElementValue(recordListFirstItem, "RecorderID", recordListItem.recorderID);

			// �������������
			xmlParam.responseParam.recordInfoParam.recordInfoItems.push_back(recordListItem);

			// ָ����һ��Ԫ��
			recordListFirstItem = recordListFirstItem->NextSiblingElement();
		}
	}	
	
	return result;
}

int SipXmlBodyParser::GenerateControlCmd(PtzControlParam ptzParam, std::string & ptzString)
{
	std::bitset<8> controlCmd[8];
	controlCmd[0] = 0xA5;
	controlCmd[1] = 0x0F;
	controlCmd[2] = 0x01;

	// ���ÿ�������
	SetControlCmd(controlCmd, ptzParam);

	// ����У����controlCmd[7]
	int sum = 0;
	for (size_t i = 0; i < controlCmd->size() - 1; i++)
	{
		sum = sum + controlCmd[i].to_ulong();
	}
	controlCmd[7] = sum % 256;

	// ����������ת��Ϊ�ַ���
	ptzString = "";
	for (size_t i = 0; i < controlCmd->size(); i++)
	{
		ptzString = ptzString + SipUtilities::BitsetToHex(controlCmd[i]);
	}
	return 0;
}

int SipXmlBodyParser::SetControlCmd(std::bitset<8>* controlCmd, const PtzControlParam ptzParam)
{
	switch (ptzParam.ptzType)
	{
	case PTZ_STOP:
	case PTZ_UP:
	case PTZ_UP_STOP:
	case PTZ_DOWN:
	case PTZ_DOWN_STOP:
	case PTZ_LEFT:
	case PTZ_LEFT_STOP:
	case PTZ_RIGHT:
	case PTZ_RIGHT_STOP:
	case ZOOM_IN:
	case ZOOM_IN_STOP:
	case ZOOM_OUT:
	case ZOOM_OUT_STOP:		
	case PTZ_LEFT_UP:
	case PTZ_LEFT_UP_STOP:
	case PTZ_LEFT_DOWN:
	case PTZ_LEFT_DOWN_STOP:
	case PTZ_RIGHT_UP:
	case PTZ_RIGHT_UP_STOP:
	case PTZ_RIGHT_DOWN:
	case PTZ_RIGHT_DOWN_STOP:
		SetPTZCmd(controlCmd, ptzParam);
		break;
	case FOCUS_FAR:
	case FOCUS_FAR_STOP:
	case FOCUS_NEAR:
	case FOCUS_NEAR_STOP:
	case IRIS_OPEN:
	case IRIS_OPEN_STOP:
	case IRIS_CLOSE:
	case IRIS_CLOSE_STOP:
		SetFICmd(controlCmd, ptzParam);
		break;
	case ADD_PRESET:
	case GOTO_PRESET:
	case DEL_PRESET:
		SetPrePosCmd(controlCmd, ptzParam);
		break;
	case ADD_CRUISE:
	case DEL_CRUISE:
	case SET_NAVI_SPEED:
	case SET_NAVI_STOP_TIME:
	case RUN_CRUISE:
	case STOP_CRUISE:
		SetNaviCmd(controlCmd, ptzParam);
		break;
	case RUN_SCAN:
	case AUTO_SCANNING_LEFT:
	case AUTO_SCANNING_RIGHT:
	case SET_AUTO_SCANNING_SPEED:
	case STOP_SCAN:
		SetScanCmd(controlCmd, ptzParam);
		break;
	default:
		break;
	}
	return 0;
}

int SipXmlBodyParser::SetPTZCmd(std::bitset<8>* controlCmd, const PtzControlParam ptzParam)
{
	int speed = ptzParam.value;
	switch (ptzParam.ptzType)
	{
	case PTZ_STOP:
		controlCmd[3] = 0;
		controlCmd[4] = 0;
		controlCmd[5] = 0;
		controlCmd[6] = 0;
		break;
	case PTZ_UP:
		controlCmd[3][3] = 1;
		controlCmd[5] = speed;
		break;
	case PTZ_UP_STOP:
		controlCmd[3][3] = 0;
		controlCmd[5] = 0;
		break;
	case PTZ_DOWN:
		controlCmd[3][2] = 1;
		controlCmd[5] = speed;
		break;
	case PTZ_DOWN_STOP:
		controlCmd[3][2] = 0;
		controlCmd[5] = 0;
		break;
	case PTZ_LEFT:
		controlCmd[3][1] = 1;
		controlCmd[4] = speed;
		break;
	case PTZ_LEFT_STOP:
		controlCmd[3][1] = 0;
		controlCmd[4] = 0;
		break;
	case PTZ_RIGHT:
		controlCmd[3][0] = 1;
		controlCmd[4] = speed;
		break;
	case PTZ_RIGHT_STOP:
		controlCmd[3][0] = 0;
		controlCmd[4] = 0;
		break;
	case ZOOM_IN:
		controlCmd[3][4] = 1;
		controlCmd[6] = speed * 16;
		break;
	case ZOOM_IN_STOP:
		controlCmd[3][4] = 0;
		controlCmd[6] = 0;
		break;
	case ZOOM_OUT:
		controlCmd[3][5] = 1;
		controlCmd[6] = speed * 16;
		break;
	case ZOOM_OUT_STOP:
		controlCmd[3][5] = 0;
		controlCmd[6] = 0;
		break;
	case PTZ_LEFT_UP:	// 2017.9.15 ����
		controlCmd[3][3] = 1; // up
		controlCmd[3][1] = 1; // left
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_LEFT_UP_STOP:
		controlCmd[3][3] = 0; // up
		controlCmd[3][1] = 0; // left
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_LEFT_DOWN:
		controlCmd[3][2] = 1; // down
		controlCmd[3][1] = 1; // left
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_LEFT_DOWN_STOP:
		controlCmd[3][2] = 0; // down
		controlCmd[3][1] = 0; // left
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_RIGHT_UP:
		controlCmd[3][3] = 1; // up
		controlCmd[3][0] = 1; // right
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_RIGHT_UP_STOP:
		controlCmd[3][3] = 0; // up
		controlCmd[3][0] = 0; // right
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_RIGHT_DOWN:
		controlCmd[3][2] = 1; // down
		controlCmd[3][0] = 1; // right
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	case PTZ_RIGHT_DOWN_STOP:
		controlCmd[3][2] = 0; // down
		controlCmd[3][0] = 0; // right
		controlCmd[4] = speed;
		controlCmd[5] = speed;
		break;
	default:
		break;
	}
	return 0;
}

int SipXmlBodyParser::SetFICmd(std::bitset<8> *controlCmd, const PtzControlParam ptzParam)
{
	CONTROL_TYPE type = ptzParam.ptzType;
	int speed = ptzParam.value;
	controlCmd[3][6] = 1;
	switch (type)
	{
	case FOCUS_FAR:
		controlCmd[3][0] = 1;
		controlCmd[4] = speed;
		break;
	case FOCUS_FAR_STOP:
		controlCmd[3][0] = 0;
		controlCmd[4] = 0;
		break;
	case FOCUS_NEAR:
		controlCmd[3][1] = 1;
		controlCmd[4] = speed;
		break;
	case FOCUS_NEAR_STOP:
		controlCmd[3][1] = 0;
		controlCmd[4] = 0;
		break;
	case IRIS_OPEN:
		controlCmd[3][2] = 1;
		controlCmd[5] = speed;
		break;
	case IRIS_OPEN_STOP:
		controlCmd[3][2] = 0;
		controlCmd[5] = 0;
		break;
	case IRIS_CLOSE:
		controlCmd[3][3] = 1;
		controlCmd[5] = speed;
		break;
	case IRIS_CLOSE_STOP:
		controlCmd[3][3] = 0;
		controlCmd[5] = 0;
		break;
	default:
		controlCmd[3] = 0;
		controlCmd[4] = 0;
		controlCmd[5] = 0;
		break;
	}
	return 0;
}

int SipXmlBodyParser::SetPrePosCmd(std::bitset<8>* controlCmd, const PtzControlParam ptzParam)
{
	controlCmd[4] = 0;

	CONTROL_TYPE type = ptzParam.ptzType;
	int speed = ptzParam.value;
	switch (type)
	{
	case ADD_PRESET:
		controlCmd[3] = 0x81;
		controlCmd[5] = speed;
		break;
	case GOTO_PRESET:
		controlCmd[3] = 0x82;
		controlCmd[5] = speed;
		break;
	case DEL_PRESET:
		controlCmd[3] = 0x83;
		controlCmd[5] = speed;
		break;
	default:
		controlCmd[3] = 0;
		controlCmd[5] = 0;
		break;
	}
	return 0;
}

int SipXmlBodyParser::SetNaviCmd(std::bitset<8>* controlCmd, const PtzControlParam ptzParam)
{
	switch (ptzParam.ptzType)
	{
	case ADD_CRUISE:
		controlCmd[3] = 0x84;
		break;
	case DEL_CRUISE:
		controlCmd[3] = 0x85;
		break;
	case SET_NAVI_SPEED:
		controlCmd[3] = 0x86;
		if (ptzParam.value > 0xFF)
		{
			controlCmd[5] = 0xFF;
			controlCmd[6] = (ptzParam.value - 0xFF);
			controlCmd[6] = controlCmd[6] << 4;
		}
		else
		{
			controlCmd[5] = ptzParam.value;
			controlCmd[6] = 0;
		}
		break;
	case SET_NAVI_STOP_TIME:
		controlCmd[3] = 0x87;
		if (ptzParam.value > 0xFF)
		{
			controlCmd[5] = 0xFF;
			controlCmd[6] = (ptzParam.value - 0xFF);
			controlCmd[6] = controlCmd[6] << 4;
		}
		else
		{
			controlCmd[5] = ptzParam.value;
			controlCmd[6] = 0;
		}
		break;
	case RUN_CRUISE:
		controlCmd[3] = 0x88;
		controlCmd[5] = 0;
		break;
	case STOP_CRUISE:
		controlCmd[3] = 00;
	default:
		break;
	}
	return 0;
}

int SipXmlBodyParser::SetScanCmd(std::bitset<8>* controlCmd, const PtzControlParam ptzParam)
{
	switch (ptzParam.ptzType)
	{
	case RUN_SCAN:
		controlCmd[3] = 0x89;
		controlCmd[5] = 0;
		break;
	case AUTO_SCANNING_LEFT:
		controlCmd[3] = 0x89;
		controlCmd[5] = 0x01;
		break;
	case AUTO_SCANNING_RIGHT:
		controlCmd[3] = 0x89;
		controlCmd[5] = 0x02;
		break;
	case SET_AUTO_SCANNING_SPEED:
		controlCmd[3] = 0x8A;
		if (ptzParam.value > 0xFF)
		{
			controlCmd[5] = 0xFF;
			controlCmd[6] = (ptzParam.value - 0xFF);
			controlCmd[6] = controlCmd[6] << 4;
		}
		else
		{
			controlCmd[5] = ptzParam.value;
			controlCmd[6] = 0;
		}
		break;
	default:
		break;
	}
	return 0;
}

int SipXmlBodyParser::SetOpenClose(std::bitset<8>* controlCmd, const PtzControlParam ptzParam)
{
	switch (ptzParam.value)
	{
	case OPEN_CAMERA:
		controlCmd[5] = 0x8C;
		break;
	case CLOSE_CAMERA:
		controlCmd[5] = 0x8D;
		break;
	}
	return 0;
}

int SipXmlBodyParser::ParserControlCmd(tinyxml2::XMLElement *& rootElement, XmlCmdParam & xmlParam)
{
	int result = -1;
	std::string cmdStr = "";

	// �ж���������
	result = GetChildElementValue(rootElement, "IFameCmd", cmdStr);
	if (cmdStr == "Send") // ǿ�ƹؼ�֡����
	{
		xmlParam.manscdpType = CONTROL_IFAME;
	}
	else
	{
		cmdStr = "";
		result = GetChildElementValue(rootElement, "PTZCmd", cmdStr);
		if (result == 0 && cmdStr.size() == 16) // ptz����
		{
			ParserCameraControlCmd(cmdStr, xmlParam);
			xmlParam.controlParam.ptzParam.ptzCmdValue = cmdStr;
			xmlParam.manscdpType = CONTROL_DEVICE_PTZ;
		}
		else
		{
			result = -1;
		}
	}
	return result;
}

int SipXmlBodyParser::ParserCameraControlCmd(const std::string cmd, XmlCmdParam &xmlParam)
{
	std::bitset<8> cmdBitset4;

	int result = -1;
	result = SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	if (result)
	{
		return -1;
	}

	if (!(cmdBitset4 & (std::bitset<8>)0xFF).to_ulong()) // Ѳ����ɨ��ָֹͣ��
	{
		return 0;
	}
	else
	{
		// �жϿ���ָ������
		if (cmdBitset4[7] == 0 && cmdBitset4[6] == 0) // PTZָ��00xx xxxx	
		{
			ParserPTZCmd(cmd, xmlParam);
		}
		else if (cmdBitset4[7] == 0 && cmdBitset4[6] == 1 &&
			     cmdBitset4[5] == 0 && cmdBitset4[4] == 0) // FIָ��0100 xxxx
		{
			ParserFICmd(cmd, xmlParam);
		}
		else if (cmdBitset4[7] == 1 && cmdBitset4[6] == 0 && // 1000 xxxx
			     cmdBitset4[5] == 0 && cmdBitset4[4] == 0)   // Ѳ����ɨ�衢Ԥ��λ
		{
			std::bitset<8> test(0x0F);
			int i = (cmdBitset4 & test).to_ulong();
			if (i <= 3) // Ԥ��λ
			{
				ParserPrePosCmd(cmd, xmlParam);
			}
			else if (i <= 8) // Ѳ��
			{
				
			}
			else if (i <= 10) // ɨ��
			{

			}
			else if (i == 12 || i == 13) // ��������
			{

			}

		}
		return 0;
	}
}

int SipXmlBodyParser::ParserPTZCmd(const std::string cmd, XmlCmdParam & xmlParam)
{
	std::bitset<8> cmdBitset4;
	std::bitset<8> cmdBitset5;
	std::bitset<8> cmdBitset6;
	std::bitset<8> cmdBitset7;

	SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	SipUtilities::GetBiteByNumber(cmd, 5, cmdBitset5);
	SipUtilities::GetBiteByNumber(cmd, 6, cmdBitset6);
	SipUtilities::GetBiteByNumber(cmd, 7, cmdBitset7);

	int horizontal    = 0; // ˮƽ�ٶ�
	int verticalSpeed = 0; // ��ֱ�ٶ�
	int zoomSpeed     = 0; // ���ű���

	if (cmdBitset4[0]) // ��̨ˮƽ����
	{
		horizontal = cmdBitset5.to_ulong();
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_RIGHT;
	}
	else if (cmdBitset4[1]) // ��̨ˮƽ����
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_LEFT;
		horizontal = cmdBitset5.to_ulong();
	}

	if (cmdBitset4[2]) // ��̨��ֱ����
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_DOWN;
		verticalSpeed = cmdBitset6.to_ulong();
	}
	else if (cmdBitset4[3]) // ��̨��ֱ����
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_UP;
		verticalSpeed = cmdBitset6.to_ulong();
	}

	
	if (cmdBitset4[1] && cmdBitset4[3])// �������˶�
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_LEFT_UP;
	}	
	if (cmdBitset4[1] && cmdBitset4[2])// �������˶�
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_LEFT_DOWN;
	}
	if (cmdBitset4[0] && cmdBitset4[3])// �������˶�
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_RIGHT_UP;
	}	
	if (cmdBitset4[0] && cmdBitset4[2])// �������˶�
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_RIGHT_DOWN;
	}
	xmlParam.controlParam.ptzParam.value = std::sqrt(horizontal * horizontal + verticalSpeed * verticalSpeed);

	if (cmdBitset4[4]) // ��ͷ�䱶�Ŵ�
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::ZOOM_IN;
		zoomSpeed = cmdBitset7.to_ulong();
		xmlParam.controlParam.ptzParam.value = zoomSpeed;
	}
	else if (cmdBitset4[5]) // ��ͷ�䱶��С
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::ZOOM_OUT;
		zoomSpeed = cmdBitset7.to_ulong();
		xmlParam.controlParam.ptzParam.value = zoomSpeed;
	}
	return 0;
}

int SipXmlBodyParser::ParserFICmd(const std::string cmd, XmlCmdParam & xmlParam)
{
	std::bitset<8> cmdBitset4;
	std::bitset<8> cmdBitset5;
	std::bitset<8> cmdBitset6;
	std::bitset<8> cmdBitset7;

	SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	SipUtilities::GetBiteByNumber(cmd, 5, cmdBitset5);
	SipUtilities::GetBiteByNumber(cmd, 6, cmdBitset6);
	SipUtilities::GetBiteByNumber(cmd, 7, cmdBitset7);

	int fSpeed = 0;
	int iSpeed = 0;

	if (!(cmdBitset4 & (std::bitset<8>)0x0F).to_ulong())// ֹͣFI���ж���cmdBitset4 0100 0000
	{
		fSpeed = 0;
		iSpeed = 0;
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::PTZ_STOP;
	}
	else
	{
		if (cmdBitset4[3] == 1) // ��С��ȦcmdBitset4 0100 1000
		{
			fSpeed = cmdBitset6.to_ulong();
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::IRIS_CLOSE;
		}
		else if (cmdBitset4[2] == 1) // �Ŵ��ȦcmdBitset4 0100 0100
		{
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::IRIS_OPEN;
			fSpeed = cmdBitset6.to_ulong();
		}

		if (cmdBitset4[1] == 1) // �۽���cmdBitset4 0100 0010
		{
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::FOCUS_NEAR;
			iSpeed = cmdBitset5.to_ulong();
		}
		else if (cmdBitset4[0] == 1) // �۽�ԶcmdBitset4 0100 0001
		{
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::FOCUS_FAR;
			iSpeed = cmdBitset5.to_ulong();
		}
	}
	xmlParam.controlParam.ptzParam.value = std::sqrt(fSpeed * fSpeed + iSpeed * iSpeed);
	return 0;
}

int SipXmlBodyParser::ParserPrePosCmd(const std::string cmd, XmlCmdParam & xmlParam)
{
	std::bitset<8> cmdBitset4;
	std::bitset<8> cmdBitset6;

	SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	SipUtilities::GetBiteByNumber(cmd, 6, cmdBitset6);

	if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x81)// ����Ԥ��λ1000 0001
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::ADD_PRESET;
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x82) // ����Ԥ��λ1000 0010
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::GOTO_PRESET;
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x83) // ɾ��Ԥ��λ1000 0011
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::DEL_PRESET;
	}
	xmlParam.controlParam.ptzParam.value = cmdBitset6.to_ulong();
	return 0;
}

int SipXmlBodyParser::ParserXHCmd(const std::string cmd, XmlCmdParam & xmlParam)
{
	std::bitset<8> cmdBitset4;
	std::bitset<8> cmdBitset5;
	std::bitset<8> cmdBitset6;
	std::bitset<8> cmdBitset7;

	SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	SipUtilities::GetBiteByNumber(cmd, 5, cmdBitset5);
	SipUtilities::GetBiteByNumber(cmd, 6, cmdBitset6);
	SipUtilities::GetBiteByNumber(cmd, 7, cmdBitset7);
	
	if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x84) // ����Ѳ����1000 0100
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::ADD_CRUISE;
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x85) // ɾ��һ��Ѳ����1000 0101
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::DEL_CRUISE;
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x86) // ����Ѳ���ٶ�1000 0110
	{
		//xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::
		int speed = cmdBitset7.to_ulong() * 256;
		speed = speed + cmdBitset6.to_ulong();
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x87) // ����Ѳ��ͣ��ʱ��1000 0111
	{
		//xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::
		int time = cmdBitset7.to_ulong() * 256;
		time = time + cmdBitset6.to_ulong();
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x88) // ��ʼѲ��1000 1000
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::RUN_CRUISE;
	}
	else if ((cmdBitset4 & (std::bitset<8>)0xFF).to_ulong() == 0x00) // ��ʼѲ��0000 0000
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::STOP_CRUISE;
	}

	return 0;
}

int SipXmlBodyParser::ParserScanCmd(const std::string cmd, XmlCmdParam & xmlParam)
{
	std::bitset<8> cmdBitset4;
	std::bitset<8> cmdBitset5;
	std::bitset<8> cmdBitset6;
	std::bitset<8> cmdBitset7;

	SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	SipUtilities::GetBiteByNumber(cmd, 5, cmdBitset5); // ɨ�����
	SipUtilities::GetBiteByNumber(cmd, 6, cmdBitset6);
	SipUtilities::GetBiteByNumber(cmd, 7, cmdBitset7);

	if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x89) // 1000 1001
	{
		if ((cmdBitset6 & (std::bitset<8>)0xFF).to_ulong() == 0)// ��ʼ�Զ�ɨ��0000 0000
		{
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::RUN_SCAN;
		}
		else if ((cmdBitset6 & (std::bitset<8>)0xFF).to_ulong() == 1)// �����Զ�ɨ����߽�0000 0001
		{
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::AUTO_SCANNING_LEFT;
		}
		else if ((cmdBitset6 & (std::bitset<8>)0xFF).to_ulong() == 2)// �����Զ�ɨ���ұ߽�0000 0010
		{
			xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::AUTO_SCANNING_RIGHT;
		}
	}
	else if ((cmdBitset4 & (std::bitset<8>)0x8F).to_ulong() == 0x8A) // �����Զ�ɨ���ٶ�1000 1010
	{
		xmlParam.controlParam.ptzParam.ptzType = CONTROL_TYPE::SET_AUTO_SCANNING_SPEED;
		int speed = cmdBitset7.to_ulong() * 256;
		speed = speed + cmdBitset6.to_ulong();
	}

	return 0;
}

int SipXmlBodyParser::ParserOpenCloseCmd(const std::string cmd, XmlCmdParam & xmlParam)
{
	std::bitset<8> cmdBitset4;
	std::bitset<8> cmdBitset5;

	SipUtilities::GetBiteByNumber(cmd, 4, cmdBitset4);
	SipUtilities::GetBiteByNumber(cmd, 5, cmdBitset5); // �������ر��

	if ((cmdBitset4 & (std::bitset<8>)0x0F).to_ulong() == 12) // 1000 1100
	{

	}
	else if ((cmdBitset4 & (std::bitset<8>)0x0F).to_ulong() == 13) // �����Զ�ɨ���ٶ�1000 1101
	{
	}

	return 0;
}

void SipXmlBodyParser::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement *& parentElement, std::string childElementName, std::string childElementValue)
{
#if 1
	if (xmlDoc && parentElement && !childElementValue.empty())
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		childElement->SetText(childElementValue.c_str());
	}
#else
	if (xmlDoc && parentElement)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		if (!childElementValue.empty())
		{
			childElement->SetText(childElementValue.c_str());
		}
	}
#endif
}


void SipXmlBodyParser::InsertChildElementWithAttr(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement* &parentElement, std::string childElementName, int childElementAttribute)
{
	if (xmlDoc && parentElement)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		childElement->SetAttribute("Num", childElementAttribute);
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

int SipXmlBodyParser::GetChildElementValue(tinyxml2::XMLElement * parentElement, std::string elementName, int & elementValue)
{
	int result = -1;
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element)
	{
		result = element->QueryIntText(&elementValue);
	}
	return result;
}

int SipXmlBodyParser::GetChildElementValue(tinyxml2::XMLElement * parentElement, std::string elementName, double & elementValue)
{
	int result = -1;
	tinyxml2::XMLElement* element = parentElement->FirstChildElement(elementName.c_str());
	if (element)
	{
		result = element->QueryDoubleText(&elementValue);
	}
	return result;
}

int SipXmlBodyParser::GetElementAttr(tinyxml2::XMLElement * element, std::string elementAttrName, int & elementAttrValue)
{
	int result = -1;
	if (element)
	{
		result = element->QueryAttribute(elementAttrName.c_str(), &elementAttrValue);
	}
	return result;
}

