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

	auto RecvData1 = [&]() {
		while (true)
		{
			SipSocketServer::RecvStreamingData();
		}
	};
	std::thread(RecvData1).detach();
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
	// 加载套接字库
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

	// 创建套接字
	sipSocket = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8060);	
	bind(sipSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));// 绑定套接字

	streamingSocket = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv1;
	addrSrv1.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv1.sin_family = AF_INET;
	addrSrv1.sin_port = htons(8000);
	bind(streamingSocket, (SOCKADDR*)&addrSrv1, sizeof(SOCKADDR));// 绑定套接字
	
}


void SipSocketServer::RecvStreamingData()
{
	// 等待并接收数据
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char recvBuf[1024 * 4] = { 0 };
	recvfrom(streamingSocket, recvBuf, 4096, 0, (SOCKADDR*)&addrClient, &len);
	
	//std::string str = recvBuf;
	//std::cout << "recv streaming data:" << str.length() << std::endl;
}

void SipSocketServer::RecvData()
{
	// 等待并接收数据
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char recvBuf[1024 * 4] = { 0 };
	recvfrom(sipSocket, recvBuf, 4096, 0, (SOCKADDR*)&addrClient, &len);
	//printf("%s\n", recvBuf);
	//std::cout << recvBuf << std::endl;

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
	// 发送数据
	sendto(sipSocket, sipMsg, strlen(sipMsg), 0,
		(SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	//printf("%s\n", sipMsg);
	//std::cout << sipMsg << std::endl;
}

void SipSocketServer::CloseSocket()
{
	// 关闭套接字
	closesocket(sipSocket);
	WSACleanup();
}

