#include "SipMgr.h"

SipMgr* SipMgr::instance = nullptr;

SipMgr* SipMgr::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SipMgr;
	}
	return instance;
}

SipMgr::SipMgr()
{
	int result = -1;

	mIsExit = false;
	mSipDialogMgr = new SipDialogMgr;
	mSipDB = new SipDBServer;
	
	mLocalPlatformInfo = std::make_shared<PlatformInfo>();
	result = mSipDB->GetLocalPlatformInfo(mLocalPlatformInfo);
	if (result)
	{
		std::cout << "无法获取本级平台信息" << std::endl;
		return;
	}
	SipMsgObj::mLocalPlatformInfo = mLocalPlatformInfo;

	Init();
}

SipMgr::~SipMgr()
{
	mIsExit = true;
	mTrnTimer.join();
}

int SipMgr::Start()
{
	//Init();

	auto run = [&]()
	{
		std::string sipMsg = "";
		while (true)
		{
			mSocketServer.GetFirstSipMsg(sipMsg);
			if (sipMsg.size())
			{
				ProcessIncomingSipMsg(sipMsg);
				sipMsg.clear();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::nanoseconds(1));
			}
		}
	};
	std::thread(run).detach();
	return 0;
}

void SipMgr::Init()
{
	osip_init(&sip);
	RegisterCommonCallbacks();
	RegisterSipMsgCallbacks();
	mTrnTimer = std::thread(&SipMgr::SipTransactionTimerExec, this);

	std::cout << "init is finished..." << std::endl;
}

void SipMgr::RegisterSipMsgCallbacks()
{
	// send message callback，发送sip消息函数
	osip_set_cb_send_message(sip, SipCallback::osip_send_message_cb);

	// receive message callback，收到sip消息后回调函数
	osip_set_message_callback(sip, OSIP_ICT_STATUS_2XX_RECEIVED, SipCallback::osip_ict_rcv_2xx_cb);
	osip_set_message_callback(sip, OSIP_ICT_STATUS_3XX_RECEIVED, SipCallback::osip_ict_rcv_3xx_cb);
	osip_set_message_callback(sip, OSIP_ICT_STATUS_4XX_RECEIVED, SipCallback::osip_ict_rcv_45xx_cb);
	osip_set_message_callback(sip, OSIP_ICT_STATUS_5XX_RECEIVED, SipCallback::osip_ict_rcv_45xx_cb);

	osip_set_message_callback(sip, OSIP_IST_INVITE_RECEIVED, SipCallback::osip_ist_rcv_invite_cb);
	osip_set_message_callback(sip, OSIP_IST_ACK_RECEIVED, SipCallback::osip_ist_rcv_ack_cb);

	osip_set_message_callback(sip, OSIP_NICT_STATUS_2XX_RECEIVED, SipCallback::osip_nict_rcv_2xx_cb);
	osip_set_message_callback(sip, OSIP_NICT_STATUS_3XX_RECEIVED, SipCallback::osip_nict_rcv_3xx_cb);
	osip_set_message_callback(sip, OSIP_NICT_STATUS_4XX_RECEIVED, SipCallback::osip_nict_rcv_4xx_cb);
	osip_set_message_callback(sip, OSIP_NICT_NOTIFY_SENT, SipCallback::osip_nict_send_notify_cb);
	osip_set_message_callback(sip, OSIP_NICT_SUBSCRIBE_SENT, SipCallback::osip_nict_send_subscribe_cb);
	osip_set_message_callback(sip, OSIP_NICT_REGISTER_SENT, SipCallback::osip_nict_send_register_cb);

	osip_set_message_callback(sip, OSIP_NIST_REGISTER_RECEIVED, SipCallback::osip_nist_rcv_register_cb);
	osip_set_message_callback(sip, OSIP_NIST_NOTIFY_RECEIVED, SipCallback::osip_nist_rcv_notify_cb);
	osip_set_message_callback(sip, OSIP_NIST_SUBSCRIBE_RECEIVED, SipCallback::osip_nist_rcv_subscribe_cb);
	osip_set_message_callback(sip, OSIP_NIST_BYE_RECEIVED, SipCallback::osip_nist_rcv_bye_cb);
	osip_set_message_callback(sip, OSIP_NIST_INFO_RECEIVED, SipCallback::osip_nist_rcv_info_cb);

	osip_set_message_callback(sip, OSIP_NIST_UNKNOWN_REQUEST_RECEIVED, SipCallback::osip_rcv_message_cb);

	// timeout callback
	osip_set_message_callback(sip, OSIP_NICT_STATUS_TIMEOUT, SipCallback::osip_nict_status_timeout_cb);
}

void SipMgr::RegisterCommonCallbacks()
{
	// kill transaction callback
	osip_set_kill_transaction_callback(sip, OSIP_ICT_KILL_TRANSACTION, SipCallback::osip_ict_kill_transaction_cb);
	osip_set_kill_transaction_callback(sip, OSIP_IST_KILL_TRANSACTION, SipCallback::osip_ist_kill_transaction_cb);
	osip_set_kill_transaction_callback(sip, OSIP_NICT_KILL_TRANSACTION, SipCallback::osip_nict_kill_transaction_cb);
	osip_set_kill_transaction_callback(sip, OSIP_NIST_KILL_TRANSACTION, SipCallback::osip_nist_kill_transaction_cb);

	// transport error callback
	osip_set_transport_error_callback(sip, OSIP_ICT_TRANSPORT_ERROR, SipCallback::osip_ict_transport_error_cb);
	osip_set_transport_error_callback(sip, OSIP_IST_TRANSPORT_ERROR, SipCallback::osip_ist_transport_error_cb);
	osip_set_transport_error_callback(sip, OSIP_NICT_TRANSPORT_ERROR, SipCallback::osip_nict_transport_error_cb);
	osip_set_transport_error_callback(sip, OSIP_NIST_TRANSPORT_ERROR, SipCallback::osip_nist_transport_error_cb);
}

int SipMgr::CreateTransaction(osip_transaction_t *& transaction, osip_event_t * event)
{
	bool isInvite = false;

	if (event == nullptr)
	{
		return -1;
	}

	// 收到请求
	if (EVT_IS_INCOMINGREQ(event))
	{
		if (EVT_IS_RCV_INVITE(event))
		{
			osip_transaction_init(&transaction, IST, sip, event->sip);
			//transaction->state = IST_PRE_PROCEEDING;
			isInvite = true;
		}
		else if (EVT_IS_RCV_ACK(event))
		{
			osip_transaction_init(&transaction, IST, sip, event->sip);
			transaction->state = IST_COMPLETED;
		}
		else // EVT_IS_RCV_REQUEST
		{
			osip_transaction_init(&transaction, NIST, sip, event->sip);
			//transaction->state = NIST_PRE_TRYING;
		}
	}

	// 发出请求
	if (EVT_IS_OUTGOINGREQ(event))
	{
		if (EVT_IS_SND_INVITE(event))
		{
			osip_transaction_init(&transaction, ICT, sip, event->sip);
			//transaction->state = ICT_PRE_CALLING;
			isInvite = true;
		}
		else if (EVT_IS_SND_ACK(event))
		{
			osip_transaction_init(&transaction, ICT, sip, event->sip);
			//transaction->state = ICT_PRE_CALLING;
		}
		else // EVT_IS_SND_REQUEST
		{
			osip_transaction_init(&transaction, NICT, sip, event->sip);
			//transaction->state = NICT_PRE_TRYING;
		}
	}

	// 创建transaction是否成功
	if (transaction == nullptr)
	{
		return -1;
	}
	else // transaction != nullptr
	{
		AppContext* appContext = new AppContext;
		/*//appContext->sip = sip;
		//appContext->mSipDialogs = mSipDialogs;
		appContext->callTransaction = nullptr;

		if (isInvite)
		{
			SipMsgParser sipMsgParser;

			sipMsgParser.ParserSipMsgBody(event->sip);
			if (transaction->ctx_type == ICT || transaction->ctx_type == IST) // 确定码流接收方ip、port
			{
				appContext->mRecvIP = sipMsgParser.mSdpParam.cParam.addr;
				appContext->mRecvPort = sipMsgParser.mSdpParam.mParam.port;
			}
		}*/
		transaction->your_instance = (void*)appContext;
	}
	return 0;
}

int SipMgr::AddSipMsgToTrnFF(osip_transaction_t * trn, osip_message_t * &sipMsg)
{
	int result = -1;
	osip_event_t* event = osip_new_outgoing_sipmessage(sipMsg);
	if (event == nullptr)
	{
		osip_message_free(sipMsg);
		sipMsg = nullptr;
	}
	else if (trn)
	{
		result = osip_fifo_add(trn->transactionff, event);
	}
	else // trn == nullptr && event != nullptr
	{
		//osip_transaction_t * trn = nullptr;
		CreateTransaction(trn, event); 
		AddEventToTrnFF(trn, event);
	}
	return result;
}

int SipMgr::AddEventToTrnFF(osip_transaction_t * trn, osip_event_t *& event)
{
	int result = -1;
	if (event && trn)
	{
		result = osip_fifo_add(trn->transactionff, event);
	}
	return result;
}

int SipMgr::QueryCatalog(std::string deviceID)
{
	SipMsgBuilder sipMsgBuilder;
	
	// 获取平台信息 
	std::shared_ptr<PlatformInfo> platformInfo = std::make_shared<PlatformInfo>();
	int result = mSipDB->GetPlatformInfo(deviceID, platformInfo);
	if (result)
	{
		std::cout << "获取信息失败" << std::endl;
	}
	sipMsgBuilder.SetPlatformInfo(platformInfo);
	
	// 设置创建消息类型
	std::shared_ptr<XmlParam> xmlParam = std::make_shared<XmlParam>();
	xmlParam->deviceID = deviceID;
	xmlParam->manscdpType = MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_CATALOG;
	xmlParam->sn = 1;
	xmlParam->queryParam.queryCatalogParam.startTime;
	xmlParam->queryParam.queryCatalogParam.endTime;
	result = sipMsgBuilder.SetXmlParam(xmlParam);
	
	osip_message_t* dstSipMsg = nullptr;
	sipMsgBuilder.CreateSipMsgXml(dstSipMsg);

	AddSipMsgToTrnFF(nullptr, dstSipMsg);
	return 0;
}

int SipMgr::Play(std::string deviceID)
{
	SipMsgBuilder sipMsgBuilder;

	// 获取平台信息 
	std::shared_ptr<PlatformInfo> platformInfo = std::make_shared<PlatformInfo>();
	int result = mSipDB->GetPlatformInfo(deviceID, platformInfo);
	if (result)
	{
		std::cout << "获取信息失败" << std::endl;
	}
	sipMsgBuilder.SetPlatformInfo(platformInfo);

	std::shared_ptr<SdpParam> sdpParam = std::make_shared<SdpParam>();
	sdpParam->oParam.addr = "192.168.1.110";
	sdpParam->oParam.userName = "34010000002005000001";
	sdpParam->cParam.addr = "192.168.1.110";
	sdpParam->mParam.port = 8000;
	sdpParam->mParam.isRecvOnly = true;
	sdpParam->y = "0001";
	sipMsgBuilder.SetSdpParam(sdpParam);

	osip_message_t* dstMsg = nullptr;
	sipMsgBuilder.CreateInviteSipMsg(nullptr, dstMsg);

	char* msg = nullptr;
	osip_message_to_str(dstMsg, &msg, 0);
	osip_free(msg);
	return 0;
}

int SipMgr::Playback(std::string deviceID)
{
	return 0;
}

int SipMgr::Download(std::string deviceID)
{
	return 0;
}

void SipMgr::ProcessIncomingSipMsg(std::string & sipMsg)
{
	int a = -1;
	size_t msgLen = sipMsg.length();
	osip_event_t* event = osip_parse(sipMsg.c_str(), msgLen);
	if (event == nullptr)
	{
		return;
	}

	osip_transaction_t* transaction = __osip_find_transaction(sip, event, 0);
	//osip_find_transaction_and_add_event(sip, event);

	// 收到reponse信息
	if (EVT_IS_INCOMINGRESP(event) && transaction == nullptr)
	{
		return;
	}

	// 收到request消息
	if (transaction == nullptr)
	{
		CreateTransaction(transaction, event);
	}

	if (transaction != nullptr)
	{
		osip_transaction_execute(transaction, event);
		for (auto& var : mSipEvents)
		{
			if (var == nullptr || var->sip == nullptr)
			{
				continue;
			}
			else if (var->sip->status_code > 0)// response message，无callTransaction,
			{
				ProcessOutGoingSipMsg(nullptr, var);
			}
			else
			{
				ProcessOutGoingSipMsg(transaction, var);
			}
		}
		mSipEvents.clear();
	}
}

void SipMgr::ProcessOutGoingSipMsg(osip_transaction_t * callTransaction, osip_event_t *& event)
{
	osip_transaction_t* transaction = __osip_find_transaction(sip, event, 0);

	if (transaction == nullptr)
	{
		CreateTransaction(transaction, event);
	}

	if (transaction != nullptr)
	{
		osip_transaction_execute(transaction, event);

		if (callTransaction != nullptr)
		{
			AppContext* appContext = (AppContext*)transaction->your_instance;
			appContext->callTransaction = callTransaction;
		}
	}
	else
	{
		osip_event_free(event);
		event = nullptr;
	}
}

void SipMgr::SipTransactionTimerExec()
{
	while (!mIsExit)
	{
		osip_timers_ict_execute(sip);
		osip_timers_nict_execute(sip);
		osip_timers_ist_execute(sip);
		osip_timers_nist_execute(sip);

		osip_ict_execute(sip);
		osip_nict_execute(sip);
		osip_ist_execute(sip);
		osip_nist_execute(sip);

		//osip_retransmissions_execute(sip);
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}
