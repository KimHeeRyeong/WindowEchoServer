#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#define BUF_SIZE 100

USING_NS_CC;
using namespace std;

class Player : public Node
{
public:
	virtual bool init();

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual bool onTouchMoved(Touch* touch, Event* event);
	virtual bool onTouchEnded(Touch* touch, Event* event);
	virtual bool onTouchCancelled(Touch* touch, Event* evnet);
	
	~Player();
	CREATE_FUNC(Player);
private:
	Sprite* spr;
	Size winSize;
	Point moveP;
	float speed;
	
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;
	char msg[BUF_SIZE];
	Label* errLabel;

	void moveToPoint(float delta);
	void setSocket();
	void sendMsg();
	void recvMsg();
	void ErrorHandling(string msg);
};
#endif

