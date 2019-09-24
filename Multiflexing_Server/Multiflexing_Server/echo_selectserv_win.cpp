#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <random>
#define BUF_SIZE 1024
void ErrorHandling(const char *message);
struct Status
{
	int reinforcement;
	int coin;
};
enum Code {
	REINFORCE,
	SELL,
	INFO,
	SCORE,
	END,
};
int main() {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE];
	std::vector<Status> clntStatus;

	srand((unsigned int)time(NULL));

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
	FD_ZERO(&reads);//초기화
	FD_SET(hServSock, &reads);

	while (1) {
		cpyReads = reads;
		timeout.tv_sec = 5;//우선
		timeout.tv_usec = 5500;//5.5초
		//select 매개변수 : 검사대상 수, 수신 데이터에 관심있는 디스크립터 변수 주소,
		//블로킹없는 데이터 전송 가능 여부에 관심있는 디스크립터 주소, 무한정 블로킹에 빠지지 않기위한시간
		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;
		if (fdNum == 0)
			continue;

		int readFdCnt = reads.fd_count;
		for (i = 0; i < readFdCnt; i++) {
		
			//읽을 게 있으면 양수 반환
			if (FD_ISSET(reads.fd_array[i], &cpyReads)) {
				if (reads.fd_array[i] == hServSock) {//connect
					adrSz = sizeof(clntAddr);
					hClntSock=
						accept(hServSock, (SOCKADDR*)&clntAddr, &adrSz);
					FD_SET(hClntSock, &reads);
					printf("connected client : %d\n", hClntSock);
					Status statusInit = { 0,1000 };
					clntStatus.push_back(statusInit);
				}
				else {//read msg
					strLen = recv(reads.fd_array[i], buf, 1, 0);
					if (strLen == 0) {
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						clntStatus.erase(clntStatus.begin() + i - 1);
						printf("close client : %d \n", cpyReads.fd_array[i]);
					}
					else {
						int cho = (int)buf[0];
						int reinforcement = clntStatus.at(i - 1).reinforcement;
						int coin = clntStatus.at(i - 1).coin;
						switch (cho) {
						case REINFORCE:
							if (reinforcement != -1) {//무기가 부서지지않은 경우
								if (rand() % 100 >= reinforcement) {
									reinforcement++;
									buf[0] = 's';
									send(reads.fd_array[i],buf, 1, 0);//sucess
								}
								else {
									reinforcement = -1;
									buf[0] = 'f';
									send(reads.fd_array[i], buf, 1, 0);//fail
									
								}
							}
							else {
								buf[0] = 'x';
								send(reads.fd_array[i],buf, 1, 0);
							}
							clntStatus.at(i - 1).reinforcement = reinforcement;
							break;
						case SELL:
							break;
						case INFO: {
							strcpy_s(buf, 4, (char*)&reinforcement);
							strcpy_s(buf+4, 4, (char*)&coin);
							reinforcement = (int)buf;
							coin = (int)(buf + 4);
							printf("%d %d", reinforcement, coin);
							strcpy_s(buf, 4, (char*)&coin);
							send(reads.fd_array[i], buf, 8, 0);
							break; 
						}
						case SCORE:
							break;
						case END://끝내기
							FD_CLR(reads.fd_array[i], &reads);
							closesocket(cpyReads.fd_array[i]);
							printf("close client : %d \n", cpyReads.fd_array[i]);
							break;
						default:
							break;
						}
					}
				}
			}
		}

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