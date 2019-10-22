#include "MainScene.h"

bool MainScene::init()
{
	winSize = Director::getInstance()->getWinSize();
	float margin = 70.0f;

	//set back
	Sprite* back = Sprite::create("back.png");
	back->setColor(Color3B(230, 230, 230));
	back->setScale(30);
	back->setPosition(winSize.width*0.5f, winSize.height*0.5f);
	this->addChild(back);

	//set title
	Sprite* title = Sprite::create("title.png");
	title->setPosition(winSize.width*0.5f, winSize.height*0.7f);
	this->addChild(title);

	//set Start button
	btnStart = ui::Button::create("btnStart1.png", "btnStart2.png");
	btnStart->setScale(1.2f);
	btnStart->setPosition(Point(winSize.width*0.5f, title->getBoundingBox().getMinY() - margin * 1.5f));
	SocketManager::getInstance()->SetNickName("user");
	btnStart->addClickEventListener(CC_CALLBACK_0(MainScene::StartGame, this));
	this->addChild(btnStart);

	//set Exit button
	btnExit = ui::Button::create("btnExit1.png", "btnExit2.png");
	btnExit->setScale(1.2f);
	btnExit->setPosition(Point(winSize.width*0.5f, btnStart->getBoundingBox().getMinY() - margin));
	btnExit->addClickEventListener(CC_CALLBACK_0(MainScene::ExitGame, this));
	this->addChild(btnExit);

	//set PopUp
	popUp = WaitPopUp::create();
	popUp->setPosition(winSize.width*0.5f, winSize.height*0.45f);
	popUp->SetLabel("Find Other User");
	popUp->setVisible(false);
	popUp->GetCancelButton()->addClickEventListener(CC_CALLBACK_0(MainScene::CancelStart, this));
	this->addChild(popUp);

	this->scheduleUpdate();
	return true;
}

void MainScene::StartGame()
{
	btnStart->setEnabled(false);
	btnExit->setEnabled(false);
	popUp->setVisible(true);
	SocketManager::getInstance()->StartSocket();
	SocketManager::getInstance()->SendNick();
}
void MainScene::ExitGame()
{
	Director::getInstance()->end();
}

void MainScene::CancelStart()
{
	SocketManager::getInstance()->CloseSocket();
	btnStart->setEnabled(true);
	btnExit->setEnabled(true);
	popUp->setVisible(false);
}

void MainScene::update(float delta)
{
	if (SocketManager::getInstance()->RecvMsg() == Message::START) {
		this->unscheduleUpdate();
		Director::getInstance()->replaceScene(GameScene::create());
	}
}
