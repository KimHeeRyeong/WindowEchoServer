#include "MainScene.h"
#include "GameScene.h"
#include "SocketManager.h"
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
	btnStart->addClickEventListener(CC_CALLBACK_0(MainScene::StartGame, this));
	this->addChild(btnStart);

	//set Exit button
	btnExit = ui::Button::create("btnExit1.png", "btnExit2.png");
	btnExit->setScale(1.2f);
	btnExit->setPosition(Point(winSize.width*0.5f, btnStart->getBoundingBox().getMinY() - margin));
	btnExit->addClickEventListener(CC_CALLBACK_0(MainScene::ExitGame, this));
	this->addChild(btnExit);

	//set Setting button
	btnSet= ui::Button::create("setting.png");
	btnSet->setPosition(Point(50, winSize.height-50));
	btnSet->addClickEventListener(CC_CALLBACK_0(MainScene::SetGame, this));
	this->addChild(btnSet);

	//set PopUp
	popUp = WaitPopUp::create();
	popUp->setPosition(winSize.width*0.5f, winSize.height*0.45f);
	popUp->SetLabel("Find Other User");
	popUp->setVisible(false);
	popUp->GetCancelButton()->addClickEventListener(CC_CALLBACK_0(MainScene::CancelStart, this));
	this->addChild(popUp);

	//set label
	label = Label::createWithTTF("Socket connect fail", "fonts/arial.ttf", 20);
	label->setPosition(Point(winSize.width*0.5f, btnExit->getBoundingBox().getMinY() - margin));
	label->setColor(Color3B(80,80,80));
	label->setOpacity(0);
	this->addChild(label,2);

	this->scheduleUpdate();
	return true;
}

void MainScene::StartGame()
{
	if (!SocketManager::getInstance()->SocketIsOpen()) {
		btnStart->setEnabled(false);
		if (SocketManager::getInstance()->StartSocket()) {//소켓 연결에 성공시
			btnSet->setEnabled(false);
			btnExit->setEnabled(false);
			popUp->setVisible(true);
			SocketManager::getInstance()->SendNick();//닉네임 send
		}
		else {
			label->setOpacity(255);
			label->stopAllActions();

			FadeOut* fade = FadeOut::create(1);

			label->runAction(fade);
			btnStart->setEnabled(true);
		}
	}
}
void MainScene::ExitGame()
{
	Director::getInstance()->end();
}

void MainScene::SetGame()
{
	btnSet->setEnabled(false);
	btnExit->setEnabled(false);
	btnStart->setEnabled(false);

	EditNickName* nick = EditNickName::create();
	nick->setPosition(winSize.width*0.5f, winSize.height*0.45f);
	nick->GetBtnOK()->addClickEventListener(CC_CALLBACK_0(MainScene::EndSetting, this,nick));
	this->addChild(nick);
}

void MainScene::EndSetting(EditNickName* nick)
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(nick->GetEventListener());
	string str = nick->GetEditNickName();
	if (str.size() > 0) {
		SocketManager::getInstance()->SetNickName(str);
	}
	btnSet->setEnabled(true);
	btnExit->setEnabled(true);
	btnStart->setEnabled(true);
	this->removeChild(nick);
}

void MainScene::CancelStart()
{
	if (SocketManager::getInstance()->SocketIsOpen()) {
		SocketManager::getInstance()->CloseSocket();
	}
	btnSet->setEnabled(true);
	btnStart->setEnabled(true);
	btnExit->setEnabled(true);
	popUp->setVisible(false);
}

void MainScene::update(float delta)
{
	if (SocketManager::getInstance()->SocketIsOpen()) {
		if (SocketManager::getInstance()->RecvMsg() == Message::START) {
			this->unscheduleUpdate();
			Director::getInstance()->replaceScene(GameScene::create());
		}
	}
}