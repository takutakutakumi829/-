#pragma once
#include "Obj.h"
#include "VECTOR2.h"
class Player :
	public Obj
{
public:
	Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffSet);
	~Player();

	bool Update(void);

	bool Beaten(int count);

private:
	void SetMove(void);
	void StartAnim(void);
	void CheckKey(void);
	bool CheckChip(DIR dir, CHIP_TYPE type);
	bool CheckChip(DIR dir, DIR dir2, CHIP_TYPE type, CHIP_TYPE range1, CHIP_TYPE range2);
	bool InitAnim(void);

	void AnimDirChange(void);

	// 掘る処理
	void Dig(void);

	// 掘れる場所かどうかの処理
	bool DigCheck(CHIP_TYPE type);

	// 攻撃処理
	void SetAttack(void);
	// ｽｺｱ関係

	int animAdd = 0;
	int count = 0;

	DIR PLDir = DIR_NON;
	DIR PLDirOld = DIR_NON;
	DIR PLDirOldOld = DIR_NON;
	int PlayerCenter = 15;		// キャラの中心座標
	int posX_BL = (pos.x % CHIP_SIZE);
	int posY_BL = (pos.y % CHIP_SIZE);


	// やられ用
	int animCnt = 0;

	// 攻撃用
	int attackCnt = 0;

	// 得点
	int HighScore = 10000;
	bool IKARIflag = false;

	VECTOR2 IKARICnt = { 0,0 };

	VECTOR2 attackPos = { 0,0 };

};

