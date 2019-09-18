#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 1024
#define OPSZ 4
void ErrorHandling(const char *message);

int calculateDday(int fromDate[], int toDate[]);
int getDays(bool leapYear,int month, int day);
int getDaysFromLast(bool leapYear, int month, int day);
int getDaysBetweenYears(int fromYear, int toYear);
bool checkLeapYear(int year);

void getToday(int* date);
enum Date
{
	YEAR,
	MONTH,
	DAY
};
int main() {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE] = {0};
	SOCKADDR_IN servAddr, clntAddr;
	int clntAddrSize;

	int fromDate[3] = { 0 };
	int toDate[3] = { 0 };
	int dDay = 0;

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

	for (int i = 0; i < 5; i++) {
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr,&clntAddrSize);
		if (hClntSock == INVALID_SOCKET) {
			ErrorHandling("accept() error");
		}

		getToday(fromDate);
		
		recv(hClntSock, (char*)&toDate[YEAR], OPSZ, 0);
		recv(hClntSock, message, 1, 0);
		toDate[MONTH] = (int)message[0];
		recv(hClntSock, message, 1, 0);
		toDate[DAY] = (int)message[0];
		
		dDay = calculateDday(fromDate, toDate);
		printf("°á°ú : %d", dDay);
		send(hClntSock, (char*)&dDay, sizeof(dDay), 0);
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	system("pause");
	return 0;
}
int calculateDday(int fromDate[],int toDate[])
{
	printf("\n========================\n");
	printf("from  %d / %d / %d\n", fromDate[YEAR], fromDate[MONTH], fromDate[DAY]);// 
	printf("to %d / %d / %d\n", toDate[YEAR], toDate[MONTH], toDate[DAY]);
	printf("========================\n");
	int dDay = 0;
	if (fromDate[YEAR] == toDate[YEAR]) {
		int days_fromDate = getDays(checkLeapYear(fromDate[YEAR]), fromDate[MONTH], fromDate[DAY]);
		int days_toDate = getDays(checkLeapYear(toDate[YEAR]), toDate[MONTH], toDate[DAY]);
		
		dDay = days_toDate - days_fromDate;
	}
	else if (fromDate[YEAR] < toDate[YEAR]) {
		int daysFL_fromDate = getDaysFromLast(checkLeapYear(fromDate[YEAR]), fromDate[MONTH], fromDate[DAY]);
		int days_toDate = getDays(checkLeapYear(toDate[YEAR]), toDate[MONTH], toDate[DAY]);
		int days_betweenYears = getDaysBetweenYears(fromDate[YEAR], toDate[YEAR]);
		dDay = days_toDate+ daysFL_fromDate+days_betweenYears;
	}
	else {
		int days_fromDate = getDays(checkLeapYear(fromDate[YEAR]), fromDate[MONTH], fromDate[DAY]);
		int daysFL_toDate = getDaysFromLast(checkLeapYear(toDate[YEAR]), toDate[MONTH], toDate[DAY]);
		int days_betweenYears = getDaysBetweenYears(toDate[YEAR], fromDate[YEAR]);

		dDay = -(days_fromDate + daysFL_toDate + days_betweenYears);
	}
	return dDay;
}
int getDays(bool leapYear, int month, int day)//get days from that year 1/1 to month/day
{
	int days = 0;
	for (int m = 1; m < month; m++) {
		switch (m)
		{
		case 4:
		case 6:
		case 9:
		case 11:
			days += 30;
			break;
		case 2:
			if (leapYear) {
				days += 29;
			}
			else {
				days += 28;
			}
			break;
		default:
			days += 31;
			break;
		}
	}
	days += day;
	return days;
}
int getDaysFromLast(bool leapYear, int month, int day)//get days from that year 12/31 to month/day
{
	int days = getDays(leapYear, month, day);
	if (leapYear) {
		days = 366 - days;
	}
	else {
		days = 365 - days;
	}

	return days;
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
int getDaysBetweenYears(int fromYear, int toYear)
{
	int days = 0;
	for (int i = fromYear + 1; i < toYear; i++) {
		if (checkLeapYear(i)) {
			days += 366;
		}
		else {
			days += 365;
		}
	}
	return days;
}
void getToday(int * date)
{
	time_t timer;
	tm t;
	timer = time(NULL);
	localtime_s(&t, &timer);

	date[YEAR] = t.tm_year + 1900;
	date[MONTH] = t.tm_mon + 1;
	date[DAY] = t.tm_mday;
}
void ErrorHandling(const char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
