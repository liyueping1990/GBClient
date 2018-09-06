#include "DBServer.h"

bool DBServer::isLogin = false;

DBServer::DBServer()
{
}


DBServer::~DBServer()
{
}

int DBServer::LoginDatabase()
{
	int result = -1;
	try
	{
		char* dbInfo = "DSN=AVSS";
		db.rlogon(dbInfo);
		if (!isLogin)
		{
			//std::cout << "��¼���ݿ�ɹ�������" << std::endl;
			isLogin = true;
			result = 0;
		}
	}
	catch (otl_exception& p)
	{
		std::cout << "��¼���ݿ�ʧ�ܣ�����" << std::endl;
	}

	return result;
}

int DBServer::LogoutDatabase()
{
	db.logoff();
	if (isLogin)
	{
		isLogin = false;
		//std::cout << "ע�����ݿ�ɹ�������" << std::endl;
	}
	return 0;
}

int DBServer::CheckDB()
{
	bool isLogout = false;
	try
	{
		otl_stream o(1, "select PlatformID from tbl_platform", db);
		db.commit();
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
