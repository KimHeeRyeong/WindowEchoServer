#ifndef __OMOK_PAN_H__
#define __OMOK_PAN_H__

#include "cocos2d.h"
USING_NS_CC;

struct PanIndex {
	int x;
	int y;
	PanIndex() :x(-1), y(-1){} //x, y�� �ʱ�ȭ
};
class OmokPan : public cocos2d::Node
{
public:
    virtual bool init();
	void InitDrawPan();//�ٵ��� �׸���
	void InitTouchListener();

	bool OnTouchBegan(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);
	void OnTouchEnded(Touch* touch, Event* event);

	void SetTurn(bool setTurn);
	PanIndex GetPanIndex();
	CREATE_FUNC(OmokPan);
	
private:
	Size winSize;
	Size blockSize;
	bool turn;
	int pan_width;//�ٵ��� ����ĭ(=����ĭ) ��
	bool panBlank[15][15];//pan�� ���� �������� Ȯ�� true:�� ����
	Sprite* target;
	PanIndex targetIndex;

	//about touch
	Point panMin;//�ٵ��� ���� Ŭ�� Ȯ���� ���� ����
	Point panMax;
	bool movePan;//�ٵ��� �̵��� ���� Ŭ���̾����� Ȯ��
	
	//function
	PanIndex PositionToPanIndex(Point pos);//position�� �ٵ��� index�� �ٲٱ�
	void initPanBlank();
};

#endif // __OMOK_PAN_H__
