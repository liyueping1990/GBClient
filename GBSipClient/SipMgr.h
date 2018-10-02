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
	SipDBServer* mSipDB;

	std::shared_ptr<PlatformInfo> mLocalPlatformInfo;

	int AddSipMsgToTrnFF(osip_transaction_t* trn, osip_message_t* &sipMsg);
	int AddEventToTrnFF(osip_transaction_t* trn, osip_event_t* &event);

	int QueryCatalog(std::string deviceID); // ��ѯ����ƽ̨���豸Ŀ¼��Ϣ
	int Play(std::string deviceID); // ʵʱ��Ƶ����
	int Playback(std::string deviceID); // ��ʷ��Ƶ����
	int Download(std::string deviceID); // ��ʷ��Ƶ����

private:		
	SipMgr();
	void Init();
	static SipMgr* instance;

	// ע��ص�����
	void RegisterSipMsgCallbacks();
	void RegisterCommonCallbacks();

	std::deque<osip_event_t*> mSipEvents;
	int CreateTransaction(osip_transaction_t* &transaction, osip_event_t* event);
	void ProcessIncomingSipMsg(std::string &sipMsg);
	void ProcessOutGoingSipMsg(osip_transaction_t* callTransaction, osip_event_t* &event);

	// ��ʱ��
	std::atomic<bool> mIsExit;
	std::thread mTrnTimer;
	void SipTransactionTimerExec();

};

