#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

bool HelloWorld::init()
{
	player = Player::create();
	this->addChild(player);
    return true;
}

