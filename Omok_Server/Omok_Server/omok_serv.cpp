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
#define READ 3
#define WRITE 5
typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;

	SOCKET hOppSock=NULL;
	OmokPan* pan = NULL;
	bool isBlack;
	bool turn;
	bool oppReady;
	char nick[20];
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {  //buffer info
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;
}PER_IO_DATA, *LPPER_IO_DATA;

unsigned WINAPI EchoThreadMain(LPVOID pComPort);
void ErrorHandling(const char *message);
void RecvMsg(SOCKET sock, DWORD* flags);

LPPER_HANDLE_DATA handleInfoWait = NULL;

HANDLE hMutex;

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

	hMutex = CreateMutex(NULL, false, NULL);
	//io �Ϸ� Ȯ���� ����
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

		LPPER_IO_DATA ioInfo = new PER_IO_DATA();
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;//��� ����

		//nickName�� �ްų� ��� �ߴܿ� ���
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf),
			1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(ioInfo->overlapped), NULL);
	}
	return 0;
}
unsigned WINAPI EchoThreadMain(LPVOID pComPort)//main �� ����ȭ�� ����
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		//��� 
		GetQueuedCompletionStatus(hComPort //�Ϸ�� io ������ ����ִ� �ڵ� ����
			, &bytesTrans //������ ũ�������� ������ ���� �ּ� ����
			, (LPDWORD)&handleInfo //Create IO Completion Port �Լ� ����° ���ڰ� ������ ���� �ּ� ����
			, (LPOVERLAPPED*)&ioInfo//OVERLAPPED ����ü ����� �ּ� ����
			, INFINITE);//Ÿ�Ӿƿ�
		sock = handleInfo->hClntSock;

		if (ioInfo->rwMode == READ) {
			if (bytesTrans <= 0) {//���� ���� Ȯ��
				WaitForSingleObject(hMutex, INFINITE);
				if (handleInfoWait == handleInfo) {
					handleInfoWait = NULL; //��� ������ ���
					closesocket(sock);
					free(handleInfo);
					free(ioInfo);
				}
				else if(handleInfo->hOppSock!=NULL){//������ ������ ���
					SOCKET oppSock = handleInfo->hOppSock;
					closesocket(sock);
					free(handleInfo->pan);
					free(handleInfo);
					free(ioInfo);

					//��� socket�� send
					Code exitOpp(Message::EXITOPP);

					ioInfo = new PER_IO_DATA();
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->wsaBuf.len = sizeof(Code);
					memcpy(ioInfo->buffer, (char*)&exitOpp, sizeof(exitOpp));
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(oppSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
				}
				else {
					closesocket(sock);
					free(handleInfo);
					free(ioInfo);
				}
				ReleaseMutex(hMutex);
				continue;
			}
			Code code;
			memcpy(&code, ioInfo->wsaBuf.buf, sizeof(code));

			switch (code.code) {
			case Message::PUTSTONE: {
				PutStone putStone;
				memcpy(&putStone, ioInfo->wsaBuf.buf, sizeof(putStone));
				free(ioInfo);
				RecvMsg(sock, &flags);//��� �Է� ����(���ῡ ����Ͽ�)
				if (handleInfo->turn) {//�� ���� ������
					handleInfo->turn = false;
					if (handleInfo->pan->CheckBlank(putStone.posX, putStone.posY)) {//���� ���� �� �ִ� �ڸ��ΰ�?
						if (!handleInfo->pan->AddStone(handleInfo->isBlack, putStone.posX, putStone.posY)) {//�����ΰ�?
							//recv�� socket���� send
							Result result;
							result.isBlack = handleInfo->isBlack;
							result.posX = putStone.posX;
							result.posY = putStone.posY;
							result.turn = false;

							ioInfo = new PER_IO_DATA();
							memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
							ioInfo->wsaBuf.len = sizeof(Result);
							memcpy(ioInfo->buffer, (char*)&result, sizeof(result));
							ioInfo->wsaBuf.buf = ioInfo->buffer;
							ioInfo->rwMode = WRITE;
							WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

							//��� socket send
							result.turn = true;

							ioInfo = new PER_IO_DATA();
							memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
							ioInfo->wsaBuf.len = sizeof(Result);
							memcpy(ioInfo->buffer, (char*)&result, sizeof(result));
							ioInfo->wsaBuf.buf = ioInfo->buffer;
							ioInfo->rwMode = WRITE;
							WSASend(handleInfo->hOppSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

						}
						else {
							//������ ��� end game
							//recv�� socket���� send
							EndGame endGame;
							endGame.isBlack = handleInfo->isBlack;
							endGame.posX = putStone.posX;
							endGame.posY = putStone.posY;
							endGame.isWin = true;

							ioInfo = new PER_IO_DATA();
							memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
							ioInfo->wsaBuf.len = sizeof(EndGame);
							memcpy(ioInfo->buffer, (char*)&endGame, sizeof(endGame));
							ioInfo->wsaBuf.buf = ioInfo->buffer;
							ioInfo->rwMode = WRITE;
							WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

							//��� socket send
							endGame.isWin = false;

							ioInfo = new PER_IO_DATA();
							memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
							ioInfo->wsaBuf.len = sizeof(EndGame);
							memcpy(ioInfo->buffer, (char*)&endGame, sizeof(endGame));
							ioInfo->wsaBuf.buf = ioInfo->buffer;
							ioInfo->rwMode = WRITE;
							WSASend(handleInfo->hOppSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
						}
					}
					else {
						//���� �� ���� �ڸ��� ��� ���
					}
				}
				else {
					//�� ���� �ƴѵ� �޼����� �Դٸ� ���
				}
				break;
			}
			case Message::NICKNAME: {
				NickName nickName;
				memcpy(&nickName, ioInfo->wsaBuf.buf, sizeof(nickName));
				strcpy_s(handleInfo->nick, nickName.nick);
				free(ioInfo);
				RecvMsg(sock, &flags);

				WaitForSingleObject(hMutex, INFINITE);
				//��� ��밡 ���� ���
				if (handleInfoWait == NULL) {
					handleInfoWait = handleInfo;
				}
				else {//��� ��� �ִ� ���
					//�� ����
					OmokPan* pan = new OmokPan();
					handleInfoWait->pan = handleInfo->pan = pan;
					//���� socket setting
					handleInfo->hOppSock = handleInfoWait->hClntSock;
					handleInfoWait->hOppSock = sock;

					//recv socket�� send Start
					Start start;
					start.turn = start.isBlack = false;//�鵹
					strcpy_s(start.nickOpponent, handleInfoWait->nick);

					ioInfo = new PER_IO_DATA();
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->wsaBuf.len = sizeof(Start);
					memcpy(ioInfo->buffer, (char*)&start, sizeof(start));
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

					//��� socket(handle wait)�� send Start
					start.turn = start.isBlack = true;//�浹
					strcpy_s(start.nickOpponent, handleInfo->nick);

					ioInfo = new PER_IO_DATA();
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->wsaBuf.len = sizeof(Start);
					memcpy(ioInfo->buffer, (char*)&start, sizeof(start));
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(handleInfoWait->hClntSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

					handleInfoWait = NULL;
				}
				ReleaseMutex(hMutex);
				break;
			}
			case Message::REPLAY: {
				free(ioInfo);
				RecvMsg(sock, &flags);
				WaitForSingleObject(hMutex, INFINITE);
				if (handleInfo->oppReady) {//�� user ��� ready => start
					handleInfo->pan->initPan();//pan data reset

					//recv socket�� send Start
					Start start;
					start.turn = start.isBlack = !handleInfo->isBlack;

					ioInfo = new PER_IO_DATA();
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->wsaBuf.len = sizeof(Start);
					memcpy(ioInfo->buffer, (char*)&start, sizeof(start));
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

					//��� socket�� send Start
					start.turn = start.isBlack = handleInfo->isBlack;

					ioInfo = new PER_IO_DATA();
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->wsaBuf.len = sizeof(Start);
					memcpy(ioInfo->buffer, (char*)&start, sizeof(start));
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(handleInfo->hOppSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
				}
				else {//���� ready => ��뿡�� ready ���� ����
					//��� socket���� send
					Code replay(Message::REPLAY);

					ioInfo = new PER_IO_DATA();
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					ioInfo->wsaBuf.len = sizeof(Code);
					memcpy(ioInfo->buffer, (char*)&replay, sizeof(replay));
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(handleInfo->hOppSock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
				}
				ReleaseMutex(hMutex);
				break; 
			}
			default:
				free(ioInfo);
				RecvMsg(sock, &flags);
				break;
			}
		}
		else {
			Code code;
			memcpy(&code, ioInfo->wsaBuf.buf, sizeof(Code));
			switch (code.code) {
			case Message::START: {
				Start start;
				memcpy(&start, ioInfo->wsaBuf.buf, sizeof(start));
				free(ioInfo);
				handleInfo->oppReady = false;
				handleInfo->turn = start.turn;
				handleInfo->isBlack = start.isBlack;
				break;
			}
			case Message::RESULT: {
				Result result;
				memcpy(&result, ioInfo->wsaBuf.buf, sizeof(result));
				free(ioInfo);
				handleInfo->turn = result.turn;
				break;
			}
			case Message::ENDGAME: {
				EndGame endGame;
				memcpy(&endGame, ioInfo->wsaBuf.buf, sizeof(endGame));
				handleInfo->turn = false;
				free(ioInfo);
				break; 
			}
			case Message::EXITOPP: {
				handleInfo->hOppSock = NULL;
				break;
			}
			case Message::REPLAY:{
				free(ioInfo);
				WaitForSingleObject(hMutex, INFINITE);
				handleInfo->oppReady = true;
				ReleaseMutex(hMutex);
				break;
			}
			default:
				free(ioInfo);
				break;			   
			}
		}

	}
	return 0;
}
void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void RecvMsg(SOCKET sock, DWORD* flags) {
	LPPER_IO_DATA ioInfo = new PER_IO_DATA();
	memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
	ioInfo->wsaBuf.len = BUF_SIZE;
	ioInfo->wsaBuf.buf = ioInfo->buffer;
	ioInfo->rwMode = READ;
	WSARecv(sock, &(ioInfo->wsaBuf),
		1, NULL, (LPDWORD)flags, &(ioInfo->overlapped), NULL);
}

