#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "SipDefine.h"

#define OTL_ODBC_MYSQL
#include "otlv4.h"

class SipDBServer
{
public:
	SipDBServer();
	virtual ~SipDBServer();

	int GetCatalogRootNode(GroupInfoList &groupInfoList);
	int GetGroupChildren(std::string &pDeviceID, GroupInfoList &groupInfoList);
	int GetGroupChildren(const int &pGroupID, GroupInfoList &groupInfoList);
	int GetCameraChildren(std::string &pDeviceID, CameraInfoList &cameraInfoList);

	int GetAllPlatfromInfo(PlatformInfoList& platformInfoList);
	int GetLocalPlatformInfo(std::shared_ptr<PlatformInfo>& platformInfo);
	int GetPlatformInfo(std::string& deviceID, std::shared_ptr<PlatformInfo>& platformInfo);

	int GetIntData(const std::string& sql);

private:
	otl_connect mDB;
	static bool isLogin;

	int LoginDatabase();
	int LogoutDatabase();
	int CheckDB();
};