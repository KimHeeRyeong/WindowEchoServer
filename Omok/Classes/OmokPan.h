#ifndef __OMOK_PAN_H__
#define __OMOK_PAN_H__

#include "cocos2d.h"
#include <list>
USING_NS_CC;
using namespace std;
struct PanIndex {
	int x;
	int y;
	PanIndex() :x(-1), y(-1){} //x, y값 초기화
};
class OmokPan : public cocos2d::Node
{
public:
    virtual bool init();
	void InitDrawPan();//바둑판 그리기
	void InitTouchListener();

	bool OnTouchBegan(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);
	void OnTouchEnded(Touch* touch, Event* event);

	void SetTurn(bool setTurn);
	bool GetTurn();
	PanIndex GetPanIndex();
	void AddStone(bool isBlack,int posX,int posY);
	void initPanBlank();
	CREATE_FUNC(OmokPan);
	
private:
	Size winSize;
	Size blockSize;
	bool turn;
	int pan_width;//바둑판 가로칸(=세로칸) 수
	bool panBlank[15][15];//pan에 돌이 놓였는지 확인 true:돌 없음
	Sprite* target;
	PanIndex targetIndex;
	list<Sprite*> stones;

	//about touch
	Point panMin;//바둑판 내부 클릭 확인을 위한 변수
	Point panMax;
	bool movePan;//바둑판 이동을 위한 클릭이었는지 확인
	
	//function
	PanIndex PositionToPanIndex(Point pos);//position을 바둑판 index로 바꾸기
};

#endif // __OMOK_PAN_H__
