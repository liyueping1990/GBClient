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
	REGISTER_DIALOG = 0,    // ע��
	PLAY_DIALOG = 1,        // ʵʱ��Ƶ
	PLAYBACK_DIALOG = 2,    // ��ʷ�ط�
	DOWNLOAD_DIALOG = 3,    // ��ʷ����
	THIRD_PARTY_DIALOG = 4, // ����������
	AUDIOBROADCAST_DIALOG = 5, // �����㲥 
	SUBSCRIBE_DIALOG = 6       // �¼�����
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
	int dialogID; // dialog���
	int sn;       // Message��Ϣsn���
	DialogHint dialogHint; // dialog�ͻ���/�����
	DialogType dialogType; // dialog����
	time_t birthTime; // dialog����ʱ��
	time_t lastResponseTime; // ����յ�keepalliveʱ��
	int expireTime; // dialog��Ч��

	std::string localDeviceID;
	std::string remoteDeviceID;

	int callerDialogID; // �ϼ�dialogID
	int calleeDialogID; // �¼�dialogID

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

	// ����������dialog
	SipDialog CreateSipDialog(osip_transaction_t* trn, osip_message_t* sipMsg, DialogType dialogType);
	void InsertSipDialog(SipDialog sipDialog);
	int RemoveSipDialog(SipDialog sipDialog);

	// ����SipDialog
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

