#pragma once
#include "Player.h"
#include <array>

constexpr unsigned int flameSize = 50;
constexpr unsigned int flameAttackPos = 30;
constexpr unsigned int BeatenSetTime = 120;

class Enemy :
	public Player
{
public:
	Enemy(std::weak_ptr<Obj> target, VECTOR2 chipOffSet ,int EnemyType);
	~Enemy();

	bool Update(void);
	void Draw(void);

	bool Beaten(int count);
	bool BGDCheck(DIR dir, CHIP_TYPE type);



private:
	void SetMove(void);
	bool TurnCheck(void);
	bool SearchPlayer(void);

	bool InitAnim(void);
	bool CheckChip(DIR dir);
	bool CheckChip(CHIP_TYPE dir, CHIP_TYPE chip);

	bool TurnCnt = false;
	DIR Turn = DIR_NON;
	int dir = 1;
	int count = 0;
	char KeyData[256];
	char KeyDataOld[256];
	std::weak_ptr<Obj> target;

	int startCnt = (int)DIR_MAX;
	bool changeFlag = false;
	int timer = 0;
	int timerChangeCnt = 0;

	VECTOR2 SearchCnt = { 0,0 };
	bool checkFlag = false;
	bool playerCheck = false;

	// 攻撃が当たった時の処理関係
	int BeatenTime = 0;
	int EnemyBeaten[5];
	int flameAttack[4];
	bool beatenFlag = false;

	// 敵の無敵時間
	int EnemyHitTime = 0;
	// 死亡時の得点以降時間
	int EnemyBeatenTime = 0;

	// 目変化時の無敵時間とｱﾆﾒｰｼｮﾝの間隔
	int InvincibleTime = 0;
	int EnemyChangeCnt = 0;

	int EnemyType = 1;			// 敵の種類

	// 炎関係
	int flameTime = 0;
	bool flameFlag = false;

};

