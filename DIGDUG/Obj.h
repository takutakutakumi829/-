#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "VECTOR2.h"
#include "MapCtl.h"

#define GAME_EDIT_SIZE_X 32
#define GAME_EDIT_SIZE_Y 32

// 優先順位
// RIGHT>DOWN>LEFT>UP
enum DIR {		// 方向
	DIR_NON,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
	DIR_MAX,
};

enum ANIM_TBL {			// ｱﾆﾒｰｼｮﾝﾃｰﾌﾞﾙ
	ANIM_TBL_START_ID,
	ANIM_TBL_FRAME,
	ANIM_TBL_INV,
	ANIM_TBL_MAX,
};

class Obj
{
public:
	//Obj();
	Obj(const char (&_keyData)[256], const char (&_keyDataOld)[256], VECTOR2 chipOffset);
	~Obj();
	
	void init(std::string filname, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed);
	virtual bool Update(void);			//	更新処理
	virtual void Draw(void);			//	描画用

	virtual bool Beaten(int count);			//　やられ


	const bool& GetBeatenFlag(void);
	void SetBeatenFlag(const bool& BeatenFlag);

	const int& GetEnemyCnt(void);
	void AddEnemyCnt(int Cnt);

	const bool& GetBeatenCnt(void);


	// 攻撃関係
	void SetAttackPos(VECTOR2 pos);
	const VECTOR2& GetAttackPos(void);


	void Draw(int id);
	const VECTOR2& GetSize(void);
	void SetSize(const VECTOR2& divSize);
	bool SetPos(VECTOR2 pos);
	const VECTOR2& GetPos(void);
	bool SetAnim(std::string animName);
	std::string GetAnim(void);

private:
	virtual bool InitAnim(void);
	std::string imageName;
	VECTOR2 divSize = { 0,0 };			//	描画ﾁｯﾌﾟのｻｲｽﾞ
	VECTOR2 divCnt = { 0,0 };			//	描画ﾁｯﾌﾟの数
	VECTOR2 chipOffset = { 0,0 };			//	描画ﾁｯﾌﾟのｲﾒｰｼﾞIDの開始位置
	std::map<std::string, int[ANIM_TBL_MAX]> animTable;
	std::string animName;

protected:
	const char (&keyData)[256];
	const char (&keyDataOld)[256];

	virtual void SetMove(void);

	// ｷｰ入力用のﾁｪｯｸ関数
	void SetMapCheck(void);
	// 体力
	void LifeDraw(int life);
	int PlayerLife = 2;


	bool AddAnim(std::string animName, int id_x, int id_y, int Frame, int inv);

	VECTOR2 pos = { 0,0 };			//	描画位置
	VECTOR2 drawOffset;
	int speed;						//	ｽﾋﾟｰﾄﾞ
	DIR Dir = DIR_NON;
	unsigned int animCnt = 0;

	bool BeatenFlag = false;
	int BeatenCnt = 0;

	int EnemyCount = 0;
	int OneUp = 0;


	// 上下左右
	CHIP_TYPE cpTypeU, cpTypeD, cpTypeR, cpTypeL;

	// 上下左右のチェック
	CHIP_TYPE cpTypeUD, cpTypeDU, cpTypeRL, cpTypeLR;

	// 左上　右上　左下　右下
	CHIP_TYPE cpTypeUL, cpTypeUR, cpTypeDL, cpTypeDR;

	// 上下の斜め左右
	CHIP_TYPE cpTypeURU, cpTypeULU, cpTypeDRD, cpTypeDLD;

	// 左右の斜め上下
	CHIP_TYPE cpTypeURR, cpTypeDRR, cpTypeULL, cpTypeDLL;

	// 中心
	CHIP_TYPE cpTypeC;

	VECTOR2 attackPos = { 0,0 };

};

