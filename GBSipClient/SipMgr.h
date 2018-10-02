#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <atomic>

#include "SipSocketServer.h"
#include "SipMsgObj.h"
#include "SipCallback.h"
#include "SipDialogMgr.h"
#include "SipDBServer.h"

struct CommentObj
{
	int type;
	void* param1;
	void* param2;
	void* param3;
	void* param4;
};
#define CommentObjList std::vector<CommentObj>

struct AppContext
{
	MESSAGE_MANSCDP_TYPE manscdpType;
	int sn;
	std::string xmlDeviceID;
	osip_transaction_t* callTransaction;

	std::string mRecvIP;
	int mRecvPort;
	std::string mSendIP;
	int mSendPort;
};

class SipMgr
{
public:
	static SipMgr* GetInstance();
	virtual ~SipMgr();

	int Start();

public:
	osip_t* sip;
	SipSocketServer mSocketServer;
	SipDialogMgr* mSipDialogMgr;

	int CreateTransaction(osip_transaction_t* &transaction, osip_event_t* event);

	int QueryCatalog(std::string deviceID); // 查询国标平台或设备目录信息
private:
		
	SipMgr();
	void Init();
	static SipMgr* instance;

	// 注册回调函数
	void RegisterSipMsgCallbacks();
	void RegisterCommonCallbacks();

	std::deque<osip_event_t*> mSipEvents;
	void ProcessIncomingSipMsg(std::string &sipMsg);
	void ProcessOutGoingSipMsg(osip_transaction_t* callTransaction, osip_event_t* &event);

	// 定时器
	std::atomic<bool> mIsExit;
	std::thread mTrnTimer;
	void SipTransactionTimerExec();

};

