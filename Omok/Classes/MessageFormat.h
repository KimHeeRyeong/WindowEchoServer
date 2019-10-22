#pragma once
enum Message
{
	NONE,
	START,
	RESULT,
	ENDGAME,
	PUTSTONE,
	EXITOPP,
	NICKNAME,
	REPLAY,
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
	int posX;//���� �� ��ġ
	int posY;
	bool isBlack;//���� �� ����
	bool isWin;//�̰峪?
};
struct ExitOpp :Code {//��� ����
	ExitOpp() :Code(Message::EXITOPP) {}
};
#pragma endregion

#pragma region ClentSend
struct PutStone :Code {
public:
	PutStone() : Code(Message::PUTSTONE) {}
	int posX;
	int posY;
};
struct NickName :Code {
public:
	NickName() :Code(Message::NICKNAME) {}
	char nick[20];
};
struct RePlay :Code {
public:
	RePlay() :Code(Message::REPLAY) {}
};
#pragma endregion
