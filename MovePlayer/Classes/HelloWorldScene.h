#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "../proj.win32/Player.h"
USING_NS_CC;
class HelloWorld : public cocos2d::Scene
{
public:
    virtual bool init();
    CREATE_FUNC(HelloWorld);
private:
	Player* player;
};

#endif // __HELLOWORLD_SCENE_H__
