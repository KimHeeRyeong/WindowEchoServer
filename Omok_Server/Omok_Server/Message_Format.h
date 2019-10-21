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
	bool isBlack;//돌 색상
	bool turn;//돌 놓을 차례 확인
	char nickOpponent[20];//상대 닉네임
private:
	Start() :Code(1) {}
};
struct Result :Code {
public:
	int pos[2];//놓을 돌 위치
	bool isBlack;//놓을 돌 색상
	bool turn;//돌 놓을 차례 확인
private:
	Result() :Code(2) {}
};
struct EndGame :Code {
public:
	int endCode;//0:정상end
	int pos[2];//놓을 돌 위치
	bool isBlack;//놓을 돌 색상
	bool isWin;//이겼나?
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
