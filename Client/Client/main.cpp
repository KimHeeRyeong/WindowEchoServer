#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define OPSZ 4
void ErrorHandling(const char* message);

bool checkLeapYear(int year);
int getMaxDay(int year, int month);

int main() {
	WSAData wsaData;
	SOCKET hSocket;
	char opmsg[BUF_SIZE] = {0};
	int result, opndCnt, i;
	SOCKADDR_IN servAddr;

	int year = 0, month = 0, day = 0;
	int dDay = 0;

	char message[BUF_SIZE];
	int strLen;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
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

	//input year
	while (year<1000||year>9999) {
		printf("연도 입력(4자리) : ");
		scanf_s("%d", (int*)&opmsg[0], OPSZ);
		year= *((int*)opmsg);
	}

	//input month
	while (month < 1 || month>12) {
		printf("월 입력(1~12) : ");
		scanf_s("%d", &month, sizeof(int));
	}
	opmsg[OPSZ] = (char)month;

	//input day
	int maxDay = getMaxDay(year, month);
	while (day < 1 || day > maxDay) {
		printf("날짜 입력(1~%d) : ", maxDay);
		scanf_s("%d",&day, sizeof(int));
	}
	opmsg[OPSZ + 1] = (char)day;

	//send date and receive dDay
	send(hSocket,opmsg,OPSZ+2, 0);
	recv(hSocket, (char*)&dDay, sizeof(int), 0);

	printf("오늘로부터 dDay %d\n", dDay);

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
bool checkLeapYear(int year)
{
	if (year % 400 == 0) {
		return true;
	}
	else if (year % 4 == 0 && year % 100 != 0) {
		return true;
	}

	return false;
}

int getMaxDay(int year, int month)
{
	int maxDay = 30;
	bool leapYear = checkLeapYear(year);

	switch (month)
	{
	case 4:
	case 6:
	case 9:
	case 11:
		maxDay = 31;
		break;
	case 2:
		if (leapYear) {
			maxDay = 29;
		}else {
			maxDay = 28;
		}
		break;
	default:
		break;
	}

	return maxDay;
}
