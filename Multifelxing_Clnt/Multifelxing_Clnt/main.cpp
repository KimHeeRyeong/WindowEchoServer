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
	fd_set reads, cpyReads;
	int reinforcement = 0;
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
		printf("0)무기강화 1)무기 팔기 2)무기 정보 3)순위 4) 끝: ");
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
			system("cls");
			switch (message[0])
			{
			case 'm':
				printf("최대 강화 완료! 더이상 강화는 불가능합니다.\n");
				break;
			case 's': {
				printf("성공!\n");
				recv(hSocket, (char*)&reinforcement, 4, 0);
				printf("%d 강화\n", reinforcement);
				if (reinforcement == 99) {
					printf("최대 강화 완료! 더이상 강화는 불가능합니다.\n");
				}
				recv(hSocket, (char*)&message, 1, 0);
				if(message[0]=='o'){
					printf("최고 기록!\n");
				}
				break; 
			}
			case 'f':
				reinforcement = -1;
				printf("실패\n");
				break;
			case 'x':
				printf("부서진 무기는 강화가 불가능합니다.\n");
				break;
			default:
				break;
			}
			break;
		case SELL:
			system("cls");
			printf("준비중입니다.");
			break;
		case INFO: {
			float coin = 0;
			recv(hSocket, (char*)&reinforcement, 4, 0);
			recv(hSocket, (char*)&coin, 4, 0);
			system("cls");
			if (reinforcement == -1) {
				printf("부서진 무기\n");
			}
			else {
				printf("%d강 무기\n", reinforcement);
			}
			printf("코인갯수:%f\n", coin);
			break;
		}
		case SCORE:
		{
			int maxReinf;
			recv(hSocket, (char*)&maxReinf, 4, 0);
			recv(hSocket, (char*)&reinforcement, 4, 0);
			system("cls");
			printf("서버 최고 강화 기록 : %d\n", maxReinf);
			printf("나의 기록 : %d", reinforcement);
			break;
		}
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
	printf("게임을 종료합니다.");
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
