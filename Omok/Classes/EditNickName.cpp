#include "EditNickName.h"
#include "SocketManager.h"
bool EditNickName::init()
{
	//background
	Sprite* back = Sprite::create("back.png");
	back->setColor(Color3B(150, 150, 150));
	back->setScaleY(4.5);
	back->setScaleX(8);
	this->addChild(back);

	Rect backRc = back->getBoundingBox();
	//ok btn
	btnOk = ui::Button::create("btnOk1.png", "btnOk2.png");
	btnOk->setScale(1.2f);
	btnOk->setColor(Color3B(130, 130, 130));
	btnOk->setPosition(Point(0, backRc.getMinY() + 45));
	this->addChild(btnOk);

	//label nick
	string nick = SocketManager::getInstance()->GetNickName();
	label = Label::createWithTTF(nick, "fonts/arial.ttf", 30);
	label->setAnchorPoint(Point(0.5, 1));
	label->setColor(Color3B::BLACK);
	label->setPosition(0, backRc.getMaxY() - 45);
	label->setColor(Color3B(250, 250, 250));
	this->addChild(label);

	listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_1(EditNickName::OnKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
	return true;
}
ui::Button * EditNickName::GetBtnOK()
{
	return btnOk;
}
string EditNickName::GetEditNickName()
{
	return label->getString();
}
EventListenerKeyboard * EditNickName::GetEventListener()
{
	return listener;
}
void EditNickName::OnKeyPressed(EventKeyboard::KeyCode key)
{
	switch (key)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE: {
		string str = label->getString();
		if (str.size() > 0) {
			str.pop_back();
		}
		label->setString(str);
		break; }
#pragma region case number:
	case cocos2d::EventKeyboard::KeyCode::KEY_0:
	case cocos2d::EventKeyboard::KeyCode::KEY_1:
	case cocos2d::EventKeyboard::KeyCode::KEY_2:
	case cocos2d::EventKeyboard::KeyCode::KEY_3:
	case cocos2d::EventKeyboard::KeyCode::KEY_4:
	case cocos2d::EventKeyboard::KeyCode::KEY_5:
	case cocos2d::EventKeyboard::KeyCode::KEY_6:
	case cocos2d::EventKeyboard::KeyCode::KEY_7:
	case cocos2d::EventKeyboard::KeyCode::KEY_8:
	case cocos2d::EventKeyboard::KeyCode::KEY_9:
#pragma endregion
	{
		string str = label->getString();
		str.push_back((char)((int)key-28));//0:48
		label->setString(str);
	}
	break;
#pragma region case alphabet:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
	case cocos2d::EventKeyboard::KeyCode::KEY_B:
	case cocos2d::EventKeyboard::KeyCode::KEY_C:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
	case cocos2d::EventKeyboard::KeyCode::KEY_E:
	case cocos2d::EventKeyboard::KeyCode::KEY_F:
	case cocos2d::EventKeyboard::KeyCode::KEY_G:
	case cocos2d::EventKeyboard::KeyCode::KEY_H:
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
	case cocos2d::EventKeyboard::KeyCode::KEY_L:
	case cocos2d::EventKeyboard::KeyCode::KEY_M:
	case cocos2d::EventKeyboard::KeyCode::KEY_N:
	case cocos2d::EventKeyboard::KeyCode::KEY_O:
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
	case cocos2d::EventKeyboard::KeyCode::KEY_Q:
	case cocos2d::EventKeyboard::KeyCode::KEY_R:
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	case cocos2d::EventKeyboard::KeyCode::KEY_T:
	case cocos2d::EventKeyboard::KeyCode::KEY_U:
	case cocos2d::EventKeyboard::KeyCode::KEY_V:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
	case cocos2d::EventKeyboard::KeyCode::KEY_X:
	case cocos2d::EventKeyboard::KeyCode::KEY_Y:
	case cocos2d::EventKeyboard::KeyCode::KEY_Z:
#pragma endregion
	{
		string str = label->getString();
		if (str.size() < 20) {
			str.push_back((char)((int)key - 27));//a:97
			label->setString(str);
		}
	 }
	break;
	default:
		break;
	}
}
