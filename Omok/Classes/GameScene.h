#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "OmokPan.h"
USING_NS_CC;
class GameScene : public cocos2d::Scene
{
public:
    virtual bool init();
	void OnClickPutBtn();

	CREATE_FUNC(GameScene);
	
private:
	OmokPan* pan;
	ui::Button* btnPut;//Âø¼ö ¹öÆ°
};

#endif // __GAME_SCENE_H__
