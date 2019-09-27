#include "Player.h"

bool Player::init()
{

	winSize = Director::getInstance()->getWinSize();
	errLabel = Label::createWithTTF("error label", "fonts/arial.ttf", 20);
	errLabel->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(errLabel);
	setSocket();

	speed = 3.0f;
	spr = Sprite::create("player.png");
	spr->setScale(0.5f);
	spr->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
	this->addChild(spr);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Player::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Player::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Player::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(Player::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	moveP = spr->getPosition();

	return true;
}

bool Player::onTouchBegan(Touch * touch, Event * event)
{
	moveP = touch->getLocation();
	this->schedule(schedule_selector(Player::moveToPoint));
	return true;
}

bool Player::onTouchMoved(Touch * touch, Event * event)
{
	moveP = touch->getLocation();
	return true;
}

bool Player::onTouchEnded(Touch * touch, Event * event)
{
	this->unscheduleAllSelectors();
	return true;
}

bool Player::onTouchCancelled(Touch * touch, Event * evnet)
{
	return true;
}

Player::~Player()
{
	closesocket(hSocket);
	WSACleanup();
}

void Player::moveToPoint(float delta)
{
	Point sprP = spr->getPosition();
	float dis = sprP.getDistance(moveP);
	if (dis>speed) {
		Vec2 dir = moveP - sprP;
		dir = dir.getNormalized()*speed;
		spr->setPosition(sprP + dir);
	}
}

void Player::setSocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)!=0) {
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
}

void Player::sendMsg()
{
}

void Player::recvMsg()
{
}

void Player::ErrorHandling(string msg)
{
	errLabel->setString(msg);
	exit(0);
}
