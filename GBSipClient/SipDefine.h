#pragma once
#include <iostream>
#include <vector>
#include <string>

enum MESSAGE_MANSCDP_TYPE
{
	MESSAGE_MANSCDP_ERROR,	// ���������

	//���ƣ�10
	CONTROL_DEVICE_PTZ,		// �豸ptz����,��Ӧ��
	CONTROL_TELE_BOOT,		// Զ����������,��Ӧ��
	CONTROL_RECORD,			// ¼����ƣ���ʼ¼��ֹͣ¼�� ��Ӧ��
	CONTROL_GUARD,			// ����������,                ��Ӧ��
	CONTROL_ALARM,			// ������λ���              ��Ӧ��
	CONTROL_IFAME,			// ǿ�ƹؼ�֡,��Ӧ��
	CONTROL_DRAG_ZOOM,	    // ����Ŵ�\��С,��Ӧ��
	CONTROL_HOME_POSITION,	// ��������������Զ���λ��    ��Ӧ��
	CONTROL_DEVICE_CONFIG,	// �豸���ã�                 ��Ӧ��

	//��ѯ����8
	QUERY_DEVICE_STATUS,	// �豸״̬��ѯ����
	QUERY_DEVICE_CATALOG,	// �豸Ŀ¼��Ϣ��ѯ����
	QUERY_DEVICE_INFO,		// �豸��Ϣ��ѯ����
	QUERY_RECORD_INFO,		// �ļ�Ŀ¼��������
	QUERY_ALARM,			// ������ѯ
	QUERY_CONFIG_DOWNLOAD,	// �豸���ò�ѯ
	QUERY_PRESET_QUERY,		// �豸Ԥ��λ��ѯ
	QUERY_MOBILE_POSITION,	// �ƶ��豸λ�����ݲ�ѯ

	//֪ͨ,5
	NOTIFY_KEEPALIVE,		// ״̬��Ϣ���ͣ�����
	NOTIFY_ALARM,			// ����֪ͨ
	NOTIFY_MEDIA_STATUS,	// ý��֪ͨ��filetoend
	NOTIFY_BROADCAST,		// �����㲥֪ͨ
	NOTIFY_MOBILE_POSITION,	// �ƶ��豸λ������֪ͨ
	NOTIFY_CATALOG,

	//Ӧ��12
	RESPONSE_DEVICE_CONTROL,			// �豸����Ӧ��
	RESPONSE_ALARM_NOTIFY,				// ����֪ͨӦ��
	RESPONSE_DEVICE_CATALOG_QUERY,		// �豸Ŀ¼��Ϣ��ѯӦ��Ŀ¼֪ͨ��
	RESPONSE_CATALOG_QUERY_RECEIVED,	// Ŀ¼��Ϣ��ѯ�յ�Ӧ��Ŀ¼����Ӧ��
	RESPONSE_CATALOG_RECEIVED,			// Ŀ¼�յ�Ӧ��Ŀ¼֪ͨӦ��
	RESPONSE_DEVICE_INFO_QUERY,			// �豸��Ϣ��ѯӦ��
	RESPONSE_DEVICE_STATUS_QUERY,		// �豸״̬��Ϣ��ѯӦ��
	RESPONSE_DOCUMENT_RECORD_INFO,		// �ļ�Ŀ¼����Ӧ��
	RESPONSE_DEVICE_CONFIG,				// �豸����Ӧ��
	RESPONSE_DEVICE_CONFIG_DOWNLOAD,	// �豸���ò�ѯӦ��
	RESPONSE_DEVICE_PRESET_QUERY,		// �豸Ԥ��λ��ѯӦ��
	RESPONSE_BROADCAST,					// �����㲥Ӧ��

	CATALOG_CHANGED,

	XML_COUNT
};

////////////////////////////////SDP//////////////////////////////////////////
typedef struct O_Param
{
	std::string userName;
	std::string sessID;
	std::string sessVersion;
	std::string netType;
	std::string addrType;
	std::string addr;
	O_Param()
	{
		userName = "";
		sessID = "0";
		sessVersion = "0";
		netType = "IN";
		addrType = "IP4";
		addr = "";
	}
}O_Param;

typedef struct U_Param
{
	std::string deviceID;
	int num;
	U_Param()
	{
		deviceID = "";
		num = 3;
	}
}U_Param;

typedef struct C_Param
{
	std::string netType;
	std::string addrType;
	std::string addr;
	C_Param()
	{
		netType = "IN";
		addrType = "IP4";
		addr = "";
	}
}C_Param;

typedef struct T_Param
{
	std::string startTime;
	std::string endTime;
	T_Param()
	{
		startTime = "0";
		endTime = "0";
	}
}T_Param;

typedef struct M_Param
{
	//bool isAudio;
	std::string mediaType;
	bool isRecvOnly;
	int port;
	std::string rtpType;
	std::string transType; // active��passive��udp
	std::string downloadSpeed;
	std::vector<std::string> payloads;   // 96 97 98
	std::vector<std::string> attributes; // a=rtpmap:96 PS/90000
	M_Param()
	{
		port = 0;
		//isAudio = false;
		mediaType = "video";
		isRecvOnly = false;
		rtpType = "RTP/AVP";
		transType = "udp";
		downloadSpeed = "2";
	}
}M_Param;

typedef struct SdpMediaParam
{
	std::string deviceID; // �豸����
	std::string version;
	O_Param oParam;
	std::string sName; // ��Play������Playback������Download��
	U_Param uParam;
	C_Param cParam;
	T_Param tParam;
	M_Param mParam;
	std::string y;
	std::string f;

	std::string bodyString;
	SdpMediaParam()
	{
		version = "0";
		sName = "Play";
		y = "";
		f = "v/////a///";
	}
}SdpParam;

/////////////////////////////////XML/////////////////////////////////////////
// Query
typedef struct QueryCatalog
{
	// �����豸��ʼ����ֹʱ�䣨��ѡ��
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryCmdParam
{
	// �豸״̬��ѯ���豸��Ϣ��ѯ���������	
	QueryCatalog queryCatalogParam;// �豸Ŀ¼��Ϣ��ѯ
// 	QueryRecordInfoParam recordInfoParam;// �ļ�Ŀ¼��������
// 	QueryAlarmParam queryAlarmParam;// ������ѯ
	std::string configType;			// �豸������Ϣ��ѯ
									// �豸Ԥ��λ��ѯ���������
	int mobileInterval;		// �ƶ��豸λ�����ݲ�ѯ
	QueryCmdParam()
	{
		mobileInterval = 5;
		configType = "BasicParam";
	}
}QueryParam;

typedef struct XmlCmdParam
{
	MESSAGE_MANSCDP_TYPE manscdpType;
	std::string reqRespType; // Control, Query, Notify, Response
	std::string cmdType;
	int sn;
	std::string deviceID;
	int sumNum;
	bool isMust;

// 	ControlParam  controlParam;
 	QueryParam    queryParam;
// 	NotifyParam   notifyParam;
// 	ResponseParam responseParam;

	std::string localSipID;
	std::string remoteSipID;

	std::string bodyString;
	XmlCmdParam()
	{
		sumNum = 0;
		isMust = false;
	}
}XmlParam;