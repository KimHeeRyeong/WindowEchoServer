#include "StartScene.h"
#include "GameScene.h"

bool StartScene::init()
{
	winSize = Director::getInstance()->getWinSize();
	ui::Button* startBtn = CreateBtn("Start"
		, Color3B::RED
		, Vec2(winSize.width*0.5f, winSize.height*0.6f));
	startBtn->addClickEventListener(CC_CALLBACK_0(StartScene::OnClickStart, this));
	
	ui::Button* exitBtn = CreateBtn("Exit"
		, Color3B::GREEN
		, Vec2(winSize.width*0.5f, winSize.height*0.4f));
	exitBtn->addClickEventListener(CC_CALLBACK_0(StartScene::OnClickExit, this));

	return true;
}

void StartScene::OnClickStart()
{
	Director::getInstance()->replaceScene(GameScene::create());
}

void StartScene::OnClickExit()
{
	Director::getInstance()->end();
}

ui::Button * StartScene::CreateBtn(std::string name, Color3B color, Vec2 pos)
{
	ui::Button* btn = ui::Button::create("grey_button00.png"
		, "grey_button02.png", "grey_button01.png");
	btn->setScale(2);
	btn->setPosition(pos);
	btn->setColor(color);
	this->addChild(btn,0);

	Label* label = Label::createWithTTF(name, "fonts/arial.ttf",40);
	label->setPosition(pos);
	label->setColor(Color3B::WHITE);
	this->addChild(label,1);

	return btn;
}
