#include "GameScene.h"

bool GameScene::init()
{
	//�ٵ��� ����
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
	if (index.x < 0)//�׻� pan index>0�̹Ƿ�, ���� �ڸ� ������ �ȵ� ���¸� �ǹ�
		return;

	pan->SetTurn(false);
	//send server msg : put stone
	
}
