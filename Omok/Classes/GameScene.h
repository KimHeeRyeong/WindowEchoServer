#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "OmokPan.h"
#include "MessageFormat.h"
USING_NS_CC;
class GameScene : public cocos2d::Scene
{
public:
    virtual bool init();
	void OnClickPutBtn();
	virtual void update(float dt);
	CREATE_FUNC(GameScene);
	
private:
	OmokPan* pan;
	ui::Button* btnPut;//Âø¼ö ¹öÆ°

	Sprite* blackStone;
	Sprite* whiteStone;
	Label* myNick;
	Label* otherNick;
	Point myPos;
	Point oppPos;

};

#endif // __GAME_SCENE_H__
