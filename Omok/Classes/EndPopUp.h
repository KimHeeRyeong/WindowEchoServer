#ifndef __END_POP_UP_H__
#define __END_POP_UP_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;
class EndPopUp : public cocos2d::Node
{
public:
    virtual bool init();
	void SetEnd(bool isWin);
	void OppisExit();
	void OppisReady();
	void Ready();
	void StartGame();
	void Exit();

	ui::Button* GetExitBtn();
	ui::Button* GetReplayBtn();
    // implement the "static create()" method manually
    CREATE_FUNC(EndPopUp);
private:
	Size winSize;
	Sprite* win;
	Sprite* lose;

	Label* label;

	ui::Button* exit;
	ui::Button* replay;
};

#endif // __END_POP_UP_H__
