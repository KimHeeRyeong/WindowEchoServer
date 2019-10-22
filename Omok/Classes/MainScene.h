#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "WaitPopUp.h"
#include "SocketManager.h"
#include "GameScene.h"
USING_NS_CC;
using namespace std;
class MainScene : public cocos2d::Scene
{
public:
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
private:
	Size winSize;

	ui::Button* btnStart;
	ui::Button* btnExit;
	WaitPopUp* popUp;

	void StartGame();
	void ExitGame();
	void CancelStart();

	virtual void update(float delta);
};

#endif // __MAIN_SCENE_H__
