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

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;

	bool isBlack;
	SOCKET hOppSock;
	OmokPan* pan;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {  //buffer info
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int mode;
	bool turn;
	
}PER_IO_DATA, *LPPER_IO_DATA;

unsigned WINAPI EchoThreadMain(LPVOID pComPort);
void ErrorHandling(const char *message);

LPPER_HANDLE_DATA handleInfoWait = NULL;
LPPER_IO_DATA ioInfoWait = NULL;

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
		if (handleInfoWait == NULL) {
			handleInfoWait = handleInfo;
			LPPER_IO_DATA ioInfo = new PER_IO_DATA();
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->mode = Message::NONE;//대기 상태
			ioInfoWait = ioInfo;
			//대기 중단을 대비하기 위함
			WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf),
				1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
		}
		else {
			//대결 상대가 있는 경우 send start
			free(ioInfoWait);//대기 중단을 대비한 WSARecv stop
			ioInfoWait = NULL;

			OmokPan* pan = new OmokPan();
			
			//hWaitSocket wsaSend
			handleInfoWait->hOppSock = hClntSock;
			handleInfoWait->isBlack = true;
			handleInfoWait->pan = pan;

			Start start;
			start.isBlack = true;
			strcpy_s(start.nickOpponent, "black");
			start.turn = true;
			
			LPPER_IO_DATA ioInfo = new PER_IO_DATA();
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = sizeof(start);
			ioInfo->wsaBuf.buf = (char*)&start;
			ioInfo->mode = Message::START;
			ioInfo->turn = true;
			WSASend(handleInfoWait->hClntSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			//hClntSocket wsaSend
			handleInfo->hOppSock = handleInfoWait->hClntSock;
			handleInfo->isBlack = false;
			handleInfo->pan = pan;

			Start startClnt;
			startClnt.isBlack = false;
			strcpy_s(startClnt.nickOpponent, "white");
			startClnt.turn = false;

			LPPER_IO_DATA ioInfoClnt = new PER_IO_DATA();
			memset(&(ioInfoClnt->overlapped), 0, sizeof(OVERLAPPED));
			ioInfoClnt->wsaBuf.len = sizeof(startClnt);
			ioInfoClnt->wsaBuf.buf = (char*)&startClnt;
			ioInfoClnt->mode = Message::START;
			ioInfoClnt->turn = false;
			WSASend(hClntSock, &(ioInfoClnt->wsaBuf), 1, NULL, 0, &(ioInfoClnt->overlapped), NULL);

			handleInfoWait = NULL;
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
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				handleInfoWait = NULL;
				ioInfo = NULL;
				continue;
			}
			break;
		case Message::START:
			if (ioInfo->turn) {
				free(ioInfo);
				ioInfo = new PER_IO_DATA();
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				ioInfo->wsaBuf.len = BUF_SIZE;
				ioInfo->wsaBuf.buf = ioInfo->buffer;
				ioInfo->mode = Message::PUTSTONE;
				ioInfo->turn = true;
				WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
			}
			else {
				free(ioInfo);
			}
			break;
		case Message::PUTSTONE:
			if (ioInfo->turn) {
				PutStone putSton;
				memcpy(&putSton,ioInfo->wsaBuf.buf, sizeof(putSton));
				free(ioInfo);
				if (handleInfo->pan->CheckBlank(putSton.posX, putSton.posY)) {
					if (!handleInfo->pan->AddStone(handleInfo->isBlack, putSton.posX, putSton.posY)) {
						Result result;
						result.isBlack = handleInfo->isBlack;
						result.posX = putSton.posX;
						result.posY = putSton.posY;
						result.turn = false;

						LPPER_IO_DATA ioInfo = new PER_IO_DATA();
						memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
						ioInfo->wsaBuf.len = sizeof(result);
						ioInfo->wsaBuf.buf = (char*)&result;
						ioInfo->mode = Message::RESULT;
						ioInfo->turn = false;
						WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

						Result resultClnt;
						resultClnt.isBlack = handleInfo->isBlack;
						resultClnt.posX = putSton.posX;
						resultClnt.posY = putSton.posY;
						resultClnt.turn = true;

						LPPER_IO_DATA ioInfoClnt = new PER_IO_DATA();
						memset(&(ioInfoClnt->overlapped), 0, sizeof(OVERLAPPED));
						ioInfoClnt->wsaBuf.len = sizeof(result);
						ioInfoClnt->wsaBuf.buf = (char*)&result;
						ioInfoClnt->mode = Message::RESULT;
						ioInfoClnt->turn = true;
						WSASend(handleInfo->hOppSock, &(ioInfoClnt->wsaBuf), 1, NULL, 0, &(ioInfoClnt->overlapped), NULL);
					}
					
				}
				
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

