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


//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* 本文件内容不能修改
/************************************************************************/
enum CONTROL_TYPE
{
	PTZ_STOP = 0x1009,	//摄像机停止控制
	VIDEO_SWITCH = 0x1010,	//摄像机切换
	PTZ_UP = 0x1011,	//向上旋转摄像头
	PTZ_UP_STOP = 0x1012,	//摄像头向上旋转停止
	PTZ_DOWN = 0x1013,	//向下旋转摄像头
	PTZ_DOWN_STOP = 0x1014,	//摄像头向下旋转停止
	PTZ_LEFT = 0x1015,	//向左旋转摄像头
	PTZ_LEFT_STOP = 0x1016,	//摄像头向左旋转停止
	PTZ_RIGHT = 0x1017,	//向右旋转摄像头
	PTZ_RIGHT_STOP = 0x1018,	//摄像头向右旋转停止
	ZOOM_IN = 0x1019,	//镜头变倍放大
	ZOOM_IN_STOP = 0x1020,	//镜头变倍放大停止
	ZOOM_OUT = 0x1021,	//镜头变倍缩小
	ZOOM_OUT_STOP = 0x1022,	//镜头变倍缩小停止

	FOCUS_FAR = 0x1023,	//镜头焦距调远
	FOCUS_FAR_STOP = 0x1024,	//镜头焦距调远停止
	FOCUS_NEAR = 0x1025,	//镜头焦距调近
	FOCUS_NEAR_STOP = 0x1026,	//镜头焦距调近停止
	IRIS_OPEN = 0x1027,	//镜头光圈放大
	IRIS_OPEN_STOP = 0x1028,	//镜头光圈放大停止
	IRIS_CLOSE = 0x1029,	//镜头光圈缩小
	IRIS_CLOSE_STOP = 0x1030,	//镜头光圈缩小停止

	GET_PRESET = 0x1031,	//查询预置位,20170912添加
	GOTO_PRESET = 0x1032,	//调用预置位
	ADD_PRESET = 0x1033,	//添加预置位\设置
	DEL_PRESET = 0x1034,	//删除预置位
	RENAME_PRESET = 0x1035,	//重命名预置位	,20170912添加 

	//////////////////////////////////////////////////////////////////////////
// 	GET_CRUISE		= 0x1036,	// 查询巡航路径
RUN_CRUISE = 0x1037,	// 调用巡航路径
STOP_CRUISE = 0x1038,	// 停止巡航
// 	RENAME_CRUISE	= 0x1039,	// 重命名巡航路径
ADD_CRUISE = 0x103A,	// 添加巡航
DEL_CRUISE = 0x103B,	// 删除巡航		
// 	STA_MEM_TRACK	= 0x103C,	// 开始记录轨迹		
// 	STO_MEM_TRACK	= 0x103D,	// 停止记录轨迹		
// 	RUN_TRACK		= 0x103E,	// 开始轨迹		
// 	RUN_TRACK_STOP	= 0x103F,	// 停止轨迹		


	//ADD_CRUISE = 0x1A,		//加入巡航点
	//DEL_CRUISE = 0x1B,		//删除一个巡航点
	//RUN_CRUISE = 0x1E,		//开始巡航
	//STOP_CRUISE = 0x1F,		//停止巡航
	SET_NAVI_SPEED = 0x1C,	//设置巡航速度
	SET_NAVI_STOP_TIME = 0x1D, //设置巡航停留时间

	//////////////////////////////////////////////////////////////////////////
	PTZ_LEFT_UP = 0x1040,	// 向左上旋转摄像头		 
	PTZ_LEFT_UP_STOP = 0x1041,	// 摄像头向左上旋转停止	
	PTZ_LEFT_DOWN = 0x1042,	// 向左下旋转摄像头		
	PTZ_LEFT_DOWN_STOP = 0x1043,	// 摄像头向左下旋转停止		
	PTZ_RIGHT_UP = 0x1044,	// 向右上旋转摄像头		
	PTZ_RIGHT_UP_STOP = 0x1045,	// 摄像头向右上旋转停止		 
	PTZ_RIGHT_DOWN = 0x1046,	// 向右下旋转摄像头		
	PTZ_RIGHT_DOWN_STOP = 0x1047,	// 摄像头向右下旋转停止	

	//////////////////////////////////////////////////////////////////////////
	//RUN_SCAN = 0x1051,			//开始自动扫描
	AUTO_SCANNING_LEFT = 0x21,		//设置自动扫描左边界
	AUTO_SCANNING_RIGHT = 0x22,		//设置自动扫描右边界
	SET_AUTO_SCANNING_SPEED = 0x23,	//设置自动扫描速度
	//STOP_SCAN = 0x1052,			//停止扫描

// 	GET_SCAN = 0x1050,				// 查询扫描		
RUN_SCAN = 0x1051,				// 启动扫描		
STOP_SCAN = 0x1052,				// 停止扫描		
// 	RENAME_SCAN = 0x1053,			// 重命名扫描		
// 	ADD_SCAN = 0x1054,				// 添加扫描		
// 	DEL_SCAN = 0x1055,				// 删除扫描
	//////////////////////////////////////////////////////////////////////////

	SET_GUARD = 0x1060,	// 布防
	RESET_GUARD = 0x1061,	// 撤防		
	REMOTE_START = 0X1070,	// 远程启动
	IFAME = 0X1071,	// 强制关键帧
	SYSTEM_ALARM = 0X1072,	// 系统报警
	RESET_ALARM = 0X1073,	// 报警复位
	DRAG_ZOOM_IN = 0X1074,	// 拉框放大
	DRAG_ZOOM_OUT = 0X1075,	// 拉框缩小
	HOME_POSITION = 0X1076,	// 看守位控制

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
// Control
typedef struct PtzControlParam
{
	CONTROL_TYPE ptzType;
	int hSpeed;		// 水平速度
	int vSpeed;		// 垂直速度
	int zoomSpeed;	// 缩放倍数

	int value;	// 速度

	int time;	// 时间
	std::bitset<8> prePosNum;
	std::bitset<8> naviGroupNum;
	std::bitset<8> scanGroupNum;

	std::string ptzCmdValue;
	PtzControlParam()
	{
		ptzType = CONTROL_TYPE::PTZ_STOP; // 默认停止一切控制

		hSpeed = 0;		// 水平速度
		vSpeed = 0;		// 垂直速度
		zoomSpeed = 0;	// 缩放倍数

		value = 0;	// 速度
		time = 0;	// 时间
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
	// deviceConfig基本参数配置
	std::string name;		// 设备名称
	int expiration;			// 注册过期时间
	int heartBeatInterval;	// 心跳间隔时间
	int heartBeatCount;		// 心跳超时次数

	// svac 编码配置
	// svac 解码配置
}DeviceConfigParam;

typedef struct ControlCmdParam
{
	PtzControlParam   ptzParam;        // Ptz控制命令
	std::string       teleBootParam;   // 远程启动命令
	std::string       recordType;      // 录像控制类型，Record/StopRecord
	std::string       guardType;       // 布防、撤防控制类型，SetGuard/ResetGuard
	ResetAlarmParam   alarmParam;       // 报警复位命令及其扩展参数
	std::string       iFame;           // 强制关键帧
	DragZoomParam     dragZoom;        // 拉框放大缩小
	HomePositionParam homePositionParam;	// 看守卫控制命令
	DeviceConfigParam deviceConfigParam;	// 设备配置
}ControlParam;

// Query
typedef struct QueryCatalog
{
	// 增加设备起始、终止时间（可选）
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryRecordInfoParam
{
	std::string deviceID;   // 设备编码
	std::string name;       // 设备名称
	std::string startTime;	// 录像起始时间
	std::string endTime;	// 录像终止时间
	std::string filePath;	// 文件路径，可选
	std::string address;	// 录像地址，可选
	std::string secrecy;	// 录像保密属性，可选
	std::string type;       // 录像产生类型，可选
	std::string recorderID;	// 录像触发者id，可选
	QueryRecordInfoParam()
	{
		type = "all";
	}
}QueryRecordInfoParam;

typedef struct QueryAlarmParam
{
	std::string startAlarmPriority; // 报警起始级别
	std::string endAlarmPriority;	// 报警结束级别
	std::string alarmMethod;		// 报警方式条件
	std::string alarmType;			// 报警类型
	std::string startAlarmTime;		// 报警开始时间
	std::string endAlarmTime;		// 报警结束时间
}QueryAlarmParam;

typedef struct QueryCmdParam
{
	// 设备状态查询，设备信息查询，无需参数	
	QueryCatalog queryCatalogParam;// 设备目录信息查询
	QueryRecordInfoParam recordInfoParam;// 文件目录检索请求
	QueryAlarmParam queryAlarmParam;// 报警查询
	std::string configType;			// 设备配置信息查询
									// 设备预置位查询，无需参数
	int mobileInterval;		// 移动设备位置数据查询
	QueryCmdParam()
	{
		mobileInterval = 5;
		configType = "BasicParam";
	}
}QueryParam;

// Notify
typedef struct NotifyKeepaliveDevList // 心跳可选参数
{
	std::vector<std::string> info; // 故障设备列表
}NotifyKeepaliveDevList;

typedef struct NotifyAlarmInfo
{
	std::string alarmType;
	std::string eventType;
}NotifyAlarmInfo;

typedef struct NotifyAlarmParam // 报警通知
{
	std::string alarmPriority;	// 报警级别，必选
	std::string alarmMethod;	// 报警方式，必选
	std::string alarmTime;		// 报警时间，必选
	std::string alarmType;
	std::string alarmTypeParam;
	std::string deviceID;
	std::string alarmDescription;// 报警内容描述，可选
	std::string longitude;	// 经度，可选
	std::string latitude;	// 纬度，可选
	std::vector<NotifyAlarmInfo> alarmInfo; // 扩展，携带报警类型，报警参数
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
	std::string time;		// 产生通知时间，必选
	std::string longtitude;	// 经度，必选
	std::string latitude;	// 纬度，必选
	std::string speed;		// 速度，可选
	std::string direction;	// 方向，可选
	std::string altitude;	// 海拔高度，可选
}NotifyMobilePositionParam;

typedef struct NotifyCmdParam
{
	NotifyKeepaliveDevList keepalive;// 状态信息报送，心跳信息
	NotifyAlarmParam alarm;			 // 报警通知
	std::string mediaStatus;         // "121",表示历史媒体文件发送结束
	NotifyBroadcastParam broadcast;	 // 语音广播通知
	NotifyMobilePositionParam mobilePosition;  // 移动设备位置数据通知
	std::vector<NotifyCatalogInfo> deviceList; // 目录变更通知
	NotifyCmdParam()
	{
		mediaStatus = "121";
	}
}NotifyParam;

// Response

// 从下级获取目录解析时和向上级汇报目录时使用
typedef struct CatalogInformation
{
	std::string tableName;
	std::map<std::string, std::string> valuesMap;
}CatalogItem, CatalogInfo, CatalogItemOrInfo, ItemMap;

typedef struct CatalogItemAndInfo			// 向上级汇报资源点使用
{
	CatalogItem resItem;					// 摄像机/设备信息
	CatalogInfo cameraInfo;					// 摄像机扩展信息
}CatalogItemAndInfo;
typedef  std::vector<CatalogItemAndInfo> RespCatalogParam;

typedef struct RespDeviceInfoParam
{
	std::string deviceName;	  // 目标设备名称
	std::string result;		  // 查询结果
	std::string manufacturer; // 设备生产商
	std::string model;		  // 设备型号
	std::string firmware;	  // 设备固件版本
	std::string channel;	  // 视频输入通道数
	std::string info;		  // 扩展信息
}RespDeviceInfoParam;

typedef struct ResDevStatusInfoAlarmstatus
{
	std::string deviceID;
	std::string dutyStatus; // "ONDUTY" "OFFDUTY" "ALARM"
}Alarmstatus;

typedef struct RespDeviceStatusParam
{
	std::string result;	// 查询结果"OK"或"ERROR"
	std::string online;	// "ONLINE" "OFFLINE"
	std::string status;	// 是否正常工作"OK"或"ERROR"
	std::string reason;	// 原因
	std::string encode;	// 是否编码 "ON"或"OFF"
	std::string record;	// 是否录像 "ON"或"OFF"
	std::string deviceTime; // 设备时间或日期
	std::vector<Alarmstatus> alarmStatus; // 报警设备状态列表
	std::string info;       // 扩展信息
}RespDeviceStatusParam;

typedef QueryRecordInfoParam RespRecordInfoItem;
typedef struct RespRecordInfoParam
{
	std::string name;	// 摄像机名称
	int sumNum;	        // 总历史视频数
	int recordListNum;	// 该次收到的历史视频片段数
	std::vector<RespRecordInfoItem> recordInfoItems;
}RespRecordInfoParam;

typedef struct RespConfigDownloadParam
{
	std::string result;

	// deviceConfig基本参数配置
	std::string name;		        // 设备名称
	std::string expiration;			// 注册过期时间
	std::string heartBeatInterval;	// 心跳间隔时间
	std::string heartBeatCount;		// 心跳超时次数
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
	std::string result; // 设备控制命令应答"OK"或"ERROR"
						// 报警通知应答"OK"或"ERROR"
						// 目录信息查询收到应答"OK"或"ERROR"
						// 设备配置应答"OK"或"ERROR"
						// 语音广播应答"OK"或"ERROR"

	RespCatalogParam        catalogParam;		 // 设备目录查询应答
	RespDeviceInfoParam     devInfoParam;		 // 设备信息查询应答
	RespDeviceStatusParam   devStatusParam;		 // 设备状态信息查询应答
	RespRecordInfoParam     recordInfoParam;	 // 文件目录检索应答
	RespConfigDownloadParam configDownloadParam; // 设备配置查询应答
	RespPresetParams        presetList;          // 设备预置位查询应答

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

// 上级查询本级目录创建目录结构使用
typedef struct CatalogTree
{
	int flag;		// 0：向上级汇报该组织目录
					// 1：向上级汇报该组织下摄像机目录
					// 2：移除该组织目录为rootVirOrgID赋值下一个组织编码
	int sn;			// 查询目录信息的命令序列号
	int startLine;	// 开始行，向上级汇报组织目录下摄像机时使用
	int cameraSumNum;	// 该组织下摄像机剩余总数
	time_t lastResponseTime;	// 最后一次目录回复200Ok的时间
	std::string protocol;		// 目录发送方式TCP、UDP
	std::string remoteSipID;	// 查询本平台目录的sipID
	std::string rootVirOrgID;	// 查询目录的根节点组织编码
	std::deque<std::string> virtualOrgIDs;	// 需要向上级汇报的组织编码

	int allItemSumNum = 0;
	std::set<int> orgGroupIDs; // 给上级分配的组的集合,GroupID
	std::set<int> orgCameraIDs;// 给上级分配的某个相机集合,ResID

	std::set<int> tempGroupIDs; // 需要向上级发送的所有组的ID
	std::set<int> tempCameraIDs;// 需要向上级发送的所有相机的ID

	std::deque<CatalogItemAndInfo> allCatalog; // 需要向上级发送的所有目录

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
 * 组信息结构体
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
 * 摄像机信息结构体
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
 * 平台信息结构体
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