#include "SipSocketServer.h"



SipSocketServer::SipSocketServer()
{
	InitSocket();

	auto RecvData = [&]() {
		while (true)
		{
			SipSocketServer::RecvData();
		}
	};

	std::thread(RecvData).detach();
}


SipSocketServer::~SipSocketServer()
{
}

void SipSocketServer::GetFirstSipMsg(std::string & sipMsg)
{
	mSipMsgMutex.lock();
	if (mSipMsgList.size())
	{
		sipMsg = std::move(mSipMsgList[0]);
		mSipMsgList.pop_front();
	}
	mSipMsgMutex.unlock();
}

void SipSocketServer::InitSocket()
{
	// �����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return;
	}

	// �����׽���
	sipSocket = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8060);

	// ���׽���
	bind(sipSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
}

void SipSocketServer::RecvData()
{
	// �ȴ�����������
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char recvBuf[1024 * 4] = { 0 };
	recvfrom(sipSocket, recvBuf, 4096, 0, (SOCKADDR*)&addrClient, &len);
	printf("%s\n", recvBuf);

	mSipMsgMutex.lock();
	mSipMsgList.push_back(recvBuf);
	mSipMsgMutex.unlock();
}

void SipSocketServer::SendData(const char * sipMsg, const char * ip, const int port)
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	// ��������
	sendto(sipSocket, sipMsg, strlen(sipMsg), 0,
		(SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	printf("%s\n", sipMsg);
}

void SipSocketServer::CloseSocket()
{
	// �ر��׽���
	closesocket(sipSocket);
	WSACleanup();
}

