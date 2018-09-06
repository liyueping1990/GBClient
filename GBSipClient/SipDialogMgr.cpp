#include "SipDialogMgr.h"

int SipDialogMgr::mDialogID = 0;

SipDialogMgr::SipDialogMgr()
{
	
}


SipDialogMgr::~SipDialogMgr()
{
}

SipDialog SipDialogMgr::CreateSipDialog(osip_transaction_t * trn, osip_message_t *sipMsg, DialogType dialogType)
{
	osip_dialog_t* osipDialog = nullptr;
	DialogHint dialogHint = DialogHint::UAC;
	if (trn->ctx_type == ICT || trn->ctx_type == NICT)
	{
		dialogHint = DialogHint::UAC;
		osip_dialog_init_as_uac(&osipDialog, sipMsg);
	}
	else
	{
		dialogHint = DialogHint::UAS;
		osip_dialog_init_as_uas(&osipDialog, trn->orig_request, sipMsg);
	}

	if (osipDialog == nullptr)
	{
		return nullptr;
	}

	SipDialog sipDialog(new DialogInfo);
	sipDialog->sn = 0;
	sipDialog->dialogHint = dialogHint;
	sipDialog->dialogType = dialogType;
	sipDialog->birthTime = time(NULL);
	sipDialog->lastResponseTime = time(NULL);
	sipDialog->osipDialog = osipDialog;
	if (dialogHint == DialogHint::UAS) // 本级为服务端
	{
		sipDialog->localDeviceID = trn->orig_request->req_uri->username;
		sipDialog->remoteDeviceID = trn->orig_request->from->url->username;
	}
	else
	{
		sipDialog->localDeviceID = trn->orig_request->from->url->username;
		sipDialog->remoteDeviceID = trn->orig_request->req_uri->username;
	}

	mDialogIDMutex.lock();
	sipDialog->dialogID = ++mDialogID;
	mDialogIDMutex.unlock();
	InsertSipDialog(sipDialog);
	return sipDialog;
}

void SipDialogMgr::InsertSipDialog(SipDialog sipDialog)
{
	switch (sipDialog->dialogType)
	{
	case DialogType::REGISTER_DIALOG:
		mRegMutex.lock();
		mRegDialogList.push_back(sipDialog);
		mRegMutex.unlock();
		break;
	case DialogType::PLAY_DIALOG:
	case DialogType::PLAYBACK_DIALOG:
	case DialogType::DOWNLOAD_DIALOG:
	case DialogType::THIRD_PARTY_DIALOG:
	case DialogType::AUDIOBROADCAST_DIALOG:
		mInviteMutex.lock();
		mInviteDialogList.push_back(sipDialog);
		mInviteMutex.unlock();
		break;
	case DialogType::SUBSCRIBE_DIALOG:
		mSubEventMutex.lock();
		mSubEventDialogList.push_back(sipDialog);
		mSubEventMutex.unlock();
		break;
	}
}

int SipDialogMgr::RemoveSipDialog(SipDialog sipDialog)
{
	switch (sipDialog->dialogType)
	{
	case DialogType::REGISTER_DIALOG:
		mRegMutex.lock();
		RemoveSipDialog(mRegDialogList, sipDialog->dialogID);
		mRegMutex.unlock();
		break;
	case DialogType::PLAY_DIALOG:
	case DialogType::PLAYBACK_DIALOG:
	case DialogType::DOWNLOAD_DIALOG:
	case DialogType::THIRD_PARTY_DIALOG:
	case DialogType::AUDIOBROADCAST_DIALOG:
		mInviteMutex.lock();
		RemoveSipDialog(mInviteDialogList, sipDialog->dialogID);
		mInviteMutex.unlock();
		break;
	case DialogType::SUBSCRIBE_DIALOG:
		mSubEventMutex.lock();
		RemoveSipDialog(mSubEventDialogList, sipDialog->dialogID);
		mSubEventMutex.unlock();
		break;
	}
	return 0;
}


int SipDialogMgr::RemoveSipDialog(SipDialogList dialogList, int dialogID)
{
	for (auto iter = dialogList.begin(); iter != dialogList.end(); iter++)
	{
		if ((*iter)->dialogID == dialogID)
		{
			dialogList.erase(iter);
			break;
		}
	}
	return 0;
}

SipDialog SipDialogMgr::FindSipDialog(SipDialogList dialogList, int dialogID)
{
	SipDialog sipDialog = nullptr;
	for (auto iter = dialogList.begin(); iter != dialogList.end(); iter++)
	{
		if ((*iter)->dialogID == dialogID)
		{
			sipDialog = *iter;
			break;
		}
	}
	return sipDialog;
}

SipDialog SipDialogMgr::FindRegSipDialog(std::string remoteDeviceID, DialogHint dialogHint)
{
	SipDialog sipDialog = nullptr;
	mRegMutex.lock();
	for (auto var : mRegDialogList)
	{
		if (var->remoteDeviceID == remoteDeviceID && var->dialogHint == dialogHint)
		{
			sipDialog = var;
		}
	}
	mRegMutex.unlock();
	return sipDialog;
}

SipDialog SipDialogMgr::FindInviteSipDialog(std::string fromTag, std::string callID, DialogHint dialogHint)
{
	SipDialog sipDialog = nullptr;
	std::string tag = "";
	std::string id = "";

	mInviteMutex.lock();
	for (auto dialog : mInviteDialogList)
	{
		if (dialog->osipDialog == nullptr)
		{
			continue;
		}

		if (dialogHint == DialogHint::UAC)
		{
			tag = dialog->osipDialog->local_tag;
		}
		else
		{
			tag = dialog->osipDialog->remote_tag;
		}
		id = dialog->osipDialog->call_id;

		if (tag == fromTag && id == callID)
		{
			sipDialog = dialog;
			break;
		}
	}
	mInviteMutex.unlock();

	return sipDialog;
}

SipDialog SipDialogMgr::FindInviteSipDialog(int dialogID)
{
	SipDialog sipDialog = nullptr;

	mInviteMutex.lock();
	sipDialog = FindSipDialog(mInviteDialogList, dialogID);
	mInviteMutex.unlock();

	return sipDialog;
}

SipDialog SipDialogMgr::FindSipDialog(int dialogID)
{
	SipDialog sipDialog = nullptr;
	sipDialog = FindInviteSipDialog(dialogID);

	if (sipDialog == nullptr)
	{
		mRegMutex.lock();
		sipDialog = FindSipDialog(mRegDialogList, dialogID);
		mRegMutex.unlock();
	}

	if (sipDialog == nullptr)
	{
		mSubEventMutex.lock();
		sipDialog = FindSipDialog(mSubEventDialogList, dialogID);
		mSubEventMutex.unlock();
	}
	
	return sipDialog;
}


SipDialog SipDialogMgr::FindSipDialog(int dialogID, DialogType dialogType)
{
	SipDialog sipDialog = nullptr;
	switch (dialogType)
	{
	case DialogType::REGISTER_DIALOG:
		mRegMutex.lock();
		sipDialog = FindSipDialog(mRegDialogList, dialogID);
		mRegMutex.unlock();
		break;
	case DialogType::PLAY_DIALOG:
	case DialogType::PLAYBACK_DIALOG:
	case DialogType::DOWNLOAD_DIALOG:
	case DialogType::THIRD_PARTY_DIALOG:
	case DialogType::AUDIOBROADCAST_DIALOG:
		mInviteMutex.lock();
		sipDialog = FindSipDialog(mInviteDialogList, dialogID);
		mInviteMutex.unlock();
		break;
	case DialogType::SUBSCRIBE_DIALOG:
		mSubEventMutex.lock();
		sipDialog = FindSipDialog(mSubEventDialogList, dialogID);
		mSubEventMutex.unlock();
		break;
	}
	return sipDialog;
}
