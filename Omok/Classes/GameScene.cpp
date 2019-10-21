#include "GameScene.h"
#include "SocketManager.h"

bool GameScene::init()
{
	//test
	SocketManager::getInstance()->StartSocket();

	//�ٵ��� ����
	Size winSize = Director::getInstance()->getWinSize();
	pan = OmokPan::create();
	pan->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(pan);

	btnPut = ui::Button::create("btnPut1.png", "btnPut2.png");
	btnPut->addClickEventListener(CC_CALLBACK_0(GameScene::OnClickPutBtn, this));
	btnPut->setPosition(Point(winSize.width*0.5f, 50));
	this->addChild(btnPut);
	
	this->scheduleUpdate();
    return true;
}

void GameScene::OnClickPutBtn()
{
	if (pan->GetTurn()) {
		PanIndex index = pan->GetPanIndex();
		if (index.x < 0)//�׻� pan index>0�̹Ƿ�, ���� �ڸ� ������ �ȵ� ���¸� �ǹ�
			return;

		pan->SetTurn(false);
		put.posX = index.x;
		put.posY = index.y;
		SocketManager::getInstance()->SendMsg((char*)&put, sizeof(put));
	}
}

void GameScene::update(float dt)
{
	int code = SocketManager::getInstance()->RecvMsg();
	switch (code)
	{
	case Message::START: {
		Start start = SocketManager::getInstance()->getStartMsg();
		pan->SetTurn(start.turn);
		break; }
	case Message::RESULT: {
		Result result = SocketManager::getInstance()->getResultMsg();
		pan->AddStone(result.isBlack, result.posX, result.posY);
		pan->SetTurn(result.turn);
		break; }
	case Message::ENDGAME: {
		EndGame endGame = SocketManager::getInstance()->getEndGameMsg();
		pan->SetTurn(false);
		break; }
	default:
		break;
	}
}
