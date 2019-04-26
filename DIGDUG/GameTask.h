#pragma once
#include <string>
#include "VECTOR2.h"
#include <vector>
#include <memory>
#include <list>
#include "Obj.h"


typedef std::shared_ptr<Obj> obj_ptr;
typedef std::list<obj_ptr> obj_List;
#define lpGameTask GameTask::GetInstance()
//#define SCREEN_SIZE_X (600)
//#define SCREEN_SIZE_Y (500)//600

constexpr unsigned int SCREEN_SIZE_X = 600;
constexpr unsigned int SCREEN_SIZE_Y = 500;

constexpr unsigned int TitleCallsetX = (SCREEN_SIZE_X / 2) / 2;
constexpr unsigned int TitleCallsetY = (SCREEN_SIZE_Y / 2) / 2;

class GameTask
{
public:
	static void Create();
	static void Destroy();

	static GameTask &GetInstance() 
	{
		return *s_Instance;
	}
	int Update(void);
	void SetOffset(VECTOR2 pos);
	const VECTOR2 &GetOffset(void);

	void SetStartFlag(bool AnimFlag);

	bool CheckEditMode(void);

	void AddScore(int score);

	const int& GetClearCnt(void);

	const VECTOR2& GetCheckStone(void);
	void SetCheckStone(VECTOR2 check);


	char keyData[256];
	char keyDataOld[256];
	int PlayerLife = 2;
	int HighScore = 10000;
	int One_UP = 0;
	bool GameStartFlag = false;

private:


	GameTask();
	~GameTask();

	int SysInit(void);
	int SysDestroy(void);

	std::list<obj_ptr>::iterator AddObjlist(obj_ptr && objPtr);

	int Title(void);

	int GameInit(void);
	int GameMain(void);

	int EditInit(void);
	int EditMain(void);

	int Result(void);

	int GameDestroy(void);
	int (GameTask::*GtskPtr)(void);

	obj_List objList;

	std::list<obj_ptr>::iterator player;
	std::list<obj_ptr>::iterator stone;
	std::list<obj_ptr>::iterator enemy1;
	std::list<obj_ptr>::iterator enemy2;

	VECTOR2 drawOffset;
	VECTOR2 ChipSize;
	VECTOR2 TitleCallSize = { 300,80 };
	VECTOR2 checkStone = { 0,0 };

	int TitleCallMove = 300;

	// ∂≥›ƒånìù
	int OPCnt = 0;
	int StartCnt = 0;
	int StageCnt = 0;
	int ClearCnt = 0;

	static GameTask *s_Instance;

};

int DrawLine(VECTOR2 vec1, VECTOR2 vec2, unsigned int Color, int Thickness = 1);
