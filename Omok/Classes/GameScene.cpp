#include "GameScene.h"

bool GameScene::init()
{
	Size winSize = Director::getInstance()->getWinSize();
	pan = OmokPan::create();
	pan->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(pan);

    return true;
}
//
//void GameScene::InitDrawPan()
//{
//	pan_width = 14;
//	//가로 세로 14x14 칸
//	for (int col = 0; col < pan_width; col++) {
//		for (int row = 0; row < pan_width; row++) {
//			Sprite* spr = Sprite::create("block.png");
//			spr->setAnchorPoint(Point(0, 0));
//			float posX = winSize.width*0.5f+ blockSize.width*(col - (pan_width*0.5f));
//			float posY = winSize.height*0.5f + blockSize.height*(row - (pan_width*0.5f));
//			spr->setPosition(Point(posX, posY));
//			this->addChild(spr, 0);
//		}
//	}
//
//	Sprite* back = Sprite::create("backblock.png");
//	back->setScale(15);
//	back->setPosition(Point(winSize.width*0.5f, winSize.height*0.5f));
//	this->addChild(back, -1);
//
//	//바둑판 왼쪽하단 point
//	panMin.setPoint(winSize.width*0.5f - blockSize.width*(pan_width*0.5f + 0.5f)
//		, winSize.height*0.5f - blockSize.height*(pan_width*0.5f + 0.5f));
//	//바둑판 오른쪽상단 point
//	panMax.setPoint(winSize.width*0.5f + blockSize.width*(pan_width*0.5f + 0.5f)
//		, winSize.height*0.5f + blockSize.height*(pan_width*0.5f + 0.5f));
//
//	initPanBlank();
//}
//
//void GameScene::InitMouseListener()
//{
//	auto listener = EventListenerTouchOneByOne::create();
//	listener->onTouchBegan = CC_CALLBACK_2(GameScene::OnTouchBegan, this);
//	listener->onTouchMoved = CC_CALLBACK_2(GameScene::OnTouchMoved, this);
//	listener->onTouchEnded= CC_CALLBACK_2(GameScene::OnTouchEnded, this);
//	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 2);
//}
//
//bool GameScene::OnTouchBegan(Touch * touch, Event * event)
//{
//	return true;
//}
//void GameScene::OnTouchMoved(Touch * touch, Event * event)
//{
//	CCLOG("move");
//	pan->setPosition(touch->getLocation());
//}
//void GameScene::OnTouchEnded(Touch * touch, Event * event)
//{
//	if (turn) {//내 차례일때
//		Point pos = touch->getLocation();
//		//touch 위치가 바둑판 내 일때,
//		if (pos.x > panMin.x&&pos.x<panMax.x&&pos.y>panMin.y&&pos.y < panMax.y) {
//			Point index = PositionToPanIndex(pos);
//			target->setPosition(panMin+(index * blockSize.width));
//			target->setVisible(true);
//		}
//	}
//}
//
//Point GameScene::PositionToPanIndex(Point pos)
//{
//	Point index = Point(0, 0);
//	pos -= panMin;
//	//내림
//	index.x = (int)(pos.x / blockSize.width);
//	index.y = (int)(pos.y / blockSize.height);
//	return index;
//}
//
//void GameScene::initPanBlank()
//{
//	for (int col = 0; col <= pan_width; col++) {
//		for (int row = 0; row <= pan_width; row++) {
//			panBlank[row][col] = true;
//		}
//	}
//}
