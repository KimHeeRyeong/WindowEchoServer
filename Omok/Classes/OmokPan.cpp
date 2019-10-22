#include "OmokPan.h"

bool OmokPan::init()
{
	winSize = Director::getInstance()->getWinSize();
	blockSize = Sprite::create("block.png")->getContentSize();
	turn = false;
	stones.clear();

	target = Sprite::create("target.png");
	target->setAnchorPoint(Point(0, 0));
	target->setVisible(false);
	this->addChild(target,1);

	InitDrawPan();
	InitTouchListener();

    return true;
}

void OmokPan::InitDrawPan()
{
	pan_width = 14;
	//가로 세로 14x14 칸
	for (int col = 0; col < pan_width; col++) {
		for (int row = 0; row < pan_width; row++) {
			Sprite* spr = Sprite::create("block.png");
			spr->setAnchorPoint(Point(0, 0));
			float posX = blockSize.width*(col - (pan_width*0.5f));
			float posY = blockSize.height*(row - (pan_width*0.5f));
			spr->setPosition(posX, posY);
			this->addChild(spr, 0);
		}
	}

	Sprite* back = Sprite::create("backblock.png");
	back->setScale(15);
	back->setPosition(0,0);
	this->addChild(back, -1);

	//바둑판 왼쪽하단 point
	panMin.setPoint(-blockSize.width*(pan_width*0.5f + 0.5f)
		, -blockSize.height*(pan_width*0.5f + 0.5f));
	//바둑판 오른쪽상단 point
	panMax.setPoint(blockSize.width*(pan_width*0.5f + 0.5f)
		, blockSize.height*(pan_width*0.5f + 0.5f));

	initPanBlank();
}

void OmokPan::InitTouchListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(OmokPan::OnTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OmokPan::OnTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OmokPan::OnTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

bool OmokPan::OnTouchBegan(Touch * touch, Event * event)
{
	movePan = false;
	return true;
}

void OmokPan::OnTouchMoved(Touch * touch, Event * event)//바둑판 이동
{
	movePan = true;
	Point pos = touch->getLocation() - this->getPosition();
	//touch 위치가 바둑판 내 일때,
	if (pos.x > panMin.x&&pos.x<panMax.x&&pos.y>panMin.y&&pos.y < panMax.y) {
		float moveX = this->getPosition().x+touch->getDelta().x;

		//바둑판 이동 제약
		if (moveX + panMin.x > 0)
			moveX = -panMin.x;
		else if (moveX + panMax.x < winSize.width)
			moveX = winSize.width - panMax.x;

		this->setPosition(moveX, this->getPosition().y);
	}

}

void OmokPan::OnTouchEnded(Touch * touch, Event * event)
{
	if (turn&&!movePan) {//내 차례, 바둑판 이동을 위한 touch가 아닐때
		Point pos = touch->getLocation()-this->getPosition();
		//touch 위치가 바둑판 내 일때,
		if (pos.x > panMin.x&&pos.x<panMax.x&&pos.y>panMin.y&&pos.y < panMax.y) {
			targetIndex = PositionToPanIndex(pos);
			if (panBlank[targetIndex.x][targetIndex.y]) {//해당 위치에 놓여진 돌이 없을때
				target->setPosition(panMin + Point(targetIndex.x * blockSize.width, targetIndex.y * blockSize.height));
				target->setVisible(true);
			}
		}
	}
}

void OmokPan::SetTurn(bool setTurn)
{
	if (!setTurn)
		target->setVisible(false);
	turn = setTurn;
}

bool OmokPan::GetTurn()
{
	return turn;
}

PanIndex OmokPan::GetPanIndex()
{
	if (!target->isVisible())
		return PanIndex();
	return targetIndex;
}

void OmokPan::AddStone(bool isBlack, int posX, int posY)
{
	Sprite* stone;
	if (isBlack) {
		stone = Sprite::create("blackStone.png");
	}
	else {
		stone = Sprite::create("whiteStone.png");
	}
	panBlank[posX][posY] = false;
	stone->setPosition(panMin + Point((posX+0.5f) * blockSize.width, (posY+0.5f) * blockSize.height));
	stone->setScale(1.2f);
	this->addChild(stone, 2);
}

PanIndex OmokPan::PositionToPanIndex(Point pos)
{
	PanIndex index;
	pos -= panMin;
	//내림
	index.x = (int)(pos.x / blockSize.width);
	index.y = (int)(pos.y / blockSize.height);
	return index;
}

void OmokPan::initPanBlank()
{
	//clear stones
	for (auto spr : stones) {
		CCLOG("%d",stones.size());
		this->removeChild(spr, true);
	}
	//init pan array
	for (int col = 0; col <= pan_width; col++) {
		for (int row = 0; row <= pan_width; row++) {
			panBlank[row][col] = true;
		}
	}
}
