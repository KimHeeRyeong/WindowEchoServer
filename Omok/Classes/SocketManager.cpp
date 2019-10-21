#include "SocketManager.h"

static SocketManager* instance = NULL;
SocketManager::SocketManager()
{
}
SocketManager::~SocketManager()
{
}
SocketManager * SocketManager::getInstance()
{
	if (instance == NULL) {
		instance = new SocketManager();
	}
	return instance;
}
bool SocketManager::StartSocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		OutputDebugString(L"WSAStartup() error\n");
		return false;
	}
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		OutputDebugString(L"socket() error\n");
		return false;
	}
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(10000);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		OutputDebugString(L"connect error\n");
		return false;
	}

	FD_ZERO(&reads);
	FD_SET(hSocket, &reads);
	return true;
}

void SocketManager::CloseSocket()
{
	closesocket(hSocket);
	WSACleanup();
}

void SocketManager::SendMsg(char * message, int size)
{
	send(hSocket, message, size, 0);
}

int SocketManager::RecvMsg()
{
	cpyReads = reads;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	if (select(0, &cpyReads, 0, 0, &timeout) == SOCKET_ERROR) {
		OutputDebugString(L"recvMsg() -> fdNum==SOCKET_ERROR\n");
		return -1;
	}

	if (FD_ISSET(hSocket, &cpyReads)) {
		Code code;
		recv(hSocket,(char*)&code, sizeof(Code), 0);
		
		switch (code.code)
		{
		case Message::START:
			memcpy(&start, &code, sizeof(Code));
			recv(hSocket, (char*)&start + sizeof(Code), sizeof(Start) - sizeof(Code), 0);
			return Message::START;
		case Message::RESULT:
			recv(hSocket, (char*)&result + sizeof(Code), sizeof(Result) - sizeof(Code), 0);
			return Message::RESULT;
		case Message::ENDGAME:
			recv(hSocket, (char*)&endGame + sizeof(Code), sizeof(EndGame) - sizeof(Code), 0);
			return Message::ENDGAME;
		default:
			break;
		}
	}
}

Start SocketManager::getStartMsg()
{
	return start;
}
Result SocketManager::getResultMsg()
{
	return result;
}
EndGame SocketManager::getEndGameMsg()
{
	return endGame;
}

