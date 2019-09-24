#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(const char* message);

enum Code {
	REINFORCE,
	SELL,
	INFO,
	SCORE,
	END,
};
int main() {
	WSAData wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAddr;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}

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
	else {
		puts("Connected...............");
	};

	while (1) {
		bool endGame = false;
		printf("0)���Ⱝȭ 1)���� �ȱ� 2)���� ���� 3)���� 4) ��: ");
		scanf_s("%d", (int*)&message);
		int cho = (int)message[0];
		if (cho >= 0 && cho <= 4) {
			send(hSocket, message, 1, 0);
		}
		else {
			continue;
		}
		switch (cho)
		{
		case REINFORCE:
			memset(message, 0, BUF_SIZE);
			strLen = recv(hSocket, message, 1, 0);
			switch (message[0])
			{
			case 's':
				printf("����!\n");
				break;
			case 'f':
				printf("����\n");
				break;
			case 'x':
				printf("�μ��� ����� ��ȭ�� �Ұ����մϴ�.\n");
				break;
			default:
				break;
			}
			break;
		case SELL:
			memset(message, 0, BUF_SIZE);
			strLen = recv(hSocket, message, 1, 0);
			break;
		case INFO: {
			int reinforce = 0;
			int coin = 0;
			recv(hSocket, (char*)&reinforce, 4, 0);
			recv(hSocket, (char*)&coin, 4, 0);
			if (reinforce == -1) {
				printf("�μ��� ����\n");
			}
			else {
				printf("%d�� ����\n", reinforce);
			}
			printf("���ΰ���:%d\n", coin);
			break;
		}
		case SCORE:
			break;
		case END:
			endGame = true;
			break;
		default:
			break;
		}
		if (endGame) {
			break;
		}
	}
	printf("������ �����մϴ�.");
	closesocket(hSocket);
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
