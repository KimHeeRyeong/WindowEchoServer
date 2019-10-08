#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


bool HelloWorld::init()
{

	sprs.clear();
	speed = 3.0f;
	//set socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(10000);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
		ErrorHandling("connect error");
	}

	recvFirst();

	FD_ZERO(&reads);
	FD_SET(hSocket, &reads);

	this->schedule(schedule_selector(HelloWorld::recvMsg));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}

void HelloWorld::recvMsg(float delta)
{
	cpyReads = reads;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	int fdNum = 0;
	if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR) {
		this->unschedule(schedule_selector(HelloWorld::recvMsg));
		CCLOG("recvMsg() -> fdNum==SOCKET_ERROR");
		exit(1);
	}
	if (fdNum == 0) {
		return;
	}

	int clntIndex = 0;
	Point pos = Point(0, 0);
	bool del = false;
	int cnt = reads.fd_count;
	if (FD_ISSET(hSocket, &cpyReads)) {
		recv(hSocket, (char*)&clntIndex, 4, 0);
		recv(hSocket, (char*)&pos.x, 4, 0);
		recv(hSocket, (char*)&pos.y, 4, 0);
		recv(hSocket, (char*)&del, 1, 0);
		if (del == true) {
			delPlayer(clntIndex);
		}
		else {
			setPlayerPos(clntIndex, pos);
		}

	}
}

void HelloWorld::recvFirst()
{
	int cnt = 0;
	recv(hSocket, (char*)&cnt, 4, 0);
	for (int i = 0; i < cnt; i++) {
		Sprite* spr = Sprite::create("player.png");
		Point pos = Point(0, 0);
		recv(hSocket, (char*)&pos.x, 4, 0);
		recv(hSocket, (char*)&pos.y, 4, 0);
		spr->setScale(0.5f);
		spr->setPosition(pos);
		this->addChild(spr);
		sprs.push_back(spr);
		if (i == cnt - 1) {
			mySpr = spr;
		}
	}
}

void HelloWorld::delPlayer(int index)
{
	vector<Sprite*>::iterator iter = sprs.begin();
	advance(iter, index);
	Sprite* spr = *iter;
	if (spr != mySpr) {
		sprs.erase(iter);
		this->removeChild(spr);
	}
}

void HelloWorld::setPlayerPos(int clntIndex, Point pos)
{
	int sprSize = sprs.size();
	if (clntIndex >= sprSize) {
		for (int i = sprSize; i <= clntIndex; i++) {
			Sprite* spr = Sprite::create("player.png");
			Point position = Point(0, 0);
			if (i == clntIndex) {
				position = pos;
			}
			spr->setScale(0.5f);
			spr->setPosition(position);
			sprs.push_back(spr);
			this->addChild(spr);
		}
	}
	else {
		vector<Sprite*>::iterator iter = sprs.begin();
		advance(iter, clntIndex);
		Sprite* spr = *iter;
		spr->setPosition(pos);
	}

}

void HelloWorld::touchPoint(float delta)
{
	Point mySprP = mySpr->getPosition();
	float dis = mySprP.getDistance(touchP);
	if (dis > speed) {
		Vec2 dir = touchP - mySprP;
		dir = dir.getNormalized()*speed;
		sendMsg(mySprP + dir);
	}
}

HelloWorld::~HelloWorld()
{
	closesocket(hSocket);
	WSACleanup();
}

bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	touchP = touch->getLocation();
	this->schedule(schedule_selector(HelloWorld::touchPoint));
	return true;
}

bool HelloWorld::onTouchMoved(Touch * touch, Event * event)
{
	touchP = touch->getLocation();
	return true;
}

bool HelloWorld::onTouchEnded(Touch * touch, Event * event)
{
	this->unschedule(schedule_selector(HelloWorld::touchPoint));
	return true;
}

void HelloWorld::ErrorHandling(const char * msg)
{
	CCLOG(msg);
	exit(1);
}

void HelloWorld::sendMsg(Point pos)
{
	memcpy(msg, &pos.x, 4);
	memcpy(&msg[4], &pos.y, 4);
	send(hSocket,msg, 8, 0);
}
