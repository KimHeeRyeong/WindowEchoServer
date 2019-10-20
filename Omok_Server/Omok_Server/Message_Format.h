#pragma once
struct Code
{
public:
	int code;
protected:
	Code(int val=0):code(val){}
};
#pragma region ServerSend

struct Start :Code {
public:
	bool isBlack;//�� ����
	bool turn;//�� ���� ���� Ȯ��
	char nickOpponent[20];//��� �г���
private:
	Start() :Code(1) {}
};
struct Result :Code {
public:
	int pos[2];//���� �� ��ġ
	bool isBlack;//���� �� ����
	bool turn;//�� ���� ���� Ȯ��
private:
	Result() :Code(2) {}
};
struct EndGame :Code {
public:
	int endCode;//0:����end
	int pos[2];//���� �� ��ġ
	bool isBlack;//���� �� ����
	bool isWin;//�̰峪?
private:
	EndGame() :Code(3) {}
};
#pragma endregion

#pragma region ClentSend
struct PutStone :Code {
public:
	int pos[2];
private:
	PutStone() : Code(4) {}
};
#pragma endregion
