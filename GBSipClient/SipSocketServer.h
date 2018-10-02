#pragma once
#include <iostream>
#include <string>
#include <deque>
#include <thread>
#include <mutex>

#include <Winsock2.h>

class SipSocketServer
{
public:
	SipSocketServer();
	virtual ~SipSocketServer();

	void GetFirstSipMsg(std::string &sipMsg);

//private:
	SOCKET sipSocket;
	SOCKET streamingSocket;
	void RecvStreamingData();
	std::mutex mSipMsgMutex;
	std::deque<std::string> mSipMsgList;

	void InitSocket();
	void RecvData();
	void SendData(const char* sipMsg, const char* ip, const int port);
	void CloseSocket();
};

