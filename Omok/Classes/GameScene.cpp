#include "GameScene.h"
#include "SocketManager.h"
#include "MainScene.h"

bool GameScene::init()
{
	Size winSize = Director::getInstance()->getWinSize();
	//back
	Sprite* back = Sprite::create("back.png");
	back->setColor(Color3B(230, 230, 230));
	back->setScale(30);
	back->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(back);
	
	blackStone = Sprite::create("blackStone.png");
	blackStone->setScale(2);
	this->addChild(blackStone);

	whiteStone = Sprite::create("whiteStone.png");
	whiteStone->setScale(2);
	this->addChild(whiteStone);

	myPos = Point(50, 60);
	oppPos = Point(myPos.x, winSize.height-myPos.y+10);

	//바둑판 세팅
	pan = OmokPan::create();
	pan->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(pan);

	//착수 버튼
	btnPut = ui::Button::create("btnPut1.png", "btnPut2.png");
	btnPut->addClickEventListener(CC_CALLBACK_0(GameScene::OnClickPutBtn, this));
	btnPut->setPosition(Point(winSize.width*0.5f, 50));
	this->addChild(btnPut);
	
	Start start = SocketManager::getInstance()->getStartMsg();
	pan->SetTurn(start.turn);
	btnPut->setVisible(start.turn);
	if (start.isBlack) {
		blackStone->setPosition(myPos);
		whiteStone->setPosition(oppPos);
	}
	else {
		blackStone->setPosition(oppPos);
		whiteStone->setPosition(myPos);
	}

	myNick = Label::createWithTTF(SocketManager::getInstance()->GetNickName(), "fonts/arial.ttf", 23);
	myNick->setColor(Color3B(80, 80, 80));
	myNick->setPosition(myPos.x, myPos.y-40);
	this->addChild(myNick);

	otherNick = Label::createWithTTF(start.nickOpponent, "fonts/arial.ttf", 23);
	otherNick->setColor(Color3B(80, 80, 80));
	otherNick->setPosition(oppPos.x,oppPos.y-40);
	this->addChild(otherNick);

	//endPopUp setting
	endPop = EndPopUp::create();
	endPop->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	endPop->StartGame();
	endPop->GetExitBtn()->addClickEventListener(CC_CALLBACK_0(GameScene::OnClickExitBtn, this));
	endPop->GetReplayBtn()->addClickEventListener(CC_CALLBACK_0(GameScene::OnClickRePlayBtn, this));
	this->addChild(endPop);
	this->scheduleUpdate();
    return true;
}

void GameScene::OnClickPutBtn()
{
	if (pan->GetTurn()) {
		PanIndex index = pan->GetPanIndex();
		if (index.x < 0)//항상 pan index>0이므로, 놓을 자리 선택이 안된 상태를 의미
			return;

		pan->SetTurn(false);
		btnPut->setVisible(false);
		SocketManager::getInstance()->SendPutStone(index.x, index.y);
	}
}

void GameScene::OnClickExitBtn()
{
	endPop->Exit();
	if (SocketManager::getInstance()->SocketIsOpen()) {
		SocketManager::getInstance()->CloseSocket();
	}
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(MainScene::create());
}

void GameScene::OnClickRePlayBtn()
{
	endPop->Ready();
	SocketManager::getInstance()->SendReplay();
}

void GameScene::update(float dt)
{
	int code = SocketManager::getInstance()->RecvMsg();
	switch (code)
	{
	case Message::START: {
		endPop->StartGame();
		Start start = SocketManager::getInstance()->getStartMsg();
		if (start.isBlack) {//흑돌 백돌 위치 조정
			blackStone->setPosition(myPos);
			whiteStone->setPosition(oppPos);
		}
		else {
			blackStone->setPosition(oppPos);
			whiteStone->setPosition(myPos);
		}
		pan->initPanBlank();
		pan->SetTurn(start.turn);
		btnPut->setVisible(start.turn);
		break; }
	case Message::RESULT: {
		Result result = SocketManager::getInstance()->getResultMsg();
		pan->AddStone(result.isBlack, result.posX, result.posY);
		pan->SetTurn(result.turn);
		btnPut->setVisible(result.turn);
		break; 
	}
	case Message::ENDGAME: {
		EndGame endGame = SocketManager::getInstance()->getEndGameMsg();
		pan->AddStone(endGame.isBlack, endGame.posX, endGame.posY);
		pan->SetTurn(false);
		btnPut->setVisible(false);
		
		endPop->SetEnd(endGame.isWin);
		break; 
	}
	case Message::EXITOPP:
		if (SocketManager::getInstance()->SocketIsOpen()) {
			SocketManager::getInstance()->CloseSocket();
		}
		pan->SetTurn(false);
		btnPut->setVisible(false);
		endPop->OppisExit();
		break;
	case Message::REPLAY:
		endPop->OppisReady();
		break;
	default:
		break;
	}
}
