#include "OmokPan.h"



OmokPan::OmokPan()
{
	initPan();
}


OmokPan::~OmokPan()
{
}

void OmokPan::initPan()
{
	for (int col = 0; col < 15; col++) {
		for (int row = 0; row < 15; row++) {
			pan[row][col] = 0;
		}
	}
}

bool OmokPan::CheckBlank(int posX, int posY)
{
	if (pan[posX][posY] == 0) {
		return true;
	}
	return false;
}

bool OmokPan::AddStone(bool isBlack, int posX, int posY)
{
	int state = 0;
	if (isBlack) {
		state = 1;
	}
	else {
		state = 2;
	}
	pan[posX][posY] = state;
	
	//오목 여부 체크
	if (CheckVer(state, posX, posY) || CheckHor(state, posX, posY)
		|| CheckDiagonal1(state, posX, posY) || CheckDiagonal2(state, posX, posY)) {
		return true;
	}
	return false;
}
//state 1:black, 2:white
bool OmokPan::CheckVer(int state, int posX, int posY)
{
	int omok = 1;
	
	//check up
	for (int i = posX - 1; i >= 0; i--)
	{
		if (pan[i][posY] == state)
			omok++;
		else
			break;
	}
	//check down
	for (int i = posX + 1; i < 15; i++)
	{
		if (pan[i][posY] == state)
			omok++;
		else
			break;
	}

	if (omok == 5)
		return true;

	return false;
}
bool OmokPan::CheckHor(int state, int posX, int posY)
{
	int omok = 1;
	//check left
	for (int i = posY - 1; i >= 0; i--)
	{
		if (pan[posX][i] == state)
			omok++;
		else
			break;
	}
	//check right
	for (int i = posY + 1; i < 15; i++)
	{
		if (pan[posX][i] == state)
			omok++;
		else
			break;
	}

	if (omok == 5)
		return true;

	return false;
}
//   ↘:this direction
bool OmokPan::CheckDiagonal1(int state, int posX, int posY)
{
	int omok = 1;
	//check top left
	int j = posY - 1;
	for (int i = posX - 1; i >= 0 && j >= 0; i--)
	{
		if (pan[i][j] == state)
		{
			omok++;
			j--;
		}
		else
			break;
	}
	//check bottom right
	j = posY + 1;
	for (int i = posX + 1; i < 15 && j < 15; i++)
	{
		if (pan[i][j] == state)
		{
			omok++;
			j++;
		}
		else
			break;
	}

	if (omok == 5)
		return true;

	return false;
}
//  ↙ :this direction
bool OmokPan::CheckDiagonal2(int state, int posX, int posY)
{
	int omok = 1;
	//check top right
	int j = posY + 1;
	for (int i = posX - 1; i >= 0 && j < 15; i--)
	{
		if (pan[i][j] == state)
		{
			omok++;
			j++;
		}
		else
			break;
	}
	//check bottom left
	j = posY - 1;
	for (int i = posX + 1; i < 15 && j >= 0; i++)
	{
		if (pan[i][j] == state)
		{
			omok++;
			j--;
		}
		else
			break;
	}

	if (omok == 5)
		return true;

	return false;
}