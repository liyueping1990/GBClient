#pragma once
#include "tinyxml2.h"
#include "SipMsgBodyParser.h"

typedef int(*CreateBodyCallback)(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);

class SipXmlBodyParser :
	public SipMsgBodyParser
{
public:
	SipXmlBodyParser();
	virtual ~SipXmlBodyParser();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString);
	virtual int ParserSipMsgBody(const char* bodyString, void* bodyParameter);

private:
	static std::once_flag mOnceFlag;
	static CreateBodyCallback CreateBodyCallbacks[XML_COUNT];
	static void SetCreateCallback(int type, CreateBodyCallback cb);
	static void RegisterCreateCallback();

	// Control
	static int ControlPtzCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 设备Ptz控制
	static int ControlTeleBoot(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 设备远程重启
	static int ControlRecordCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);    // 设备录像控制
	static int ControlGuardCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 设备布防/撤防
	static int ControlAlarmCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 设备报警复位
	static int ControlIFameCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 关键帧
	static int ControlDragZoom(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 拉框放大/缩小
	static int ControlHomePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam); // 看守卫复位
	static int ControlDeviceConfig(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam); // 设备配置

	// Query
	static int QueryDeviceStatus(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);  // 设备目录信息查询
	static int QueryCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	      // 设备目录信息查询
	static int QueryDeviceInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	  // 设备目录信息查询
	static int QueryRecordInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	  // 历史录像文件查询
	static int QueryAlarm(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		  // 报警查询
	static int QueryConfigDownload(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// 设备配置查询
	static int QueryPreset(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);        // 设备配置查询
	static int QueryMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// 移动设备位置查询

	// Notify
	static int NotifyKeepalive(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 设备状态通知
	static int NotifyAlarm(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		   // 报警通知
	static int NotifyMediaStutas(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);   // 媒体状态通知
	static int NotifyBroadcast(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // 语音广播通知
	static int NotifyMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// 移动设备位置数据通知
	static int NotifyCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);       // 目录变更通知

	// Response
	static int ResponseDeviceControl(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// 设备控制应答
	static int ResponseAlarmNotify(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// 报警通知应答
	static int ResponseCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);			// 目录查询应答
	static int ResponseRcvCatalogQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// 收到目录信息查询的应答
	static int ResponseRcvCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// 目录信息查询收到（即收到下级发送的目录信息）应答
	static int ResponseDeviceInfoQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// 设备信息查询应答
	static int ResponseDeviceStatusQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// 设备状态信息查询应答
	static int ResponseRecordInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// 文件目录检索应答
	static int ResponseDeviceConfig(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	    // 设备配置应答
	static int ResponseConfigDownload(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// 设备配置查询应答
	static int ResponsePresetQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// 设备预置位查询应答
	static int ResponseBroadcast(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// 语音广播应答

	//////////////////////////////////////////////////////////////////////////
	static int ParserControl(tinyxml2::XMLElement* &xmlRootElement, XmlCmdParam *xmlParam);
	static int ParserQuery(tinyxml2::XMLElement* &xmlRootElement, XmlCmdParam *xmlParam);
	static int ParserNotify(tinyxml2::XMLElement* &xmlRootElement, XmlCmdParam *xmlParam);
	static int ParserResponse(tinyxml2::XMLElement* &xmlRootElement, XmlCmdParam *xmlParam);


	template<typename T>
	static void InsertChildElement(tinyxml2::XMLDocument *& xmlDoc,
		                           tinyxml2::XMLElement* &parentElement,
		                           std::string childElementName,
		                           T childElementValue);
	static void InsertChildElement(tinyxml2::XMLDocument *& xmlDoc,
		                           tinyxml2::XMLElement* &parentElement,
		                           std::string childElementName,
		                           std::string childElementValue);
	static int GetChildElementValue(tinyxml2::XMLElement* parentElement,
		                            std::string elementName,
		                            std::string &elementValue);
	static int GetChildElementValue(tinyxml2::XMLElement* parentElement,
		                            std::string elementName,
		                            int &elementValue);
};

template<typename T>
void SipXmlBodyParser::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, tinyxml2::XMLElement* &parentElement, std::string childElementName, T childElementValue)
{
	if (xmlDoc && childElementValue > 0)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		childElement->SetText(childElementValue);
	}
}
