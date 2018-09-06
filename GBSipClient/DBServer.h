#pragma once
#include <iostream>
#include <string>
#include <vector>

#define OTL_ODBC_MYSQL
#include "otlv4.h"

class DBServer
{
public:
	DBServer();
	virtual ~DBServer();

private:
	otl_connect db;
	static bool isLogin;

	int LoginDatabase();
	int LogoutDatabase();
	int CheckDB();
};