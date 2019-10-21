#pragma once
class OmokPan
{
public:
	OmokPan();
	~OmokPan();
	void initPan();
	bool CheckBlank(int posX, int posY);//�� �ڸ����� Ȯ��
	bool AddStone(bool isBlack, int posX, int posY);
	//return true->���� �ϼ�

private:
	int pan[15][15];//0:blank 1:black 2:white
	bool CheckVer(int state, int posX, int posY);
	bool CheckHor(int state, int posX, int posY);
	bool CheckDiagonal1(int state, int posX, int posY);
	bool CheckDiagonal2(int state, int posX, int posY);
};

