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
	bool isBlack;//돌 색상
	bool turn;//돌 놓을 차례 확인
	char nickOpponent[20];//상대 닉네임
};
struct Result :Code {
public:
	Result() :Code(Message::RESULT) {}
	int pos[2];//놓을 돌 위치
	bool isBlack;//놓을 돌 색상
	bool turn;//돌 놓을 차례 확인
};
struct EndGame :Code {
public:
	EndGame() :Code(Message::ENDGAME) {}
	int endCode;//0:정상end
	int pos[2];//놓을 돌 위치
	bool isBlack;//놓을 돌 색상
	bool isWin;//이겼나?
};
#pragma endregion

#pragma region ClentSend
struct PutStone :Code {
public:
	PutStone() : Code(Message::PUTSTONE) {}
	int pos[2];
};
#pragma endregion
