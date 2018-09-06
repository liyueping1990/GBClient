#include "SipCallback.h"

int SipCallback::AddSipMsgToTrnFF(osip_transaction_t * trn, osip_message_t * &sipMsg)
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
		osip_transaction_t * trn = nullptr;
		SipMgr::GetInstance()->CreateTransaction(trn, event);
		result = osip_fifo_add(trn->transactionff, event);
	}
	return result;
}

int SipCallback::AddEventToTrnFF(osip_transaction_t * trn, osip_event_t *& event)
{
	int result = -1;
	if (event && trn)
	{
		result = osip_fifo_add(trn->transactionff, event);
	}
	return result;
}

int SipCallback::osip_send_message_cb(osip_transaction_t * trn, osip_message_t * sipMsg, char * dstIP, int dstPort, int sendSock)
{
	char* dd = nullptr;
	osip_message_to_str(sipMsg, &dd, nullptr);
	SipMgr::GetInstance()->mSocketServer.SendData(dd, dstIP, dstPort);
	osip_free(dd);
	return 0;
}

void SipCallback::osip_rcv_message_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}

	SipMsgParser sipMsgParser;
	auto xmlParam = sipMsgParser.GetXmlParam(sipMsg);
	if (xmlParam == nullptr) // 400
	{
		return;
	}

	SipMsgBuilder sipMsgBuilder;
	osip_message_t* responseSipMsg = nullptr;
	switch (xmlParam->manscdpType)
	{
	case MESSAGE_MANSCDP_ERROR:
		break;
	case CONTROL_DEVICE_PTZ:
		break;
	case CONTROL_TELE_BOOT:
		break;
	case CONTROL_RECORD:
		break;
	case CONTROL_GUARD:
		break;
	case CONTROL_ALARM:
		break;
	case CONTROL_IFAME:
		break;
	case CONTROL_DRAG_ZOOM:
		break;
	case CONTROL_HOME_POSITION:
		break;
	case CONTROL_DEVICE_CONFIG:
		break;
	case QUERY_DEVICE_STATUS:
		break;
	case QUERY_DEVICE_CATALOG:
		break;
	case QUERY_DEVICE_INFO:
		break;
	case QUERY_RECORD_INFO:
		break;
	case QUERY_ALARM:
		break;
	case QUERY_CONFIG_DOWNLOAD:
		break;
	case QUERY_PRESET_QUERY:
		break;
	case QUERY_MOBILE_POSITION:
		break;
	case NOTIFY_KEEPALIVE:
	{
		sipMsgBuilder.CreateXxxSipMsg(sipMsg, responseSipMsg, 200);
		AddSipMsgToTrnFF(trn, responseSipMsg);

		static int i = 0;
		i++;
		if (i == 1) // 调看视频
		{
			SipMsgBuilder inviteBuilder;
			osip_message_t* inviteSipMsg = nullptr;
			SdpParam* sdpParam = new SdpParam;
			sdpParam->oParam.addr = "192.168.1.110";
			sdpParam->oParam.userName = "34010000002005000001";
			sdpParam->cParam.addr = "192.168.1.110";
			sdpParam->mParam.port = 8000;
			sdpParam->mParam.isRecvOnly = true;
			inviteBuilder.CreateInviteSipMsg(inviteSipMsg, sdpParam);

			char* strMsg = nullptr;
			osip_message_to_str(inviteSipMsg, &strMsg, 0);
			osip_free(strMsg);

			AddSipMsgToTrnFF(nullptr, inviteSipMsg);
		}
	}
		break;
	case NOTIFY_ALARM:
		break;
	case NOTIFY_MEDIA_STATUS:
		break;
	case NOTIFY_BROADCAST:
		break;
	case NOTIFY_MOBILE_POSITION:
		break;
	case NOTIFY_CATALOG:
		break;
	case RESPONSE_DEVICE_CONTROL:
		break;
	case RESPONSE_ALARM_NOTIFY:
		break;
	case RESPONSE_DEVICE_CATALOG_QUERY:
		break;
	case RESPONSE_CATALOG_QUERY_RECEIVED:
		break;
	case RESPONSE_CATALOG_RECEIVED:
		break;
	case RESPONSE_DEVICE_INFO_QUERY:
		break;
	case RESPONSE_DEVICE_STATUS_QUERY:
		break;
	case RESPONSE_DOCUMENT_RECORD_INFO:
		break;
	case RESPONSE_DEVICE_CONFIG:
		break;
	case RESPONSE_DEVICE_CONFIG_DOWNLOAD:
		break;
	case RESPONSE_DEVICE_PRESET_QUERY:
		break;
	case RESPONSE_BROADCAST:
		break;
	case CATALOG_CHANGED:
		break;
	case XML_COUNT:
		break;
	default:
		break;
	}

	if (xmlParam->reqRespType == "Control")
	{
	} 
	else if (xmlParam->reqRespType == "Query")
	{
	} 
	else if (xmlParam->reqRespType == "Notify")
	{
	} 
	else if (xmlParam->reqRespType == "Response")
	{
	}
}

void SipCallback::osip_nict_rcv_2xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_rcv_3xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_rcv_4xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_register_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_notify_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_subscribe_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_2xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn->your_instance == nullptr)
	{
		return;
	}

	SipMgr* sipMgr = SipMgr::GetInstance();
	AppContext* appContext = (AppContext*)trn->your_instance;

	// 向下级回复ack
	SipMsgBuilder sipMsgBuilder;
	osip_message_t* ackMsg = nullptr;
	sipMsgBuilder.CreateAckSipMsg(sipMsg, ackMsg);
	osip_send_message_cb(trn, ackMsg, trn->ict_context->destination, trn->ict_context->port, trn->out_socket);
}

void SipCallback::osip_ict_rcv_3xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_45xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_send_invite_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ist_rcv_invite_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ist_rcv_ack_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_register_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	//if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	//{
	//	return;
	//}
	SipMgr* sipInstance = SipMgr::GetInstance();


	SipMsgBuilder sipMsgBuilder;
	SipMsgParser sipMsgParser;
	osip_message_t* dstMsg = nullptr;
	osip_header_t *expires = nullptr;
	osip_message_get_expires(sipMsg, 0, &expires);
	if (expires == nullptr) // 不存在expires字段
	{
		return;
	}

	SipDialog registerDialog = nullptr;
	if (!strcmp(expires->hvalue, "0")) // 注销)
	{
	}
	else
	{
		if (registerDialog == nullptr) // 平台第一次向本级注册
		{
			if (sipMsg->authorizations.node == nullptr) // 不带鉴权, 401
			{
				std::string nonce = SipCommon::RandLengthStr(10);
				sipMsgBuilder.CreateRegister401SipMsg(sipMsg, dstMsg, nonce);
				AddSipMsgToTrnFF(trn, dstMsg);
			}
			else // 带鉴权
			{
				bool isAuth = true;
				// 对鉴权信息进行验证

				if (isAuth) // 通过验证, 200
				{
					// 回复带date的200
					sipMsgBuilder.CreateRegister200SipMsg(sipMsg, dstMsg);

					// 设置平台状态为 “ON”

					// 创建regDialog
					SipDialog regSipDialog = sipInstance->mSipDialogMgr->CreateSipDialog(trn, dstMsg, DialogType::REGISTER_DIALOG);

					osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg);
					event->transactionid = trn->transactionid;
					osip_fifo_add(trn->transactionff, event);
				}
				else // 验证失败，403
				{
					sipMsgBuilder.CreateXxxSipMsg(sipMsg, dstMsg, 403);
				}

			}
		}
		else // 已注册，刷新注册
		{
		}
	}
}

void SipCallback::osip_nist_rcv_notify_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_subscribe_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_bye_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_info_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_status_timeout_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_ist_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_nict_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_nist_kill_transaction_cb(int type, osip_transaction_t * trn)
{
	int more_event = 1;
	osip_event_t* se = nullptr;
	do {
		se = (osip_event_t *)osip_fifo_tryget(trn->transactionff);
		if (se == NULL)           /* no more event for this transaction */
			more_event = 0;
		else
		{
			osip_event_free(se);
			se = nullptr;
			//osip_transaction_execute(transaction, se);
		}		
	} while (more_event == 1);

	osip_t* sip = (osip_t*)trn->config;
	osip_list_remove(&sip->osip_nist_transactions, type);
}

void SipCallback::osip_ict_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_ist_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_nict_transport_error_cb(int type, osip_transaction_t *, int error)
{

}

void SipCallback::osip_nist_transport_error_cb(int type, osip_transaction_t *, int error)
{
}
