#include "GameScene.h"

bool GameScene::init()
{
	//바둑판 세팅
	Size winSize = Director::getInstance()->getWinSize();
	pan = OmokPan::create();
	pan->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(pan);

	//test
	pan->SetTurn(true);

	btnPut = ui::Button::create("btnPut1.png", "btnPut2.png");
	btnPut->addClickEventListener(CC_CALLBACK_0(GameScene::OnClickPutBtn, this));
	btnPut->setPosition(Point(winSize.width*0.5f, 50));
	this->addChild(btnPut);

    return true;
}

void GameScene::OnClickPutBtn()
{
	PanIndex index = pan->GetPanIndex();
	if (index.x < 0)//항상 pan index>0이므로, 놓을 자리 선택이 안된 상태를 의미
		return;

	pan->SetTurn(false);
	//send server msg : put stone
	
}
