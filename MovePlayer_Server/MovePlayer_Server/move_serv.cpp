#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <Windows.h>
#include <string.h>

#define BUF_SIZE 1024
#define MAX_CLNT 100

unsigned WINAPI HandlingClnt(void * arg);
void SendFirst(SOCKET* sock);//send all player pos
void SendMsg(char * msg, int index, bool del);
void ErrorHandling(const char* msg);

int clntCnt = 0;
SOCKET clntSock[MAX_CLNT];
HANDLE hMutex;

struct Point {
	float x=0;
	float y=0;
};
Point pos[MAX_CLNT];
int main() {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	int szClntAdr;
	HANDLE hThread;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WsaStartup() error");
	}

	hMutex = CreateMutex(NULL, false, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl (INADDR_ANY);
	servAdr.sin_port = htons(10000);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}
	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	while (1) {
		szClntAdr = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &szClntAdr);
		
		WaitForSingleObject(hMutex, INFINITE);
		clntSock[clntCnt++] = hClntSock;
		SendFirst(&hClntSock);
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandlingClnt, (void*)&hClntSock, 0, NULL);
		printf("Connect Client : %s\n", inet_ntoa(clntAdr.sin_addr));
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

unsigned WINAPI HandlingClnt(void * arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	char msg[BUF_SIZE] = { 0 };
	int strLen = 0,i;

	while ((strLen = recv(hClntSock, msg, 8, 0)) > 0) {
		int index = 0;
		WaitForSingleObject(hMutex, INFINITE);
		for (i = 0; i < clntCnt; i++) {
			if (hClntSock == clntSock[i]) {
				index = i;
				break;
			}
		}
		ReleaseMutex(hMutex);
		memcpy(&pos[index].x, msg, 4);
		memcpy(&pos[index].y, &msg[4], 4);
		SendMsg(msg,index, false);
	}

	WaitForSingleObject(hMutex, INFINITE);

	for(i = 0;i<clntCnt;i++){
		if (hClntSock == clntSock[i]) {
			SendMsg(msg, i, true);
			pos[i].x = 0;
			pos[i].y = 0;
			while (i<clntCnt-1) {
				pos[i].x = pos[i + 1].x;
				pos[i].y = pos[i + 1].y;
				clntSock[i] = clntSock[i + 1];
				i++;
			}
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);

	printf("Close Client:%d\n", hClntSock);
	closesocket(hClntSock);
	return 0;
}

void SendFirst(SOCKET* sock)
{
	SOCKET hClntSock = *sock;
	char msg[BUF_SIZE] = { 0 };
	memcpy(msg, &clntCnt, 4);
	for (int i = 0, j=4; i < clntCnt; i++, j+=8) {
		//printf("%d : %f %f", i, pos[i].x, pos[i].y);
		memcpy(&msg[j], &pos[i].x,4);
		memcpy(&msg[4 + j], &pos[i].y,4);
	}
	int strLen = 4 + (clntCnt * 8);
	send(hClntSock, msg, strLen, 0);
}
void SendMsg(char * msg, int index,bool del)
{
	int i;
	char sendMsg[13] = {0};

	memcpy(sendMsg, &index, 4);
	memcpy(&sendMsg[4], msg, 8);
	memcpy(&sendMsg[12], &del, 1);
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++) {
		send(clntSock[i], sendMsg,13, 0);
	}
	ReleaseMutex(hMutex);
}

void ErrorHandling(const char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	//exit(1);
}
