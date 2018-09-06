#include "GBSipClient.h"
#include <QtWidgets/QApplication>

#if 1
int main(int argc, char *argv[])
{
	SipMgr* sipClient = SipMgr::GetInstance();
	sipClient->Start();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	//QApplication a(argc, argv);
	//GBSipClient w;
	//w.show();
	//return a.exec();
}
#else
using namespace std;
LONG  g_nPort = -1;
#define READ_BUF_SIZE 200
#define PRIVT_HEAD_LEN 40

void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize,
	FRAME_INFO * pFrameInfo,
	long nReserved1, long /*nReserved2*/)
{

	//---------------------------------------
	// 获取解码后音频数据
	if (pFrameInfo->nType == T_AUDIO16)
	{
		printf("test:: get audio data !\n");
	}

	//---------------------------------------
	// 获取解码后视频数据
	else if (pFrameInfo->nType == T_YV12)
	{
		//printf("test:: get video data !\n");
	}
}


using namespace std;

typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();
PROCGETCONSOLEWINDOW GetConsoleWindow0;

void main()
{
	//SipMgr* sipClient = SipMgr::GetInstance();
	//sipClient->Start();

	BOOL bFlag = FALSE;
	int  nError = 0;
	FILE* fp = NULL;
	char* pBuffer = NULL;

	//---------------------------------------
	// 获取控制台窗口句柄
	HMODULE hKernel32 = GetModuleHandle(L"kernel32");
	GetConsoleWindow0 = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");


	//---------------------------------------
	// 获取播放库通道号
	PlayM4_GetPort(&g_nPort);

	//打开待解码文件
	fp = fopen("test.mp4", ("rb"));
	if (fp == NULL)
	{
		printf("cannot open the file !\n");
		return;
	}


	pBuffer = new char[READ_BUF_SIZE];
	if (pBuffer == NULL)
	{
		return;
	}

	//读取文件中文件头
	fread(pBuffer, PRIVT_HEAD_LEN, 1, fp);

	//设置流模式类型 
	PlayM4_SetStreamOpenMode(g_nPort, STREAME_REALTIME);
	//打开流模式
	PlayM4_OpenStream(g_nPort, (PBYTE)pBuffer, PRIVT_HEAD_LEN, 1024 * 1024);

	//设置解码回调
	//PlayM4_SetDecCallBackExMend(g_nPort, DecCBFun, NULL, 0, NULL);

	//---------------------------------------
	// 获取显示窗口句柄
	HWND hWnd = GetConsoleWindow0();
	PlayM4_Play(g_nPort, hWnd);

	//////////////////////////////////////////////////////////////////////////
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
	SOCKET sipSocket = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8268);

	// 绑定套接字
	bind(sipSocket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	while (true)
	{
		// 等待并接收数据
		SOCKADDR_IN addrClient;
		int len = sizeof(SOCKADDR);
		char recvBuf[1024 * 4] = { 0 };
		recvfrom(sipSocket, recvBuf, 4096, 0, (SOCKADDR*)&addrClient, &len);
		//printf("%s\n", recvBuf);

		while (1)
		{
			int length = strlen(recvBuf);
			bFlag = PlayM4_InputData(g_nPort, (PBYTE)recvBuf, length);
			if (bFlag == FALSE)
			{
				nError = PlayM4_GetLastError(g_nPort);

				//若缓冲区满，则重复送入数据
				if (nError == PLAYM4_BUF_OVER)
				{
					Sleep(2);
					continue;
				}
			}

			//若送入成功，则继续读取数据送入到播放库缓冲
			break;
		}
	}


	Sleep(1000);
	//---------------------------------------
	// 停止解码
	PlayM4_Stop(g_nPort);

	//关闭流，回收源数据缓冲
	PlayM4_CloseStream(g_nPort);

	//释放播放库端口号
	PlayM4_FreePort(g_nPort);

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}


	if (pBuffer != NULL)
	{
		delete[] pBuffer;
		pBuffer = NULL;
	}
	return;
}
#endif