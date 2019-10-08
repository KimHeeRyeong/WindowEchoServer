#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class GameScene : public cocos2d::Scene
{
public:
	virtual bool init();
	void CreatePan();
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

private:
	Size _winSize;
	Size _szBlock;
	Size _margin;
	Sprite* _stone;
	bool _myTurn;

	void OnMouseDown(EventMouse* mouse);
	void OnMouseMove(EventMouse* mouse);
	void OnMouseUp(EventMouse* mouse);
};

#endif // __GAME_SCENE_H__
