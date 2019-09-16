#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>

#define BUF_SIZE 1024
void ErrorHandling(const char *message);
int main() {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strLen, i;

	SOCKADDR_IN servAddr, clntAddr;
	int clntAddrSize;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}
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

	clntAddrSize = sizeof(clntAddr);

	for (i = 0; i < 5; i++) {
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr,&clntAddrSize);
		if (hClntSock == -1) {
			ErrorHandling("accept() error");
		}
		else {
			printf("Connected client %d \n", i + 1);
		}

		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0) {
			send(hClntSock, message, strLen, 0);
		}
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	system("pause");
	return 0;
}
void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
