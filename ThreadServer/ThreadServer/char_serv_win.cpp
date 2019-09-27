#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <process.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

unsigned WINAPI HandleClnt(void* arg);
void SendMsg(char* msg, int len, SOCKET* clntSock);
void ErrorHandling(const char *message);

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main() {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int clntAdrSz;
	HANDLE hThread;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}
	hMutex = CreateMutex(NULL, false, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	
	if (hServSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(10000);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}                                   
	while (1) {
		clntAdrSz = sizeof(clntAddr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSz);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
		printf("Connected client IP:%s \n", inet_ntoa(clntAddr.sin_addr));
	}
	closesocket(hServSock);
	WSACleanup();
	system("pause");
	return 0;
}
unsigned WINAPI HandleClnt(void * arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0,i;
	char msg[BUF_SIZE] = { 0 };

	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) > 0) {
		SendMsg(msg, strLen, &hClntSock);
	}

	WaitForSingleObject(hMutex, INFINITE);

	for (i = 0; i < clntCnt; i++) {
		if (hClntSock == clntSocks[i]) {
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	printf("closeSocket:%d\n", hClntSock);
	closesocket(hClntSock);
	return 0;
}
void SendMsg(char * msg, int len, SOCKET* clntSock)
{
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++){
		send(clntSocks[i], msg, len, 0);
	}
		
	ReleaseMutex(hMutex);
}
void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}