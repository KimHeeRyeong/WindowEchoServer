
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <WinSock2.h>
#include <string>
#include <vector>
#define BUF_SIZE 1024
#define MAX_CLNT 100

USING_NS_CC;
using namespace std;
class HelloWorld : public Scene
{
public:
    virtual bool init();
	~HelloWorld();

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual bool onTouchMoved(Touch* touch, Event* event);
	virtual bool onTouchEnded(Touch* touch, Event* event);

    CREATE_FUNC(HelloWorld);
private:
	//socket
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;
	char msg[BUF_SIZE];

	void ErrorHandling(const char* msg);
	void sendMsg(Point pos);
	void recvMsg(float delta);
	void recvFirst();

	//game
	vector<Sprite*> sprs;
	Sprite* mySpr;
	Point touchP;

	void delPlayer(int index);
	void setPlayerPos(int clntIndex, Point pos);
	void touchPoint(float delta);
	float speed;


};
#endif // __HELLOWORLD_SCENE_H__
