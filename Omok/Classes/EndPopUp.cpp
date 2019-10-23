#include "EndPopUp.h"

bool EndPopUp::init()
{
	winSize = Director::getInstance()->getWinSize();

	win = Sprite::create("win.png");
	win->setPosition(0, 120);
	this->addChild(win);
	
	lose = Sprite::create("lose.png");
	lose->setPosition(0, 120);
	this->addChild(lose);

	label = Label::createWithTTF("", "fonts/arial.ttf", 20);
	this->addChild(label);

	//set Start button
	replay = ui::Button::create("btnStart1.png", "btnStart2.png");
	replay->setPosition(Point(-120, -150));
	this->addChild(replay);

	//set Exit button
	exit = ui::Button::create("btnExit1.png", "btnExit2.png");
	exit->setPosition(Point(120, -150));
	this->addChild(exit);
	return true;
}

void EndPopUp::SetEnd(bool isWin)
{
	replay->setVisible(true);
	exit->setPosition(Point(120, -150));
	if (isWin) {
		win->setVisible(true);
	}
	else {
		lose->setVisible(true);
	}
}

void EndPopUp::OppisExit()
{
	exit->setPosition(Point(0, -150));
	replay->setVisible(false);
	label->setString("opp is out");
	label->setVisible(true);
}

void EndPopUp::OppisReady()
{
	label->setString("opp is ready");
	label->setVisible(true);
}

void EndPopUp::Ready()
{
	exit->setPosition(Point(0, -150));
	label->setString("ready");
	label->setVisible(true);
	replay->setVisible(false);
}

void EndPopUp::StartGame()
{
	win->setVisible(false);
	lose->setVisible(false);
	label->setVisible(false);
	replay->setVisible(false);
	exit->setPosition(Point(winSize.width*0.5f - 100,winSize.height*0.5f-50));
}

void EndPopUp::Exit()
{
	exit->setEnabled(false);
}

ui::Button * EndPopUp::GetExitBtn()
{
	return exit;
}

ui::Button * EndPopUp::GetReplayBtn()
{
	return replay;
}
