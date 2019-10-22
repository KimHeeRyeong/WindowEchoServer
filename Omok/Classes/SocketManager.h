#ifndef __SOCK_MANAGER_H__
#define __SOCK_MANAGER_H__
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "MessageFormat.h"
#include <string>
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
	void SendPutStone(int posX, int posY);
	void SendNick();
	void SendReplay();
	int RecvMsg();

	Start getStartMsg();
	Result getResultMsg();
	EndGame getEndGameMsg();

	void SetNickName(string str);
	string GetNickName();
private:
	SocketManager();

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	char msg[BUF_SIZE];

	//msg format : recieve
	Start start;
	Result result;
	EndGame endGame;
	ExitOpp exitOpp;

	//send
	PutStone putStone;
	NickName nick;
	RePlay replay;

};

#endif // __SOCK_MANAGER_H__
