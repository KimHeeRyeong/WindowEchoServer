#include "GameScene.h"

bool GameScene::init()
{
	_winSize = Director::getInstance()->getWinSize();
	_szBlock = Sprite::create("block.png")->getContentSize();
	_margin = Vec2(_szBlock.width*0.5f, _szBlock.height*0.5f);
	_myTurn = false;

	CreatePan();

	_stone = Sprite::create("whiteStone.png");
	_stone->setVisible(false);
	this->addChild(_stone,3);

	auto listener = EventListenerMouse::create();
	listener->onMouseDown = CC_CALLBACK_1(GameScene::OnMouseDown, this);
	listener->onMouseMove = CC_CALLBACK_1(GameScene::OnMouseMove, this);
	listener->onMouseUp = CC_CALLBACK_1(GameScene::OnMouseUp, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	return true;
}

void GameScene::CreatePan()
{
	int cnt = 19;//colum row count
	Color3B panColor = Color3B(221, 188, 131);
	for (int i = 0; i < cnt; i++) {
		for (int j = 0; j < cnt; j++) {
			Sprite* block = Sprite::create("block.png");
			block->setColor(panColor);
			block->setAnchorPoint(Vec2(0, 0));
			Vec2 pos = Vec2(j*_szBlock.width, i*_szBlock.height)+_margin;
			block->setPosition(pos);
			this->addChild(block,1);
		}
	}

	Scale9Sprite* blockEdge = Scale9Sprite::create("blockEdge2.png", Rect(0,0,32,32), Rect(15,15,2,2));
	blockEdge->setAnchorPoint(Vec2(0, 0));
	blockEdge->setColor(panColor);
	blockEdge->setContentSize(_szBlock*(cnt+1));
	blockEdge->setPosition(Vec2(0,0));
	this->addChild(blockEdge,0);
}

void GameScene::OnMouseDown(EventMouse * mouse)
{

}

void GameScene::OnMouseMove(EventMouse * mouse)
{
}

void GameScene::OnMouseUp(EventMouse * mouse)
{
}
