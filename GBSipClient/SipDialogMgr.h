#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <mutex>

#include <osip2/osip_dialog.h>

#define IntList std::list<int>

typedef enum class DialogType
{
	REGISTER_DIALOG = 0,    // 注册
	PLAY_DIALOG = 1,        // 实时视频
	PLAYBACK_DIALOG = 2,    // 历史回放
	DOWNLOAD_DIALOG = 3,    // 历史下载
	THIRD_PARTY_DIALOG = 4, // 第三方呼叫
	AUDIOBROADCAST_DIALOG = 5, // 语音广播 
	SUBSCRIBE_DIALOG = 6       // 事件订阅
}DialogType;

typedef enum class DialogHint
{
	UAC,
	UAS
}DialogHint;

typedef enum class StreamDirection
{
	SENDER = 0,
	RECEIVER = 1
}StreamDirection;

typedef struct DialogInfo
{
	int dialogID; // dialog编号
	int sn;       // Message消息sn编号
	DialogHint dialogHint; // dialog客户端/服务端
	DialogType dialogType; // dialog类型
	time_t birthTime; // dialog创建时间
	time_t lastResponseTime; // 最后收到keepallive时间
	int expireTime; // dialog有效期

	std::string localDeviceID;
	std::string remoteDeviceID;

	int callerDialogID; // 上级dialogID
	int calleeDialogID; // 下级dialogID

	std::string recvIP;
	int recvPort;
	std::string sendIP;
	int sendPort;

	int startTime;
	int endTime;
	IntList inviteDialogIDs;
	osip_dialog_t* osipDialog;
	~DialogInfo()
	{
		osip_dialog_free(osipDialog);
		osipDialog = nullptr;
	}
}DialogInfo;
#define SipDialog std::shared_ptr<DialogInfo>
#define SipDialogList std::list<SipDialog>

class SipDialogMgr
{
public:
	SipDialogMgr();
	virtual ~SipDialogMgr();

	// 创建、销毁dialog
	SipDialog CreateSipDialog(osip_transaction_t* trn, osip_message_t* sipMsg, DialogType dialogType);
	void InsertSipDialog(SipDialog sipDialog);
	int RemoveSipDialog(SipDialog sipDialog);

	// 查找SipDialog
	SipDialog FindRegSipDialog(std::string remoteDeviceID, DialogHint dialogHint);
	SipDialog FindInviteSipDialog(std::string fromTag, std::string callID, DialogHint dialogHint);
	SipDialog FindInviteSipDialog(int dialogID);
	SipDialog FindSipDialog(int dialogID);
	SipDialog FindSipDialog(int dialogID, DialogType dialogType);

private:
	static int mDialogID;
	std::mutex mDialogIDMutex;

	std::mutex mRegMutex;
	SipDialogList mRegDialogList;

	std::mutex mInviteMutex;
	SipDialogList mInviteDialogList;

	std::mutex mSubEventMutex;
	SipDialogList mSubEventDialogList;
	
	int RemoveSipDialog(SipDialogList dialogList, int dialogID);
	SipDialog FindSipDialog(SipDialogList dialogList, int dialogID);
};

