#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <WinSock2.h>
#include <Windows.h>

#define BUF_SIZE 100
#define WRITE 3
#define READ 5

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;
}PER_IO_DATA, *LPPER_IO_DATA;

void ErrorHandling(const char* message);
unsigned WINAPI EchoThreadMain(LPVOID pComPort);

int main(){
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int recvBytes, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);
	int processCnt = sysInfo.dwNumberOfProcessors;
	for (int i = 0; i < processCnt; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hServSock == INVALID_SOCKET) 
		ErrorHandling("WSASocket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(10000);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	while (1) {
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);
		
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		if (hClntSock == INVALID_SOCKET) {
			ErrorHandling("accept() error");
		}
		handleInfo = new PER_HANDLE_DATA();
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);
		
		ioInfo = new PER_IO_DATA();
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock
			, &(ioInfo->wsaBuf)
			, 1
			, (LPDWORD)&recvBytes
			, (LPDWORD)&flags
			, &(ioInfo->overlapped)
			, NULL);
	}
	return 0;

}

void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

unsigned WINAPI EchoThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	DWORD byteTrans;
	SOCKET sock;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;
	DWORD flags = 0;

	while (1) {
		GetQueuedCompletionStatus(hComPort
			, &byteTrans
			, (LPDWORD)&handleInfo
			, (LPOVERLAPPED*)&ioInfo
			, INFINITE);
		sock = handleInfo->hClntSock;

		if (ioInfo->rwMode == READ) {
			if (byteTrans <= 0) {
				closesocket(sock);
				free(ioInfo);
				free(handleInfo);
				continue;
			}
			puts("message received!");

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = byteTrans;
			ioInfo->rwMode = WRITE;
			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			ioInfo = new PER_IO_DATA();
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
			
		}
		else {
			puts("message sent!");
			free(ioInfo);
		}
	}
	return 0;
}
