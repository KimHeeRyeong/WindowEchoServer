#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <process.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);
void ErrorHandling(const char *message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main() {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	HANDLE hSndThread, hRcvThread;

	system("tiTle 채팅창");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}
	fgets(name, BUF_SIZE, stdin);
	printf("name 설정 : %s", name);
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(10000);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error");
	}

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSocket, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSocket, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

unsigned WINAPI SendMsg(void * arg)
{
	SOCKET hSocket = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	while (1) {
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
			closesocket(hSocket);
			exit(0);
		}
		sprintf_s(nameMsg, "%s %s", name, msg);
		send(hSocket, nameMsg, strlen(nameMsg), 0);
	}
	return 0;
}

unsigned WINAPI RecvMsg(void * arg)
{
	int hSocket = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	int strLen;
	while (1) {
		strLen = recv(hSocket, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		if (strLen == -1)
			return -1;
		nameMsg[strLen] = 0;
		printf("\n");
		fputs(nameMsg, stdout);
	}
	return 0;
}

void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
