#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <WinSock2.h>
#include <Windows.h>
#include <string.h>
#include "MessageFormat.h"
#include "OmokPan.h"
#include <list>

#define BUF_SIZE 100
#define MAX_CLNT 256

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {  //buffer info
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int mode;
	bool isBlack;
	SOCKET hOppSock;
	
}PER_IO_DATA, *LPPER_IO_DATA;

unsigned WINAPI EchoThreadMain(LPVOID pComPort);
void ErrorHandling(const char *message);

SOCKET hWaitSocket= NULL;
int main() {
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;//get computer information(ex. core count)
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, i, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	//io 완료 확인을 위함
	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);//dwNumber~ :cpu core count
	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("WSASocket() error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(10000);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	while (1) {
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		handleInfo = new PER_HANDLE_DATA();
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);
		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);
		//대결 상대가 없는 경우
		if (hWaitSocket == NULL) {
			hWaitSocket = hClntSock;
			LPPER_IO_DATA ioInfo = new PER_IO_DATA();
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->mode = Message::NONE;//대기 상태
			WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf),
				1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
		}
		else {
			LPPER_IO_DATA ioInfo = new PER_IO_DATA();
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->mode = Message::START;
			ioInfo->hOppSock = hWaitSocket;
			hWaitSocket = NULL;
			WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf),
				1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
		}
	}
	return 0;
}
unsigned WINAPI EchoThreadMain(LPVOID pComPort)//main 과 동기화를 위해
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		//대기 
		GetQueuedCompletionStatus(hComPort //완료된 io 정보가 들어있는 핸들 전달
			, &bytesTrans //데이터 크기정보를 저장할 변수 주소 전달
			, (LPDWORD)&handleInfo //Create IO Completion Port 함수 세번째 인자값 저장을 위한 주소 전달
			, (LPOVERLAPPED*)&ioInfo//OVERLAPPED 구조체 저장될 주소 전달
			, INFINITE);//타임아웃
		sock = handleInfo->hClntSock;

		switch (ioInfo->mode)
		{
		case Message::NONE:
			if (bytesTrans <= 0) {//대기 종료할 경우
				hWaitSocket = NULL;
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				continue;
			}
			break;
		default:
			break;
		}
			//puts("message received!");
			//memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			//ioInfo->wsaBuf.len = bytesTrans;
			//ioInfo->rwMode = WRITE;
			//WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			//ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			//memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			//ioInfo->wsaBuf.len = BUF_SIZE;
			//ioInfo->wsaBuf.buf = ioInfo->buffer;
			//ioInfo->rwMode = READ;
			//WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		//}
		//else {
		//	//puts("message sent!");
		//	//puts(ioInfo->wsaBuf.buf);
		//	//free(ioInfo);
		//}
	}
	return 0;
}
void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

