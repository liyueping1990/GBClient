#pragma once
#include <iostream>
#include <vector>
#include <string>

enum MESSAGE_MANSCDP_TYPE
{
	MESSAGE_MANSCDP_ERROR,	// 错误的命令

	//控制，10
	CONTROL_DEVICE_PTZ,		// 设备ptz控制,无应答
	CONTROL_TELE_BOOT,		// 远程启动控制,无应答
	CONTROL_RECORD,			// 录像控制，开始录像、停止录像， 有应答
	CONTROL_GUARD,			// 布防、撤防,                有应答
	CONTROL_ALARM,			// 报警复位命令，              有应答
	CONTROL_IFAME,			// 强制关键帧,无应答
	CONTROL_DRAG_ZOOM,	    // 拉框放大\缩小,无应答
	CONTROL_HOME_POSITION,	// 看守卫控制命令，自动归位，    有应答
	CONTROL_DEVICE_CONFIG,	// 设备配置，                 有应答

	//查询请求，8
	QUERY_DEVICE_STATUS,	// 设备状态查询请求
	QUERY_DEVICE_CATALOG,	// 设备目录信息查询请求
	QUERY_DEVICE_INFO,		// 设备信息查询请求
	QUERY_RECORD_INFO,		// 文件目录检索请求
	QUERY_ALARM,			// 报警查询
	QUERY_CONFIG_DOWNLOAD,	// 设备配置查询
	QUERY_PRESET_QUERY,		// 设备预置位查询
	QUERY_MOBILE_POSITION,	// 移动设备位置数据查询

	//通知,5
	NOTIFY_KEEPALIVE,		// 状态信息报送，心跳
	NOTIFY_ALARM,			// 报警通知
	NOTIFY_MEDIA_STATUS,	// 媒体通知，filetoend
	NOTIFY_BROADCAST,		// 语音广播通知
	NOTIFY_MOBILE_POSITION,	// 移动设备位置数据通知
	NOTIFY_CATALOG,

	//应答，12
	RESPONSE_DEVICE_CONTROL,			// 设备控制应答
	RESPONSE_ALARM_NOTIFY,				// 报警通知应答
	RESPONSE_DEVICE_CATALOG_QUERY,		// 设备目录信息查询应答（目录通知）
	RESPONSE_CATALOG_QUERY_RECEIVED,	// 目录信息查询收到应答（目录订阅应答）
	RESPONSE_CATALOG_RECEIVED,			// 目录收到应答（目录通知应答）
	RESPONSE_DEVICE_INFO_QUERY,			// 设备信息查询应答
	RESPONSE_DEVICE_STATUS_QUERY,		// 设备状态信息查询应答
	RESPONSE_DOCUMENT_RECORD_INFO,		// 文件目录检索应答
	RESPONSE_DEVICE_CONFIG,				// 设备配置应答
	RESPONSE_DEVICE_CONFIG_DOWNLOAD,	// 设备配置查询应答
	RESPONSE_DEVICE_PRESET_QUERY,		// 设备预置位查询应答
	RESPONSE_BROADCAST,					// 语音广播应答

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
	std::string transType; // active、passive、udp
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
	std::string deviceID; // 设备编码
	std::string version;
	O_Param oParam;
	std::string sName; // “Play”、“Playback”、“Download”
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
	// 增加设备起始、终止时间（可选）
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryCmdParam
{
	// 设备状态查询，设备信息查询，无需参数	
	QueryCatalog queryCatalogParam;// 设备目录信息查询
// 	QueryRecordInfoParam recordInfoParam;// 文件目录检索请求
// 	QueryAlarmParam queryAlarmParam;// 报警查询
	std::string configType;			// 设备配置信息查询
									// 设备预置位查询，无需参数
	int mobileInterval;		// 移动设备位置数据查询
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