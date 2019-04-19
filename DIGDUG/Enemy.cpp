#include "DxLib.h"
#include "Enemy.h"
#include "GameTask.h"


Enemy::Enemy(std::weak_ptr<Obj> target, VECTOR2 chipOffSet,int EnemyType) :Player(KeyData, KeyDataOld, chipOffSet)
{
	for (int j = 0; j < sizeof(KeyData); j++)
	{
		KeyData[j] = 0;
		KeyDataOld[j] = 0;
	}
	for (int j = 0; j <= 5; j++)
	{
		if (EnemyType == 1)
		{
			EnemyBeaten[j] = LoadDivGraph("image/ìGÇÃÇ‚ÇÁÇÍ.png", 5, 5, 1, 24, 24, EnemyBeaten, true);
		}
		else if (EnemyType == 2)
		{
			EnemyBeaten[j] = LoadDivGraph("image/ìGÇÃÇ‚ÇÁÇÍ2.png", 5, 5, 1, 24, 24, EnemyBeaten, true);
			if (j <= 4)
			{
				flameAttack[j] = LoadDivGraph("image/âä.png", 4, 4, 1, 50, 16, flameAttack, true);
			}
		}
	}
	this->target = target;
	this->EnemyType = EnemyType;
	SetAttackPos(VECTOR2(0, 0));
}

Enemy::~Enemy()
{
	pos = { 0,0 };
}

bool Enemy::Update(void)
{
	/*if (EnemyType == 2)
	{
		DrawFormatString(300, 40, GetColor(255, 255, 255), "ìGÇÃìñÇΩÇËîªíËÅ@%d", BeatenCnt);
		DrawFormatString(300, 60, GetColor(255, 255, 255), "éÄñSéûä‘Å@%d", EnemyBeatenTime);
	}*/
	if (BeatenCnt == 0)
	{
		beatenFlag = false;
		BeatenTime = 0;
		SetMapCheck();
		SetMove();
	}
	else
	{
		if (BeatenTime++ % 120 == 119 && BeatenCnt < 4)
		{
			BeatenCnt--;
		}

		if (BeatenTime % 120 == 119 && BeatenCnt == 4)
		{
			BeatenCnt++;
		}

		if (BeatenTime % 120 >= 30)
		{
			EnemyHitTime = 0;
		}
	}
	return false;
}

void Enemy::Draw(void)
{
	if (BeatenCnt == 0)
	{
		Obj::Draw();
	}
	else
	{
		Obj::Draw(0);
		if (Dir == DIR_LEFT)
		{
			DrawExtendGraph(pos.x + CHIP_SIZE + (CHIP_CHAR_SIZE / 2), pos.y - (CHIP_CHAR_SIZE / 2), pos.x - (CHIP_CHAR_SIZE / 2), pos.y + CHIP_SIZE + (CHIP_CHAR_SIZE / 2), EnemyBeaten[BeatenCnt], true);
		}
		else
		{
			DrawExtendGraph(pos.x - (CHIP_CHAR_SIZE / 2), pos.y - (CHIP_CHAR_SIZE / 2), pos.x + CHIP_SIZE + (CHIP_CHAR_SIZE / 2), pos.y + CHIP_SIZE + (CHIP_CHAR_SIZE / 2), EnemyBeaten[BeatenCnt], true);
		}
		if (BeatenCnt >= 5)
		{
			if (EnemyBeatenTime == 0)
			{
				lpGameTask.AddScore(200 * EnemyType);
			}
			EnemyBeatenTime++;
			DrawString(pos.x +  (CHIP_CHAR_SIZE / 2), pos.y + (CHIP_CHAR_SIZE / 2), (EnemyType == 1 ? "200" : "400"),GetColor(255,255,255));
			if (EnemyBeatenTime > BeatenSetTime)
			{
				BeatenCnt = 6;
				pos = { -500,-500 };
				Obj::EnemyCount--;
				if (EnemyBeatenTime != 0)
				{
					Obj::OneUp = 400;
				}
				EnemyBeatenTime = 0;
			}
		}
	}
}

bool Enemy::Beaten(int count)
{
	BeatenTime = 0;
	if (EnemyHitTime == 0)
	{
		BeatenCnt += count;
		beatenFlag = true;
	}
	EnemyHitTime++;

	return false;
}


void Enemy::SetMove(void)
{
	int posX_BL = (pos.x % CHIP_SIZE);
	int posY_BL = (pos.y % CHIP_SIZE);

	/*DrawFormatString(200, 40, GetColor(255, 255, 255), "Xç¿ïW %d ", posX_BL);
	DrawFormatString(200, 60, GetColor(255, 255, 255), "Yç¿ïW %d ", posY_BL);*/

	if (!(lpGameTask.GameStartFlag))
	{
		checkFlag = true;
	}

	if (!SearchPlayer())
	{
		TurnCheck();
	}
	else
	{
		if (EnemyType == 2 && Turn == Dir)
		{
			checkFlag = true;
		}
	}

	if (!checkFlag)
	{
		playerCheck = false;
		if (EnemyChangeCnt <= 10)
		{
			if (Turn == DIR_RIGHT)
			{
				if (CheckChip(cpTypeR, LDR_CHIP_BLANK) || CheckChip(cpTypeR, EDIT_CHIP_UBL))
				{
					auto checkTypeR = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() + 1), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD);
					if (CheckChip(checkTypeR, EDIT_BGD_BLANK) || CheckChip(checkTypeR, LDR_CHIP_BLANK) || BGDCheck(Turn,checkTypeR))
					{
						if (pos.x + CHIP_SIZE < CHIP_SIZE * GAME_AREA_CNT_X)
						{
							Dir = DIR_RIGHT;

							if (posY_BL == 31)
							{
								pos.x += speed;
							}
							else
							{
								pos.y += speed;
							}
						}
					}
				}
			}
			else if (Turn == DIR_LEFT)
			{
				if (CheckChip(cpTypeL, LDR_CHIP_BLANK) || CheckChip(cpTypeL, EDIT_CHIP_UBL))
				{
					auto checkTypeL = lpMapCtl.GetMapData(pos + VECTOR2(-1, (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD);
					if (CheckChip(checkTypeL, EDIT_BGD_BLANK) || CheckChip(checkTypeL, LDR_CHIP_BLANK) || BGDCheck(Turn,checkTypeL))
					{
						if (pos.x >= CHIP_SIZE)
						{
							Dir = DIR_LEFT;
							if (posY_BL == 31)
							{
								pos.x -= speed;
							}
							else
							{
								pos.y -= speed;
							}
						}
					}
				}
			}
			else if (Turn == DIR_UP)
			{
				if (CheckChip(cpTypeU, LDR_CHIP_BLANK) || CheckChip(cpTypeU, EDIT_CHIP_WBL))
				{
					auto checkTypeU = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), -1), LDR_GP_SD);

					if (CheckChip(checkTypeU, EDIT_BGD_BLANK) || CheckChip(checkTypeU, LDR_CHIP_BLANK) || BGDCheck(Turn,checkTypeU))
					{
						if (pos.y >= CHIP_SIZE * 2)
						{
							if (posX_BL == 31)
							{
								pos.y -= speed;
							}
							else
							{
								pos.x += speed;
							}
						}
						else
						{
							count++;
						}
					}
				}
			}
			else
			{
				if (CheckChip(cpTypeD, LDR_CHIP_BLANK) || CheckChip(cpTypeD, EDIT_CHIP_WBL))
				{
					auto checkTypeD = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), lpMapCtl.GetChipSize() + 1), LDR_GP_SD);

					if (CheckChip(checkTypeD, EDIT_BGD_BLANK) || CheckChip(checkTypeD, LDR_CHIP_BLANK) || BGDCheck(Turn,checkTypeD))
					{
						if (pos.y < CHIP_SIZE * 14 - 1)
						{
							if (posX_BL == 31)
							{
								pos.y += speed;
							}
							else
							{
								pos.x -= speed;
							}
						}
					}
				}
				else
				{
					// Ãﬂ⁄≤‘∞Çå©Ç¬ÇØÇΩèÍçá
					playerCheck = true;
				}
			}
		}
		else
		{
			if (Obj::animCnt % 2 == 0)
			{
				if (pos.x < target.lock()->GetPos().x)
				{
					pos.x += speed;
				}
				else if (pos.x > target.lock()->GetPos().x)
				{
					pos.x -= speed;
				}
				if (pos.y < target.lock()->GetPos().y)
				{
					pos.y += speed;
				}
				else if (pos.y > target.lock()->GetPos().y)
				{
					pos.y -= speed;
				}
			}
			if (Obj::animCnt % 30 == 0)
			{
				if (cpTypeC == LDR_CHIP_BLANK || cpTypeC == EDIT_CHIP_UBL || cpTypeC == EDIT_CHIP_WBL)
				{
					if (posY_BL == 31 || posX_BL == 31)
					{
						EnemyChangeCnt = 0;
					}
				}
			}
		}
		if (Obj::animCnt % 60 == 0)
		{
			EnemyChangeCnt++;
		}
		EnemyChangeCnt > 10 ? SetAnim("ñ⁄ïœâª") : SetAnim("óßÇø");

	}
	else
	{
		SetAnim("âŒêÅÇ´");
		if (Obj::animCnt % 60 == 59)
		{
			flameFlag = true;
		}
		if (flameFlag == true)
		{
			if (Obj::animCnt % 10 == 9)
			{
				flameTime++;
			}
			if (flameTime >= 0)
			{
				if (Turn == DIR_LEFT)
				{
					DrawExtendGraph(pos.x, pos.y, pos.x - CHIP_SIZE - flameSize, pos.y + CHIP_SIZE, flameAttack[(BeatenCnt != 6 ? flameTime : 0)], true);
					SetAttackPos(VECTOR2(-(int)flameAttackPos * flameTime, CHIP_SIZE));
				}
				else
				{
					DrawExtendGraph(pos.x + CHIP_SIZE, pos.y, pos.x + (CHIP_SIZE * 2) + flameSize, pos.y + CHIP_SIZE, flameAttack[(BeatenCnt != 6 ? flameTime : 0)], true);
					SetAttackPos(VECTOR2((int)flameAttackPos * flameTime, CHIP_SIZE));
				}
			}
			if (flameTime > 3)
			{
				flameTime = 0;
				SetAttackPos(VECTOR2(0, 0));
				checkFlag = false;
				flameFlag = false;
			}
		}
	}

	Obj::animCnt++;
}

bool Enemy::TurnCheck(void)
{
	if ((timerChangeCnt != 0 && timerChangeCnt <= 31) || count >= 7)
	{
		if (!changeFlag)
		{
			changeFlag = true;
			count = 0;
		}
		else
		{
			changeFlag = false;
			count = 0;
		}
	}

	if (!changeFlag)
	{
		if (dir >= (int)DIR_MAX)
		{
			timerChangeCnt = timer;

			startCnt = DIR_MAX;
			while (dir > 0)
			{
				if (!CheckChip((DIR)dir))
				{
					dir--;
				}
				else
				{
					if (Turn != (DIR)dir)
					{
						if ((int)dir <= startCnt)
						{
							startCnt = (int)dir;
						}
						dir--;
					}
				}
				if (dir <= 0)
				{
					
					count++;
				}
				Turn = (DIR)startCnt;
			}
		}
	}
	else
	{

		if (dir <= (int)DIR_NON)
		{
			startCnt = DIR_NON;
			while (dir < 5)
			{
				if (!CheckChip((DIR)dir))
				{
					dir++;
				}
				else
				{
					if (Turn != (DIR)dir)
					{
						if ((int)dir >= startCnt)
						{
							startCnt = (int)dir;
						}
						dir++;
					}
				}
				if (dir >= (int)DIR_MAX)
				{
					count++;
				}
				Turn = (DIR)startCnt;
			}

		}
	}

	if (!CheckChip((DIR)dir))
	{
		if ((DIR)dir < DIR_MAX)
		{
			if (!changeFlag)
				dir++;
			else
				dir--;
		}
		else
		{
			dir--;
		}
		timerChangeCnt = timer;
		if(dir < (int)DIR_MAX)
		timer = 0;
	}
	else
	{
		if (Turn != (DIR)dir)
		{
			Turn = (DIR)dir;
			count++;
		}
		timer++;
	}


	return false;
}

bool Enemy::SearchPlayer(void)
{

	VECTOR2 tmpPos(pos / CHIP_SIZE);
	VECTOR2 targetPos(target.lock()->GetPos() / CHIP_SIZE);
	/*DrawFormatString(500, 40, GetColor(255, 255, 255), " %d ", targetPos.x);
	DrawFormatString(500, 60, GetColor(255, 255, 255), " %d ", targetPos.y);

	DrawFormatString(550, 40, GetColor(255, 255, 255), " %d ", tmpPos.x);
	DrawFormatString(550, 60, GetColor(255, 255, 255), " %d ", tmpPos.y);

	DrawFormatString(550, 80, GetColor(255, 255, 255), " %d ", SearchCnt.x / 32);
	DrawFormatString(550, 100, GetColor(255, 255, 255), " %d ", SearchCnt.y / 32);*/

	SearchCnt = { 0,0 };

	if ((tmpPos.x >= 1 && tmpPos.x < GAME_AREA_CNT_X) || (tmpPos.y >= 1 && tmpPos.y < GAME_AREA_CNT_Y))
	{
		for (int j = 0; j < 3; j++)
		{
			if (tmpPos.x + j == targetPos.x && tmpPos.y == targetPos.y)
			{
				
				Dir = DIR_RIGHT;

				return true;
			}
			if (tmpPos.x - j == targetPos.x && tmpPos.y == targetPos.y)
			{
				Dir = DIR_LEFT;

				return true;
			}
			if (tmpPos.y + j == targetPos.y && tmpPos.x == targetPos.x)
			{
				Dir = DIR_UP;
				return true;
			}
			if (tmpPos.y - j == targetPos.y && tmpPos.x == targetPos.x)
			{
				Dir = DIR_DOWN;
				return true;
			}
		}
	}

	return false;
}

bool Enemy::InitAnim(void)
{
	(EnemyType == 1 ? AddAnim("óßÇø", 1, 0, 2, 8) : AddAnim("óßÇø", 1, 1, 2, 8));
	(EnemyType == 1 ? AddAnim("ñ⁄ïœâª", 3, 0, 2, 8) : AddAnim("ñ⁄ïœâª", 3, 1, 2, 8));
	AddAnim("âŒêÅÇ´", 0, 1, 3, 8);

	return true;
}

bool Enemy::CheckChip(DIR dir)
{
	int rtn_id = false;
	if (dir == DIR_RIGHT)
	{
		if (CheckChip(cpTypeR, LDR_CHIP_BLANK) || CheckChip(cpTypeR, EDIT_CHIP_UBL))
		{
			if ((pos.x + CHIP_SIZE) + CHIP_SIZE < CHIP_SIZE * GAME_AREA_CNT_X - 3)
			{
				rtn_id = true;
			}
		}
	}

	if (dir == DIR_LEFT)
	{
		if (CheckChip(cpTypeL, LDR_CHIP_BLANK) || CheckChip(cpTypeL, EDIT_CHIP_UBL))
		{
			if (pos.x >= CHIP_SIZE)
			{
				rtn_id = true;
			}
		}
	}

	if (dir == DIR_DOWN)
	{
		if (CheckChip(cpTypeD, LDR_CHIP_BLANK) || CheckChip(cpTypeD, EDIT_CHIP_WBL))
		{
			if ((pos.y + CHIP_SIZE) < CHIP_SIZE * 14 - 1)
			{
				rtn_id = true;
			}
		}
	}

	if (dir == DIR_UP)
	{
		if (CheckChip(cpTypeU, LDR_CHIP_BLANK) || CheckChip(cpTypeU, EDIT_CHIP_WBL))
		{
			if (pos.y >= CHIP_SIZE * 2 - 4)
			{
				rtn_id = true;
			}
		}
	}

	return rtn_id;
}

bool Enemy::BGDCheck(DIR dir, CHIP_TYPE type)
{

	if (type == EDIT_CHIP_BGD_R || type == EDIT_CHIP_BGD_L || type == EDIT_CHIP_BGD_D || type == EDIT_BGD_CURVE_L || type == EDIT_BGD_CURVE_J)
	{
		return true;
	}
	return false;
}

bool Enemy::CheckChip(CHIP_TYPE dir, CHIP_TYPE chip)
{

	if (dir == chip)
	{
		
		return true;
	}
	return false;
}
