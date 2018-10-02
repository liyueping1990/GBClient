#include "SipDBServer.h"

bool SipDBServer::isLogin = false;

SipDBServer::SipDBServer()
{
	mDB;
	otl_connect m2DB;
}

SipDBServer::~SipDBServer()
{
}

int SipDBServer::LoginDatabase()
{
	int result = -1;
	try
	{
		const char* dbInfo = "DSN=VIS";
		mDB.rlogon(dbInfo);
		if (!isLogin)
		{
			//std::cout << "登录数据库成功！！！" << std::endl;
			isLogin = true;
			result = 0;
		}
	}
	catch (otl_exception& p)
	{
		std::cout << "登录数据库失败！！！" << std::endl;
	}

	return result;
}

int SipDBServer::LogoutDatabase()
{
	mDB.logoff();
	if (isLogin)
	{
		isLogin = false;
		//std::cout << "注销数据库成功！！！" << std::endl;
	}
	return 0;
}

int SipDBServer::CheckDB()
{
	bool isLogout = false;
	try
	{
		otl_stream o(1, "select PlatformID from tbl_platform", mDB);
		mDB.commit();
	}
	catch (otl_exception& p)
	{
		isLogout = true;
	}

	if (isLogout)
	{
		LogoutDatabase();
		LoginDatabase();
	}
	return 0;
}

int SipDBServer::GetCatalogRootNode(GroupInfoList & groupInfoList)
{
	bool isNeedCheckDB = false;
	std::string sql = "";
	sql = "select * from tbl_group where ParentGroupID = 0";

	// GroupInfo
	int  groupID = 0;
	int  type = 0;
	char deviceID[64] = { 0 };
	char name[64] = { 0 };
	char parentID[64] = { 0 };
	char bussinessGroupID[64] = { 0 };
	int  onNum = 0;
	int  sumNum = 0;
	int  parentGroupID = 0;

	do
	{
		if (isNeedCheckDB)
		{
			CheckDB();
		}

		try
		{
			otl_stream o(1, sql.c_str(), mDB);
			while (!o.eof())
			{
				std::shared_ptr<GroupInfo> groupInfo(new GroupInfo);

				o >> groupID;
				o >> type;
				o >> deviceID;
				o >> name;
				o >> parentID;
				o >> bussinessGroupID;
				o >> onNum;
				o >> sumNum;
				o >> parentGroupID;

				groupInfo->groupID = groupID;
				groupInfo->type = type;
				groupInfo->deviceID = deviceID;
				groupInfo->name = name;
				groupInfo->parentID = parentID;
				groupInfo->bussinessGroupID = bussinessGroupID;
				groupInfo->onNum = onNum;
				groupInfo->sumNum = sumNum;
				groupInfo->parentGroupID = parentGroupID;

				groupInfoList.push_back(groupInfo);
			}
			break;
		}
		catch (otl_exception& e)
		{
			isNeedCheckDB = !isNeedCheckDB;
			printf("%s\n", e.msg);
		}
	} while (isNeedCheckDB);

	return 0;
}

int SipDBServer::GetGroupChildren(std::string & deviceID, GroupInfoList & groupInfoList)
{
	int result = -1;

	std::string sql = "select GroupID from tbl_group where DeviceID = \'" + deviceID + "\'";
	int groupID = GetIntData(sql);
	if (groupID > 0)
	{
		GetGroupChildren(groupID, groupInfoList);
		result = 0;
	}
	return result;
}

int SipDBServer::GetGroupChildren(const int & pgroupID, GroupInfoList & groupInfoList)
{
	bool isNeedCheckDB = false;
	std::string sql = "";
	sql = "select * from tbl_group where ParentGroupID = \'" + std::to_string(pgroupID) + "\'";

	// GroupInfo
	int  groupID = 0;
	int  type = 0;
	char deviceID[64] = { 0 };
	char name[64] = { 0 };
	char parentID[64] = { 0 };
	char bussinessGroupID[64] = { 0 };
	int  onNum = 0;
	int  sumNum = 0;
	int  parentGroupID = 0;

	do
	{
		if (isNeedCheckDB)
		{
			CheckDB();
		}

		try
		{
			otl_stream o(1, sql.c_str(), mDB);
			while (!o.eof())
			{
				std::shared_ptr<GroupInfo> groupInfo(new GroupInfo);

				o >> groupID;
				o >> type;
				o >> deviceID;
				o >> name;
				o >> parentID;
				o >> bussinessGroupID;
				o >> onNum;
				o >> sumNum;
				o >> parentGroupID;

				groupInfo->groupID = groupID;
				groupInfo->type = type;
				groupInfo->deviceID = deviceID;
				groupInfo->name = name;
				groupInfo->parentID = parentID;
				groupInfo->bussinessGroupID = bussinessGroupID;
				groupInfo->onNum = onNum;
				groupInfo->sumNum = sumNum;
				groupInfo->parentGroupID = parentGroupID;

				groupInfoList.push_back(groupInfo);
			}
			break;
		}
		catch (otl_exception& e)
		{
			isNeedCheckDB = !isNeedCheckDB;
			printf("%s\n", e.msg);
		}
	} while (isNeedCheckDB);

	return 0;
}

int SipDBServer::GetCameraChildren(std::string & pDeviceID, CameraInfoList & cameraInfoList)
{
	bool isNeedCheckDB = false;
	std::string sql = "";
	if (pDeviceID.length() <= 10)
	{
		sql = "select DeviceID, Name, CivilCode, ParentID, Status from tbl_camera where CivilCode = \'" + std::string(pDeviceID) + "\'";
	} 
	else
	{
		sql = "select DeviceID, Name, CivilCode, ParentID, Status from tbl_camera where ParentID like \'%" + std::string(pDeviceID) + "\%'";
	}

	// GroupInfo
	char deviceID[64] = { 0 };
	char name[64] = { 0 };
	char civilCode = { 0 };
	char parentID[64] = { 0 };
	char status[16] = { 0 };

	do
	{
		if (isNeedCheckDB)
		{
			CheckDB();
		}

		try
		{
			otl_stream o(1, sql.c_str(), mDB);
			while (!o.eof())
			{
				std::shared_ptr<CameraInfo> cameraInfo(new CameraInfo);

				o >> deviceID;
				o >> name;
				o >> civilCode;
				o >> parentID;
				o >> status;

				cameraInfo->deviceID = deviceID;
				cameraInfo->name = name;
				cameraInfo->civilCode = civilCode;
				cameraInfo->parentID = parentID;
				cameraInfo->status = status;

				cameraInfoList.push_back(cameraInfo);
			}
			break;
		}
		catch (otl_exception& e)
		{
			isNeedCheckDB = !isNeedCheckDB;
			printf("%s\n", e.msg);
		}
	} while (isNeedCheckDB);

	return 0;
}

int SipDBServer::GetAllPlatfromInfo(PlatformInfoList & platformInfoList)
{
	bool isNeedCheckDB = false;
	std::string sql = "";
	sql = "select PlatformID, Name, DeviceID, IP, Port, ConnectState, Status from tbl_Platform";

	// PlatformInfo
	int  id = 0;
	char name[64] = { 0 };
	char deviceID[20] = { 0 };
	char ip[64] = { 0 };
	int  port = 0;
	int  upOrDown = 0;
	char status[4] = { 0 };

	do
	{
		if (isNeedCheckDB)
		{
			CheckDB();
		}

		try
		{
			otl_stream o(1, sql.c_str(), mDB);
			while (!o.eof())
			{
				std::shared_ptr<PlatformInfo> platformInfo(new PlatformInfo);

				o >> id;
				o >> name;
				o >> deviceID;
				o >> ip;
				o >> port;
				o >> upOrDown;
				o >> status;

				platformInfo->id = id;
				platformInfo->name = name;
				platformInfo->deviceID = deviceID;
				platformInfo->ip = ip;
				platformInfo->port = port;
				platformInfo->upOrDown = upOrDown;
				platformInfo->status = status;

				platformInfoList.push_back(platformInfo);
			}
			break;
		}
		catch (otl_exception& e)
		{
			isNeedCheckDB = !isNeedCheckDB;
			printf("%s\n", e.msg);
		}
	} while (isNeedCheckDB);

	return 0;
}

int SipDBServer::GetIntData(const std::string & sql)
{
	int result = -1;
	bool isNeedCheckDB = false;
	do
	{
		if (isNeedCheckDB)
		{
			CheckDB();
		}

		try
		{
			otl_stream o(1, sql.c_str(), mDB);
			while (!o.eof())
			{
				o >> result;				
			}
			break;
		}
		catch (otl_exception& e)
		{
			isNeedCheckDB = !isNeedCheckDB;
			printf("%s\n", e.msg);
		}
	} while (isNeedCheckDB);

	return result;
}
