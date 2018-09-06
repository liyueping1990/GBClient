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
	static int ControlPtzCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �豸Ptz����
	static int ControlTeleBoot(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �豸Զ������
	static int ControlRecordCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);    // �豸¼�����
	static int ControlGuardCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �豸����/����
	static int ControlAlarmCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �豸������λ
	static int ControlIFameCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �ؼ�֡
	static int ControlDragZoom(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // ����Ŵ�/��С
	static int ControlHomePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam); // ��������λ
	static int ControlDeviceConfig(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam); // �豸����

	// Query
	static int QueryDeviceStatus(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);  // �豸Ŀ¼��Ϣ��ѯ
	static int QueryCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	      // �豸Ŀ¼��Ϣ��ѯ
	static int QueryDeviceInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	  // �豸Ŀ¼��Ϣ��ѯ
	static int QueryRecordInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	  // ��ʷ¼���ļ���ѯ
	static int QueryAlarm(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		  // ������ѯ
	static int QueryConfigDownload(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// �豸���ò�ѯ
	static int QueryPreset(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);        // �豸���ò�ѯ
	static int QueryMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// �ƶ��豸λ�ò�ѯ

	// Notify
	static int NotifyKeepalive(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �豸״̬֪ͨ
	static int NotifyAlarm(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		   // ����֪ͨ
	static int NotifyMediaStutas(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);   // ý��״̬֪ͨ
	static int NotifyBroadcast(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	   // �����㲥֪ͨ
	static int NotifyMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// �ƶ��豸λ������֪ͨ
	static int NotifyCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);       // Ŀ¼���֪ͨ

	// Response
	static int ResponseDeviceControl(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// �豸����Ӧ��
	static int ResponseAlarmNotify(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// ����֪ͨӦ��
	static int ResponseCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);			// Ŀ¼��ѯӦ��
	static int ResponseRcvCatalogQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// �յ�Ŀ¼��Ϣ��ѯ��Ӧ��
	static int ResponseRcvCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// Ŀ¼��Ϣ��ѯ�յ������յ��¼����͵�Ŀ¼��Ϣ��Ӧ��
	static int ResponseDeviceInfoQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// �豸��Ϣ��ѯӦ��
	static int ResponseDeviceStatusQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);// �豸״̬��Ϣ��ѯӦ��
	static int ResponseRecordInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// �ļ�Ŀ¼����Ӧ��
	static int ResponseDeviceConfig(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	    // �豸����Ӧ��
	static int ResponseConfigDownload(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);	// �豸���ò�ѯӦ��
	static int ResponsePresetQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// �豸Ԥ��λ��ѯӦ��
	static int ResponseBroadcast(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam *xmlParam);		// �����㲥Ӧ��

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