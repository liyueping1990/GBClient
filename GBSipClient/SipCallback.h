#pragma once
#include "SipMgr.h"

namespace SipCallback
{
	//int AddSipMsgToTrnFF(osip_transaction_t* trn, osip_message_t* &sipMsg);
	//int AddEventToTrnFF(osip_transaction_t* trn, osip_event_t* &event);

	// ����sip��Ϣ
	int osip_send_message_cb(osip_transaction_t* trn, osip_message_t* sipMsg, char* dstIP, int dstPort, int sendSock);
	void osip_rcv_message_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);

	// sip��Ϣ��ص�����
	void osip_nict_rcv_2xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_rcv_3xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_rcv_4xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_send_register_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_send_notify_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_nict_send_subscribe_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	void osip_ict_rcv_2xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ict_rcv_3xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ict_rcv_45xx_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ict_send_invite_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	void osip_ist_rcv_invite_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);
	void osip_ist_rcv_ack_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	void osip_nist_rcv_register_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_notify_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_subscribe_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_bye_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);
	void osip_nist_rcv_info_cb(int type, osip_transaction_t *trn, osip_message_t *sipMsg);

	// ״̬��ʱ����
	void osip_nict_status_timeout_cb(int type, osip_transaction_t* trn, osip_message_t* sipMsg);

	// transaction �ص�����
	void osip_ict_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_ist_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_nict_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_nist_kill_transaction_cb(int type, osip_transaction_t *trn);
	void osip_ict_transport_error_cb(int type, osip_transaction_t *, int error);
	void osip_ist_transport_error_cb(int type, osip_transaction_t *, int error);
	void osip_nict_transport_error_cb(int type, osip_transaction_t *, int error);
	void osip_nist_transport_error_cb(int type, osip_transaction_t *, int error);
};

