#pragma once
enum Message
{
	NONE,
	START,
	RESULT,
	ENDGAME,
	PUTSTONE
};
struct Code
{
public:
	int code;
	Code(int val = Message::NONE) :code(val) {}
};
#pragma region ServerSend

struct Start :Code {
public:
	Start() :Code(Message::START) {}
	bool isBlack;//�� ����
	bool turn;//�� ���� ���� Ȯ��
	char nickOpponent[20] = "";//��� �г���
};
struct Result :Code {
public:
	Result() :Code(Message::RESULT) {}
	int posX;//���� �� ��ġ
	int posY;
	bool isBlack;//���� �� ����
	bool turn;//�� ���� ���� Ȯ��
};
struct EndGame :Code {
public:
	EndGame() :Code(Message::ENDGAME) {}
	int endCode;//0:����end
	int posX;//���� �� ��ġ
	int posY; 
	bool isBlack;//���� �� ����
	bool isWin;//�̰峪?
};
#pragma endregion

#pragma region ClentSend
struct PutStone :Code {
public:
	PutStone() : Code(Message::PUTSTONE) {}
	int posX;
	int posY;
};	
#pragma endregion
