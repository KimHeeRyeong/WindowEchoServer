#ifndef __SOCK_MANAGER_H__
#define __SOCK_MANAGER_H__
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "MessageFormat.h"
#define BUF_SIZE 100

using namespace std;
class SocketManager
{
public:
	static SocketManager* getInstance();
	~SocketManager();

	bool StartSocket();
	void CloseSocket();
	void SendMsg(char* message, int size);
	int RecvMsg();

	Start getStartMsg();
	Result getResultMsg();
	EndGame getEndGameMsg();
private:
	SocketManager();

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	char msg[BUF_SIZE];

	//msg format
	Start start;
	Result result;
	EndGame endGame;
};

#endif // __SOCK_MANAGER_H__
