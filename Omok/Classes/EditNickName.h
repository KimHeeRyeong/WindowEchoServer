#ifndef __EDIT_NICK_NAME_H__
#define __EDIT_NICK_NAME_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include <string>
USING_NS_CC;
using namespace std;
class EditNickName : public cocos2d::Node
{
public:
    virtual bool init();
	ui::Button* GetBtnOK();
	string GetEditNickName();
	EventListenerKeyboard* GetEventListener();

    CREATE_FUNC(EditNickName);
private:
	Label* label;
	ui::Button* btnOk;
	EventListenerKeyboard* listener;
	virtual void OnKeyPressed(EventKeyboard::KeyCode key);
};

#endif // __EDIT_NICK_NAME_H__
