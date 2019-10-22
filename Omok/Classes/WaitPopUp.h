#ifndef __WAIT_POP_UP_H__
#define __WAIT_POP_UP_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include <string>
using namespace std;
USING_NS_CC;
class WaitPopUp : public cocos2d::Node
{
public:
    virtual bool init();
	ui::Button* GetCancelButton();
	void SetLabel(string str);
	void dotMove(float delta);
    // implement the "static create()" method manually
    CREATE_FUNC(WaitPopUp);
private:
	Sprite* back;
	ui::Button* btnCancel;
	Label* label;
	string labelStr;

	int dotCount;
};

#endif // __WAIT_POP_UP_H__
