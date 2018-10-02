#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <bitset>
#include <map>
#include <set>

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


//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* ���ļ����ݲ����޸�
/************************************************************************/
enum CONTROL_TYPE
{
	PTZ_STOP = 0x1009,	//�����ֹͣ����
	VIDEO_SWITCH = 0x1010,	//������л�
	PTZ_UP = 0x1011,	//������ת����ͷ
	PTZ_UP_STOP = 0x1012,	//����ͷ������תֹͣ
	PTZ_DOWN = 0x1013,	//������ת����ͷ
	PTZ_DOWN_STOP = 0x1014,	//����ͷ������תֹͣ
	PTZ_LEFT = 0x1015,	//������ת����ͷ
	PTZ_LEFT_STOP = 0x1016,	//����ͷ������תֹͣ
	PTZ_RIGHT = 0x1017,	//������ת����ͷ
	PTZ_RIGHT_STOP = 0x1018,	//����ͷ������תֹͣ
	ZOOM_IN = 0x1019,	//��ͷ�䱶�Ŵ�
	ZOOM_IN_STOP = 0x1020,	//��ͷ�䱶�Ŵ�ֹͣ
	ZOOM_OUT = 0x1021,	//��ͷ�䱶��С
	ZOOM_OUT_STOP = 0x1022,	//��ͷ�䱶��Сֹͣ

	FOCUS_FAR = 0x1023,	//��ͷ�����Զ
	FOCUS_FAR_STOP = 0x1024,	//��ͷ�����Զֹͣ
	FOCUS_NEAR = 0x1025,	//��ͷ�������
	FOCUS_NEAR_STOP = 0x1026,	//��ͷ�������ֹͣ
	IRIS_OPEN = 0x1027,	//��ͷ��Ȧ�Ŵ�
	IRIS_OPEN_STOP = 0x1028,	//��ͷ��Ȧ�Ŵ�ֹͣ
	IRIS_CLOSE = 0x1029,	//��ͷ��Ȧ��С
	IRIS_CLOSE_STOP = 0x1030,	//��ͷ��Ȧ��Сֹͣ

	GET_PRESET = 0x1031,	//��ѯԤ��λ,20170912���
	GOTO_PRESET = 0x1032,	//����Ԥ��λ
	ADD_PRESET = 0x1033,	//���Ԥ��λ\����
	DEL_PRESET = 0x1034,	//ɾ��Ԥ��λ
	RENAME_PRESET = 0x1035,	//������Ԥ��λ	,20170912��� 

	//////////////////////////////////////////////////////////////////////////
// 	GET_CRUISE		= 0x1036,	// ��ѯѲ��·��
RUN_CRUISE = 0x1037,	// ����Ѳ��·��
STOP_CRUISE = 0x1038,	// ֹͣѲ��
// 	RENAME_CRUISE	= 0x1039,	// ������Ѳ��·��
ADD_CRUISE = 0x103A,	// ���Ѳ��
DEL_CRUISE = 0x103B,	// ɾ��Ѳ��		
// 	STA_MEM_TRACK	= 0x103C,	// ��ʼ��¼�켣		
// 	STO_MEM_TRACK	= 0x103D,	// ֹͣ��¼�켣		
// 	RUN_TRACK		= 0x103E,	// ��ʼ�켣		
// 	RUN_TRACK_STOP	= 0x103F,	// ֹͣ�켣		


	//ADD_CRUISE = 0x1A,		//����Ѳ����
	//DEL_CRUISE = 0x1B,		//ɾ��һ��Ѳ����
	//RUN_CRUISE = 0x1E,		//��ʼѲ��
	//STOP_CRUISE = 0x1F,		//ֹͣѲ��
	SET_NAVI_SPEED = 0x1C,	//����Ѳ���ٶ�
	SET_NAVI_STOP_TIME = 0x1D, //����Ѳ��ͣ��ʱ��

	//////////////////////////////////////////////////////////////////////////
	PTZ_LEFT_UP = 0x1040,	// ��������ת����ͷ		 
	PTZ_LEFT_UP_STOP = 0x1041,	// ����ͷ��������תֹͣ	
	PTZ_LEFT_DOWN = 0x1042,	// ��������ת����ͷ		
	PTZ_LEFT_DOWN_STOP = 0x1043,	// ����ͷ��������תֹͣ		
	PTZ_RIGHT_UP = 0x1044,	// ��������ת����ͷ		
	PTZ_RIGHT_UP_STOP = 0x1045,	// ����ͷ��������תֹͣ		 
	PTZ_RIGHT_DOWN = 0x1046,	// ��������ת����ͷ		
	PTZ_RIGHT_DOWN_STOP = 0x1047,	// ����ͷ��������תֹͣ	

	//////////////////////////////////////////////////////////////////////////
	//RUN_SCAN = 0x1051,			//��ʼ�Զ�ɨ��
	AUTO_SCANNING_LEFT = 0x21,		//�����Զ�ɨ����߽�
	AUTO_SCANNING_RIGHT = 0x22,		//�����Զ�ɨ���ұ߽�
	SET_AUTO_SCANNING_SPEED = 0x23,	//�����Զ�ɨ���ٶ�
	//STOP_SCAN = 0x1052,			//ֹͣɨ��

// 	GET_SCAN = 0x1050,				// ��ѯɨ��		
RUN_SCAN = 0x1051,				// ����ɨ��		
STOP_SCAN = 0x1052,				// ֹͣɨ��		
// 	RENAME_SCAN = 0x1053,			// ������ɨ��		
// 	ADD_SCAN = 0x1054,				// ���ɨ��		
// 	DEL_SCAN = 0x1055,				// ɾ��ɨ��
	//////////////////////////////////////////////////////////////////////////

	SET_GUARD = 0x1060,	// ����
	RESET_GUARD = 0x1061,	// ����		
	REMOTE_START = 0X1070,	// Զ������
	IFAME = 0X1071,	// ǿ�ƹؼ�֡
	SYSTEM_ALARM = 0X1072,	// ϵͳ����
	RESET_ALARM = 0X1073,	// ������λ
	DRAG_ZOOM_IN = 0X1074,	// ����Ŵ�
	DRAG_ZOOM_OUT = 0X1075,	// ������С
	HOME_POSITION = 0X1076,	// ����λ����

	OPEN_CAMERA = 0X25,
	CLOSE_CAMERA = 0X26,

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
// Control
typedef struct PtzControlParam
{
	CONTROL_TYPE ptzType;
	int hSpeed;		// ˮƽ�ٶ�
	int vSpeed;		// ��ֱ�ٶ�
	int zoomSpeed;	// ���ű���

	int value;	// �ٶ�

	int time;	// ʱ��
	std::bitset<8> prePosNum;
	std::bitset<8> naviGroupNum;
	std::bitset<8> scanGroupNum;

	std::string ptzCmdValue;
	PtzControlParam()
	{
		ptzType = CONTROL_TYPE::PTZ_STOP; // Ĭ��ֹͣһ�п���

		hSpeed = 0;		// ˮƽ�ٶ�
		vSpeed = 0;		// ��ֱ�ٶ�
		zoomSpeed = 0;	// ���ű���

		value = 0;	// �ٶ�
		time = 0;	// ʱ��
	}
}PtzControlParam;

typedef struct AlarmInfoParam
{
	std::string alarmMethod;
	std::string alarmType;
}AlarmInfoParam;

typedef struct ResetAlarmParam
{
	std::string resetAlarm;
	std::string alarmMethod;
	std::string alarmType;
	//std::vector<AlarmInfoParam> alarmInfoParam;
	ResetAlarmParam()
	{
		resetAlarm = "ResetAlarm";
	}
}ResetAlarmParam;

typedef struct DragZoomParam
{
	bool isZoomOut;
	int length;
	int width;
	int midPointX;
	int midPointY;
	int lengthX;
	int lengthY;
	DragZoomParam()
	{
		isZoomOut = true;
	}
}DragZoomParam;

typedef struct HomePositionParam
{
	int enabled;
	int resetTime;
	int presetIndex;
}HomePositionParam;

typedef struct DeviceConfigParam
{
	// deviceConfig������������
	std::string name;		// �豸����
	int expiration;			// ע�����ʱ��
	int heartBeatInterval;	// �������ʱ��
	int heartBeatCount;		// ������ʱ����

	// svac ��������
	// svac ��������
}DeviceConfigParam;

typedef struct ControlCmdParam
{
	PtzControlParam   ptzParam;        // Ptz��������
	std::string       teleBootParam;   // Զ����������
	std::string       recordType;      // ¼��������ͣ�Record/StopRecord
	std::string       guardType;       // �����������������ͣ�SetGuard/ResetGuard
	ResetAlarmParam   alarmParam;       // ������λ�������չ����
	std::string       iFame;           // ǿ�ƹؼ�֡
	DragZoomParam     dragZoom;        // ����Ŵ���С
	HomePositionParam homePositionParam;	// ��������������
	DeviceConfigParam deviceConfigParam;	// �豸����
}ControlParam;

// Query
typedef struct QueryCatalog
{
	// �����豸��ʼ����ֹʱ�䣨��ѡ��
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryRecordInfoParam
{
	std::string deviceID;   // �豸����
	std::string name;       // �豸����
	std::string startTime;	// ¼����ʼʱ��
	std::string endTime;	// ¼����ֹʱ��
	std::string filePath;	// �ļ�·������ѡ
	std::string address;	// ¼���ַ����ѡ
	std::string secrecy;	// ¼�������ԣ���ѡ
	std::string type;       // ¼��������ͣ���ѡ
	std::string recorderID;	// ¼�񴥷���id����ѡ
	QueryRecordInfoParam()
	{
		type = "all";
	}
}QueryRecordInfoParam;

typedef struct QueryAlarmParam
{
	std::string startAlarmPriority; // ������ʼ����
	std::string endAlarmPriority;	// ������������
	std::string alarmMethod;		// ������ʽ����
	std::string alarmType;			// ��������
	std::string startAlarmTime;		// ������ʼʱ��
	std::string endAlarmTime;		// ��������ʱ��
}QueryAlarmParam;

typedef struct QueryCmdParam
{
	// �豸״̬��ѯ���豸��Ϣ��ѯ���������	
	QueryCatalog queryCatalogParam;// �豸Ŀ¼��Ϣ��ѯ
	QueryRecordInfoParam recordInfoParam;// �ļ�Ŀ¼��������
	QueryAlarmParam queryAlarmParam;// ������ѯ
	std::string configType;			// �豸������Ϣ��ѯ
									// �豸Ԥ��λ��ѯ���������
	int mobileInterval;		// �ƶ��豸λ�����ݲ�ѯ
	QueryCmdParam()
	{
		mobileInterval = 5;
		configType = "BasicParam";
	}
}QueryParam;

// Notify
typedef struct NotifyKeepaliveDevList // ������ѡ����
{
	std::vector<std::string> info; // �����豸�б�
}NotifyKeepaliveDevList;

typedef struct NotifyAlarmInfo
{
	std::string alarmType;
	std::string eventType;
}NotifyAlarmInfo;

typedef struct NotifyAlarmParam // ����֪ͨ
{
	std::string alarmPriority;	// �������𣬱�ѡ
	std::string alarmMethod;	// ������ʽ����ѡ
	std::string alarmTime;		// ����ʱ�䣬��ѡ
	std::string alarmType;
	std::string alarmTypeParam;
	std::string deviceID;
	std::string alarmDescription;// ����������������ѡ
	std::string longitude;	// ���ȣ���ѡ
	std::string latitude;	// γ�ȣ���ѡ
	std::vector<NotifyAlarmInfo> alarmInfo; // ��չ��Я���������ͣ���������
}NotifyAlarmParam;

typedef struct NotifyBroadcastParam
{
	std::string sourceID;
	std::string targetID;
}NotifyBroadcastParam;

struct NotifyCatalogInfo
{
	std::string deviceID;
	std::string event;

	std::string name;
};

typedef struct NotifyMobilePositionParam
{
	std::string time;		// ����֪ͨʱ�䣬��ѡ
	std::string longtitude;	// ���ȣ���ѡ
	std::string latitude;	// γ�ȣ���ѡ
	std::string speed;		// �ٶȣ���ѡ
	std::string direction;	// ���򣬿�ѡ
	std::string altitude;	// ���θ߶ȣ���ѡ
}NotifyMobilePositionParam;

typedef struct NotifyCmdParam
{
	NotifyKeepaliveDevList keepalive;// ״̬��Ϣ���ͣ�������Ϣ
	NotifyAlarmParam alarm;			 // ����֪ͨ
	std::string mediaStatus;         // "121",��ʾ��ʷý���ļ����ͽ���
	NotifyBroadcastParam broadcast;	 // �����㲥֪ͨ
	NotifyMobilePositionParam mobilePosition;  // �ƶ��豸λ������֪ͨ
	std::vector<NotifyCatalogInfo> deviceList; // Ŀ¼���֪ͨ
	NotifyCmdParam()
	{
		mediaStatus = "121";
	}
}NotifyParam;

// Response

// ���¼���ȡĿ¼����ʱ�����ϼ��㱨Ŀ¼ʱʹ��
typedef struct CatalogInformation
{
	std::string tableName;
	std::map<std::string, std::string> valuesMap;
}CatalogItem, CatalogInfo, CatalogItemOrInfo, ItemMap;

typedef struct CatalogItemAndInfo			// ���ϼ��㱨��Դ��ʹ��
{
	CatalogItem resItem;					// �����/�豸��Ϣ
	CatalogInfo cameraInfo;					// �������չ��Ϣ
}CatalogItemAndInfo;
typedef  std::vector<CatalogItemAndInfo> RespCatalogParam;

typedef struct RespDeviceInfoParam
{
	std::string deviceName;	  // Ŀ���豸����
	std::string result;		  // ��ѯ���
	std::string manufacturer; // �豸������
	std::string model;		  // �豸�ͺ�
	std::string firmware;	  // �豸�̼��汾
	std::string channel;	  // ��Ƶ����ͨ����
	std::string info;		  // ��չ��Ϣ
}RespDeviceInfoParam;

typedef struct ResDevStatusInfoAlarmstatus
{
	std::string deviceID;
	std::string dutyStatus; // "ONDUTY" "OFFDUTY" "ALARM"
}Alarmstatus;

typedef struct RespDeviceStatusParam
{
	std::string result;	// ��ѯ���"OK"��"ERROR"
	std::string online;	// "ONLINE" "OFFLINE"
	std::string status;	// �Ƿ���������"OK"��"ERROR"
	std::string reason;	// ԭ��
	std::string encode;	// �Ƿ���� "ON"��"OFF"
	std::string record;	// �Ƿ�¼�� "ON"��"OFF"
	std::string deviceTime; // �豸ʱ�������
	std::vector<Alarmstatus> alarmStatus; // �����豸״̬�б�
	std::string info;       // ��չ��Ϣ
}RespDeviceStatusParam;

typedef QueryRecordInfoParam RespRecordInfoItem;
typedef struct RespRecordInfoParam
{
	std::string name;	// ���������
	int sumNum;	        // ����ʷ��Ƶ��
	int recordListNum;	// �ô��յ�����ʷ��ƵƬ����
	std::vector<RespRecordInfoItem> recordInfoItems;
}RespRecordInfoParam;

typedef struct RespConfigDownloadParam
{
	std::string result;

	// deviceConfig������������
	std::string name;		        // �豸����
	std::string expiration;			// ע�����ʱ��
	std::string heartBeatInterval;	// �������ʱ��
	std::string heartBeatCount;		// ������ʱ����
	std::string positionCapaility;
	std::string Longitude;
	std::string Latitude;

	// avac

}RespConfigDownloadParam;

typedef struct RespPresetParam
{
	std::string presetID;
	std::string presetName;
}RespPresetParam;
typedef std::vector<RespPresetParam> RespPresetParams;

typedef struct ResponseCmdParam
{
	std::string result; // �豸��������Ӧ��"OK"��"ERROR"
						// ����֪ͨӦ��"OK"��"ERROR"
						// Ŀ¼��Ϣ��ѯ�յ�Ӧ��"OK"��"ERROR"
						// �豸����Ӧ��"OK"��"ERROR"
						// �����㲥Ӧ��"OK"��"ERROR"

	RespCatalogParam        catalogParam;		 // �豸Ŀ¼��ѯӦ��
	RespDeviceInfoParam     devInfoParam;		 // �豸��Ϣ��ѯӦ��
	RespDeviceStatusParam   devStatusParam;		 // �豸״̬��Ϣ��ѯӦ��
	RespRecordInfoParam     recordInfoParam;	 // �ļ�Ŀ¼����Ӧ��
	RespConfigDownloadParam configDownloadParam; // �豸���ò�ѯӦ��
	RespPresetParams        presetList;          // �豸Ԥ��λ��ѯӦ��

	ResponseCmdParam()
	{
		result = "OK";
	}
}ResponseParam;


typedef struct XmlCmdParam
{
	MESSAGE_MANSCDP_TYPE manscdpType;
	std::string reqRespType; // Control, Query, Notify, Response
	std::string cmdType;
	int sn;
	std::string deviceID;
	int sumNum;
	bool isMust;

	ControlParam  controlParam;
	QueryParam    queryParam;
	NotifyParam   notifyParam;
	ResponseParam responseParam;

	std::string localSipID;
	std::string remoteSipID;

	std::string bodyString;
	XmlCmdParam()
	{
		sumNum = 0;
		isMust = false;
	}
}XmlParam;

// �ϼ���ѯ����Ŀ¼����Ŀ¼�ṹʹ��
typedef struct CatalogTree
{
	int flag;		// 0�����ϼ��㱨����֯Ŀ¼
					// 1�����ϼ��㱨����֯�������Ŀ¼
					// 2���Ƴ�����֯Ŀ¼ΪrootVirOrgID��ֵ��һ����֯����
	int sn;			// ��ѯĿ¼��Ϣ���������к�
	int startLine;	// ��ʼ�У����ϼ��㱨��֯Ŀ¼�������ʱʹ��
	int cameraSumNum;	// ����֯�������ʣ������
	time_t lastResponseTime;	// ���һ��Ŀ¼�ظ�200Ok��ʱ��
	std::string protocol;		// Ŀ¼���ͷ�ʽTCP��UDP
	std::string remoteSipID;	// ��ѯ��ƽ̨Ŀ¼��sipID
	std::string rootVirOrgID;	// ��ѯĿ¼�ĸ��ڵ���֯����
	std::deque<std::string> virtualOrgIDs;	// ��Ҫ���ϼ��㱨����֯����

	int allItemSumNum = 0;
	std::set<int> orgGroupIDs; // ���ϼ��������ļ���,GroupID
	std::set<int> orgCameraIDs;// ���ϼ������ĳ���������,ResID

	std::set<int> tempGroupIDs; // ��Ҫ���ϼ����͵��������ID
	std::set<int> tempCameraIDs;// ��Ҫ���ϼ����͵����������ID

	std::deque<CatalogItemAndInfo> allCatalog; // ��Ҫ���ϼ����͵�����Ŀ¼

	CatalogTree()
	{
		sn = 0;
		flag = 0;
		startLine = 0;
		cameraSumNum = 0;
		lastResponseTime = 0;
	}
}CatalogTree;
#define CatalogTrees std::deque<CatalogTree>

/*
 * ����Ϣ�ṹ��
 */
struct GroupInfo
{
	int groupID;
	int type;
	std::string deviceID;
	std::string name;
	std::string parentID;
	std::string bussinessGroupID;
	int onNum;
	int sumNum;
	int parentGroupID;
};
#define GroupInfoList std::vector<std::shared_ptr<GroupInfo>>

/*
 * �������Ϣ�ṹ��
 */
struct CameraInfo
{
	std::string deviceID;
	std::string name;
	std::string parentID;
	std::string civilCode;
	std::string status;
	int ptzType;
};
#define CameraInfoList std::vector<std::shared_ptr<CameraInfo>>

/*
 * ƽ̨��Ϣ�ṹ��
 */
struct PlatformInfo
{
	int id;
	std::string name;
	std::string deviceID;
	std::string domain;
	std::string ip;
	int port;
	int upOrDown;
	std::string status;
};
#define PlatformInfoList std::vector<std::shared_ptr<PlatformInfo>>