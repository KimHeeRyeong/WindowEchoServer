#include "WaitPopUp.h"

bool WaitPopUp::init()
{
	//popup background
	back = Sprite::create("back.png");
	back->setColor(Color3B(150, 150, 150));
	back->setScaleY(4.5);
	back->setScaleX(6);
	this->addChild(back);

	Rect backRc = back->getBoundingBox();
	//popup cancel btn
	btnCancel = ui::Button::create("btnCancel1.png", "btnCancel2.png");
	btnCancel->setScale(1.2f);
	btnCancel->setColor(Color3B(130, 130, 130));
	btnCancel->setPosition(Point(0, backRc.getMinY()+45));
	this->addChild(btnCancel);
	
	Sprite* title = Sprite::create("waitPopUpTitle.png");
	title->setScale(1.35);
	title->setPosition(0, backRc.getMaxY());
	this->addChild(title);

	//popup explain label
	labelStr = "pop up explain";
	label = Label::createWithTTF(labelStr,"fonts/arial.ttf", 23);
	label->setAnchorPoint(Point(0.5, 1));
	label->setPosition(0, backRc.getMaxY() - 55);
	label->setColor(Color3B(250,250,250));
	this->addChild(label);

	dotCount = 0;

	this->schedule(schedule_selector(WaitPopUp::dotMove),0.5);
	return true;
}

ui::Button * WaitPopUp::GetCancelButton()
{
	return btnCancel;
}

void WaitPopUp::SetLabel(string str)
{
	label->setString(str);
	labelStr = str;
}

void WaitPopUp::dotMove(float delta)
{
	switch (dotCount) {
	case 0:
		dotCount++;
		label->setString(labelStr+".");
		break;
	case 1:
		dotCount++;
		label->setString(labelStr+"..");
		break;
	case 2:
		dotCount++;
		label->setString(labelStr+"...");
		break;
	case 3:
		dotCount=0;
		label->setString(labelStr);
		break;
	default:
		dotCount = 0;
		label->setString(labelStr);
		break;
	}
}