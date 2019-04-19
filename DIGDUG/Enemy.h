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

	// �U���������������̏����֌W
	int BeatenTime = 0;
	int EnemyBeaten[5];
	int flameAttack[4];
	bool beatenFlag = false;

	// �G�̖��G����
	int EnemyHitTime = 0;
	// ���S���̓��_�ȍ~����
	int EnemyBeatenTime = 0;

	// �ڕω����̖��G���ԂƱ�Ұ��݂̊Ԋu
	int InvincibleTime = 0;
	int EnemyChangeCnt = 0;

	int EnemyType = 1;			// �G�̎��

	// ���֌W
	int flameTime = 0;
	bool flameFlag = false;

};

