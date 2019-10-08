#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
USING_NS_CC;
class StartScene : public cocos2d::Scene
{
public:
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);

private:
	Size winSize;

	void OnClickStart();
	void OnClickExit();
	ui::Button* CreateBtn(std::string,Color3B, Vec2);
};

#endif // __START_SCENE_H__
