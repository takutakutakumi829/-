#include "DxLib.h"
#include "GameTask.h"
#include "MapCtl.h"
#include "Player.h"
#include "ImageMng.h"



Player::Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffSet) :Obj(_keyData, _keyDataOld, chipOffSet)
{
	count = 0;
	IKARIflag = false;
}


Player::~Player()
{

}

bool Player::Update(void)
{

	if (BeatenFlag == true)
	{
		Beaten(0);
	}
	else
	{
		if (GameTask::GetInstance().GetClearCnt() <= 0)
		{
			if (!IKARIflag)
			{
				(!(GameTask::GetInstance().GameStartFlag) ? StartAnim() : CheckKey());

				SetMapCheck();

				// à⁄ìÆÇ‚å@ÇÈìÆçÏ
				SetMove();
			}
			// çUåÇ
			SetAttack();
		}
	}
	PlayerLife = GameTask::GetInstance().PlayerLife;
	for (int life = 0; life < PlayerLife; life++)
	{
		LifeDraw(life);
	}
	return false;
}

bool Player::Beaten(int count)
{

	if (animCnt++ % 10 == 0)
	{
		if(animCnt > 60)
		Obj::animCnt++;
	}
	SetAnim("Ç‚ÇÁÇÍ");

	if (animCnt > 410)
	{
		animCnt = 0;
	}
	return (animCnt >= 410 ? true : false);
}

void Player::SetMove(void)
{
	if (count == 0)
	{
		SetAnim("óßÇø");
	}
	count++;

	posX_BL = (pos.x % CHIP_SIZE);
	posY_BL = (pos.y % CHIP_SIZE);

	auto changeBlack = [&](const VECTOR2 tmpPos, const CHIP_GRP grpType, const CHIP_TYPE type){

		if (lpMapCtl.GetMapData(pos + tmpPos, grpType) == type)
		{
			lpMapCtl.SetMapData(pos + tmpPos, EDIT_BGD_BLANK);
			return true;
		}

		return false;
	};

	// âEï˚å¸
	if (PLDir == DIR_RIGHT)
	{
		Obj::animCnt++;

		if (lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() + 1), (lpMapCtl.GetChipSize() / 2)), LDR_GP_ITEM) == LDR_CHIP_BLANK ||
			lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() + 1), (lpMapCtl.GetChipSize() / 2)), LDR_GP_ITEM) == ITEM_CHIP_BLANK)
		{
			if (cpTypeR == EDIT_CHIP_WBL || cpTypeR == EDIT_CHIP_BBL05 || cpTypeR == EDIT_CHIP_BBL05_L || CheckChip(PLDir, cpTypeR))
			{
				SetAnim("â°ñx");
				if (pos.x + CHIP_SIZE < CHIP_SIZE * GAME_AREA_CNT_X - 3)
				{


					if ((posY_BL == 31))
					{
						pos.x += speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.y += speed * (PLDirOldOld == DIR_DOWN ? 1 : -1);
					}

					if (CheckChip(PLDir, cpTypeC))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), (cpTypeC >= EDIT_CHIP_BBL75_U && cpTypeC <= EDIT_CHIP_BBL25_U) ? EDIT_CHIP_BBL_BLACK_UM : EDIT_CHIP_BBL_BLACK_U);
					}

					if (cpTypeLR == EDIT_CHIP_WBL || cpTypeR == EDIT_CHIP_BBL05 || cpTypeR == EDIT_CHIP_BBL05_L || CheckChip(PLDir, cpTypeLR))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize(), lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
					}
				}
			}
			else if (!(cpTypeR >= EDIT_CHIP_BBL && cpTypeR <= EDIT_CHIP_BBL05) && !(cpTypeUR >= EDIT_CHIP_BBL && cpTypeUR <= EDIT_CHIP_BBL05) && !(cpTypeDR >= EDIT_CHIP_BBL && cpTypeDR <= EDIT_CHIP_BBL05))
			{

				SetAnim("ï‡Ç≠");
				if (pos.x + CHIP_SIZE < CHIP_SIZE * GAME_AREA_CNT_X - 1)
				{
					if ((posY_BL == 31))
					{
						pos.x += speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();

						pos.y += speed * (PLDirOldOld == DIR_DOWN ? 1 : -1);
					}
				}
			}
			else if (((cpTypeR == EDIT_CHIP_BBL) || (cpTypeR == EDIT_CHIP_BBL75) || (cpTypeR == EDIT_CHIP_BBL50) || (cpTypeR == EDIT_CHIP_BBL25) || (cpTypeR == EDIT_CHIP_BBL05)) && (cpTypeURR != LDR_CHIP_BLANK) && (cpTypeDRR != LDR_CHIP_BLANK))
			{
				SetAnim("â°ñx");
				if (pos.x + CHIP_SIZE < CHIP_SIZE * GAME_AREA_CNT_X - 3)
				{

					if ((posY_BL == 31))
					{
						if ((Obj::animCnt % 20) == 0)
						{
							GameTask::GetInstance().AddScore(10);
						}
						pos.x += speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();

						pos.y += speed * (PLDirOldOld == DIR_DOWN ? 1 : -1);
					}


				}
			}
			else if (cpTypeDRR == LDR_CHIP_BLANK)
			{
				SetAnim("â∫ç~");
				if (pos.y < CHIP_SIZE * 14 - 1)
				{
					pos.y += speed;
				}
			}
			else if (cpTypeURR == LDR_CHIP_BLANK)
			{
				SetAnim("è„è∏");
				if (pos.y >= CHIP_SIZE * 2 - 4)
				{
					pos.y -= speed;
				}
			}
		}

		Dig();
		Dir = DIR_RIGHT;

		// ïsóvÇ»œØÃﬂèÓïÒÇÃÿæØƒ
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_R);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_RM);

		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_L);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_LM);

	}
	
	// ç∂ï˚å¸ÇÃà⁄ìÆä÷åW
	if (PLDir == DIR_LEFT)
	{
		Obj::animCnt++;

		if (lpMapCtl.GetMapData(pos + VECTOR2(-1, (lpMapCtl.GetChipSize() / 2)), LDR_GP_ITEM) == LDR_CHIP_BLANK || 
			lpMapCtl.GetMapData(pos + VECTOR2(-1, (lpMapCtl.GetChipSize() / 2)), LDR_GP_ITEM) == ITEM_CHIP_BLANK)
		{
			if (cpTypeL == EDIT_CHIP_WBL || cpTypeL == EDIT_CHIP_BBL05 || cpTypeL == EDIT_CHIP_BBL05_L || CheckChip(PLDir, cpTypeL))
			{
				SetAnim("â°ñx");
				if (pos.x >= CHIP_SIZE)
				{
					if ((posY_BL == 31))
					{
						pos.x -= speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.y += speed * (PLDirOldOld == DIR_DOWN ? 1 : -1);
					}

					if (CheckChip(PLDir, cpTypeC))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), (cpTypeC >= EDIT_CHIP_BBL75_U && cpTypeC <= EDIT_CHIP_BBL25_U) ? EDIT_CHIP_BBL_BLACK_DM : EDIT_CHIP_BBL_BLACK_D);
					}

					if (cpTypeRL == EDIT_CHIP_WBL || cpTypeL == EDIT_CHIP_BBL05 || cpTypeL == EDIT_CHIP_BBL05_L || CheckChip(PLDir, cpTypeRL))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(1, lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
					}
				}

			}
			else if ((cpTypeL != EDIT_CHIP_BBL && cpTypeUL != EDIT_CHIP_BBL && cpTypeDL != EDIT_CHIP_BBL) && !(cpTypeL >= EDIT_CHIP_BBL75_L && cpTypeL <= EDIT_CHIP_BBL05_L) && !(cpTypeUL >= EDIT_CHIP_BBL75_L && cpTypeUL <= EDIT_CHIP_BBL05_L) && !(cpTypeDL >= EDIT_CHIP_BBL75_L && cpTypeDL <= EDIT_CHIP_BBL05_L))
			{

				SetAnim("ï‡Ç≠");
				if (pos.x >= CHIP_SIZE)
				{
					if ((posY_BL == 31))
					{
						pos.x -= speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.y += speed * (PLDirOldOld == DIR_DOWN ? 1 : -1);
					}
				}
			}
			else if (((cpTypeL == EDIT_CHIP_BBL) || (cpTypeL == EDIT_CHIP_BBL75_L) || (cpTypeL == EDIT_CHIP_BBL50_L) || (cpTypeL == EDIT_CHIP_BBL25_L) || (cpTypeL == EDIT_CHIP_BBL05_L)) && (cpTypeULL != LDR_CHIP_BLANK) && (cpTypeDLL != LDR_CHIP_BLANK))
			{
				SetAnim("â°ñx");
				if (pos.x >= CHIP_SIZE)
				{

					if ((posY_BL == 31))
					{
						if ((Obj::animCnt % 20) == 0)
						{
							GameTask::GetInstance().AddScore(10);
						}
						pos.x -= speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.y += speed * (PLDirOldOld == DIR_DOWN ? 1 : -1);
					}


				}
			}
			else if (cpTypeDLL == LDR_CHIP_BLANK)
			{
				SetAnim("â∫ç~");
				if (pos.y < CHIP_SIZE * 14 - 2)
				{
					Dir = DIR_DOWN;
					Dig();

					pos.y += speed;
				}
			}
			else if (cpTypeULL == LDR_CHIP_BLANK)
			{
				SetAnim("è„è∏");
				if (pos.y >= CHIP_SIZE * 2 - 4)
				{
					Dir = DIR_UP;
					Dig();
					pos.y -= speed;
				}
			}
		}

		Dig();
		Dir = DIR_LEFT;
		// ïsóvÇ»œØÃﬂèÓïÒÇÃÿæØƒ
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_R);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_RM);

		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_L);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_LM);

	}

	// è„ï˚å¸
	if (PLDir == DIR_UP)
	{
		Obj::animCnt++;

		if (lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), -1), LDR_GP_ITEM) == LDR_CHIP_BLANK || 
			lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), -1), LDR_GP_ITEM) == ITEM_CHIP_BLANK)
		{
			if (cpTypeU == EDIT_CHIP_UBL || cpTypeU == EDIT_CHIP_BBL05_U || cpTypeU == EDIT_CHIP_BBL05_D || CheckChip(PLDir, cpTypeU))
			{
				SetAnim("è„å@");
				Dig();

				if (pos.y >= CHIP_SIZE * 2 - 4)
				{
					if ((posX_BL == 31))
					{
						pos.y -= speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.x += speed * (PLDirOldOld == DIR_RIGHT ? 1 : -1);
					}

					if (CheckChip(PLDir, cpTypeC))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), (cpTypeC >= EDIT_CHIP_BBL75_L && cpTypeC <= EDIT_CHIP_BBL25_L) ? EDIT_CHIP_BBL_BLACK_LM : EDIT_CHIP_BBL_BLACK_RM);
					}

					if (cpTypeDU == EDIT_CHIP_UBL || cpTypeU == EDIT_CHIP_BBL05_U || cpTypeU == EDIT_CHIP_BBL05_D || CheckChip(PLDir, cpTypeDU))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, 0), LDR_CHIP_BLANK);
					}
				}
			}
			else if ((cpTypeU != EDIT_CHIP_BBL && cpTypeUL != EDIT_CHIP_BBL && cpTypeUR != EDIT_CHIP_BBL) && !(cpTypeU >= EDIT_CHIP_BBL75_U && cpTypeU <= EDIT_CHIP_BBL05_U) && !(cpTypeUR >= EDIT_CHIP_BBL75_U && cpTypeUR <= EDIT_CHIP_BBL05_U) && !(cpTypeUL >= EDIT_CHIP_BBL75_U && cpTypeUL <= EDIT_CHIP_BBL05_U))
			{

				SetAnim("è„è∏");
				if (pos.y >= CHIP_SIZE * 2)
				{
					if ((posX_BL == 31))
					{
						pos.y -= speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.x += speed * (PLDirOldOld == DIR_RIGHT ? 1 : -1);
					}

					if (cpTypeU == EDIT_CHIP_BBL05_U)
					{
						int x = 0;
					}

				}

			}
			else if (((cpTypeU == EDIT_CHIP_BBL) || (cpTypeU == EDIT_CHIP_BBL75_U) || (cpTypeU == EDIT_CHIP_BBL50_U) || (cpTypeU == EDIT_CHIP_BBL25_U) || (cpTypeU == EDIT_CHIP_BBL05_U)) && (cpTypeULU != LDR_CHIP_BLANK) && (cpTypeURU != LDR_CHIP_BLANK))
			{
				SetAnim("è„å@");
				Dig();

				if (pos.y >= CHIP_SIZE * 2 - 4)
				{

					if ((posX_BL == 31))
					{
						if ((Obj::animCnt % 20) == 0)
						{
							GameTask::GetInstance().AddScore(10);
						}
						pos.y -= speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						pos.x += speed * (PLDirOldOld == DIR_RIGHT ? 1 : -1);
					}

				}

			}
			else if (cpTypeULU == LDR_CHIP_BLANK)
			{
				Dir = DIR_LEFT;
				SetAnim("â°ñx");
				pos.x -= speed;
				Dig();

			}
			else if (cpTypeURU == LDR_CHIP_BLANK)
			{
				Dir = DIR_RIGHT;
				SetAnim("â°ñx");
				pos.x += speed;
				Dig();

			}
		}


		// ïsóvÇ»œØÃﬂèÓïÒÇÃÿæØƒ
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_U);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_UM);

		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_D);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_DM);

	}


	// â∫ï˚å¸
	if (PLDir == DIR_DOWN)
	{
		Obj::animCnt++;

		if (lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), lpMapCtl.GetChipSize() + 1), LDR_GP_ITEM) == LDR_CHIP_BLANK ||
			lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), lpMapCtl.GetChipSize() + 1), LDR_GP_ITEM) == ITEM_CHIP_BLANK)
		{

			if (cpTypeD == EDIT_CHIP_UBL || cpTypeD == EDIT_CHIP_BBL05_U || cpTypeD == EDIT_CHIP_BBL05_D || CheckChip(PLDir, cpTypeD))
			{
				SetAnim("â∫ñx");
				if (pos.y <= CHIP_SIZE * 14 - 2)
				{
					if ((posX_BL == 31))
					{
						pos.y += speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.x += speed * (PLDirOldOld == DIR_RIGHT ? 1 : -1);
					}

					if (CheckChip(PLDir, cpTypeC))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), (cpTypeC >= EDIT_CHIP_BBL75_L && cpTypeC <= EDIT_CHIP_BBL25_L) ? EDIT_CHIP_BBL_BLACK_L : EDIT_CHIP_BBL_BLACK_R);
					}

					if (cpTypeUD == EDIT_CHIP_UBL || cpTypeD == EDIT_CHIP_BBL05_U || cpTypeD == EDIT_CHIP_BBL05_D || CheckChip(PLDir, cpTypeUD))
					{
						lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize()), LDR_CHIP_BLANK);
					}

				}
				Dig();

			}
			else if ((cpTypeD != EDIT_CHIP_BBL && cpTypeDL != EDIT_CHIP_BBL && cpTypeDR != EDIT_CHIP_BBL) && !(cpTypeD >= EDIT_CHIP_BBL75_D && cpTypeD <= EDIT_CHIP_BBL05_D) && !(cpTypeDR >= EDIT_CHIP_BBL75_D && cpTypeDR <= EDIT_CHIP_BBL05_D) && !(cpTypeDL >= EDIT_CHIP_BBL75_D && cpTypeDL <= EDIT_CHIP_BBL05_D))
			{

				SetAnim("â∫ç~");
				if (pos.y <= CHIP_SIZE * 14 - 2)
				{
					if ((posX_BL == 31))
					{
						pos.y += speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						Dig();
						pos.x += speed * (PLDirOldOld == DIR_RIGHT ? 1 : -1);
					}
				}
			}
			else if (((cpTypeD == EDIT_CHIP_BBL) || (cpTypeD == EDIT_CHIP_BBL75_D) || (cpTypeD == EDIT_CHIP_BBL50_D) || (cpTypeD == EDIT_CHIP_BBL25_D) || (cpTypeD == EDIT_CHIP_BBL05_D)) && (cpTypeDLD != LDR_CHIP_BLANK) && (cpTypeDRD != LDR_CHIP_BLANK))
			{
				SetAnim("â∫ñx");
				if (pos.y <= CHIP_SIZE * 14 - 2)
				{

					if ((posX_BL == 31))
					{
						if ((Obj::animCnt % 20) == 0)
						{
							GameTask::GetInstance().AddScore(10);
						}
						pos.y += speed;
					}
					else
					{
						PLDirOld = PLDirOldOld;
						AnimDirChange();
						pos.x += speed * (PLDirOldOld == DIR_RIGHT ? 1 : -1);
					}

				}
				Dig();

			}
			else if (cpTypeDLD == LDR_CHIP_BLANK)
			{
				if (pos.y <= CHIP_SIZE * 14 - 2)
				{
					Dir = DIR_LEFT;
					SetAnim("â°ñx");
					pos.x -= speed;
				}
				Dig();

			}
			else if (cpTypeDRD == LDR_CHIP_BLANK)
			{
				if (pos.y <= CHIP_SIZE * 14 - 2)
				{
					Dir = DIR_RIGHT;
					SetAnim("â°ñx");
					pos.x += speed;
				}
				Dig();

			}
		}

		// ïsóvÇ»œØÃﬂèÓïÒÇÃÿæØƒ
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_D);
		changeBlack(VECTOR2((lpMapCtl.GetChipSize() / 2), (lpMapCtl.GetChipSize() / 2)), LDR_GP_SD, EDIT_CHIP_BBL_BLACK_U);

	}
	if (lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), -1), LDR_GP_ITEM) == ITEM_CHIP_STONE)
	{
		GameTask::GetInstance().SetCheckStone(pos / CHIP_SIZE);
	}

}

void Player::StartAnim(void)
{
	VECTOR2 tmpPos(pos / CHIP_SIZE);
	if (tmpPos.y == 7 && PLDir != DIR_DOWN)
	{
		PLDir = DIR_NON;
		GameTask::GetInstance().SetStartFlag(true);
	}

	if (tmpPos.y == 1)
	{
		if (tmpPos.x != 5)
		{
			PLDir = DIR_LEFT;
		}
		else
		{
			PLDir = DIR_DOWN;
		}
	}

	if (tmpPos.y == 8 && PLDir != DIR_NON)
	{
		PLDir = DIR_LEFT;
	}

	PLDirOld = PLDir;
	Dig();
}

void Player::CheckKey(void)
{
	if (PLDir == DIR_NON)
	{
		if (PLDirOld != DIR_NON)
		{
			if (PLDirOldOld != PLDirOld)
			{
				PLDirOldOld = PLDirOld;
			}
		}
	}
	if (PLDir != DIR_NON)
	{
		if (PLDirOld != DIR_NON)
		{
			if (PLDirOldOld != PLDirOld)
			{
				PLDirOldOld = PLDirOld;
			}
		}
		if (PLDirOld != PLDir)
		{
			PLDirOld = PLDir;
		}
	}
	PLDir = (keyData[KEY_INPUT_RIGHT] ? DIR_RIGHT : (keyData[KEY_INPUT_LEFT] ? DIR_LEFT : (keyData[KEY_INPUT_UP] ? DIR_UP : (keyData[KEY_INPUT_DOWN] ? DIR_DOWN : DIR_NON))));
}

bool Player::CheckChip(DIR dir, CHIP_TYPE type)
{
	if (dir == DIR_RIGHT)
	{
		if ((type >= EDIT_CHIP_BBL75_L && type <= EDIT_CHIP_BBL25_L) || (type >= EDIT_CHIP_BBL75_U && type <= EDIT_CHIP_BBL25_U) || (type >= EDIT_CHIP_BBL75_D && type <= EDIT_CHIP_BBL25_D))
		{
			return true;
		}
	}
	if (dir == DIR_LEFT)
	{
		if ((type >= EDIT_CHIP_BBL75 && type <= EDIT_CHIP_BBL25) || (type >= EDIT_CHIP_BBL75_U && type <= EDIT_CHIP_BBL25_U) || (type >= EDIT_CHIP_BBL75_D && type <= EDIT_CHIP_BBL25_D))
		{
			return true;
		}
	}
	if (dir == DIR_UP)
	{
		if ((type >= EDIT_CHIP_BBL75_D && type <= EDIT_CHIP_BBL25_D) || (type >= EDIT_CHIP_BBL75 && type <= EDIT_CHIP_BBL25) || (type >= EDIT_CHIP_BBL75_L && type <= EDIT_CHIP_BBL25_L))
		{
			return true;
		}
	}

	if (dir == DIR_DOWN)
	{
		if ((type >= EDIT_CHIP_BBL75_U && type <= EDIT_CHIP_BBL25_U) || (type >= EDIT_CHIP_BBL75 && type <= EDIT_CHIP_BBL25) || (type >= EDIT_CHIP_BBL75_L && type <= EDIT_CHIP_BBL25_L))
		{
			return true;
		}
	}
	return false;
}

bool Player::CheckChip(DIR dir, DIR dir2, CHIP_TYPE type, CHIP_TYPE range1, CHIP_TYPE range2)
{
	if (dir == dir2)
	{
		if ((type >= range1 && type <= range2))
		{
			return true;
		}
	}
	return false;
}

bool Player::InitAnim(void)
{
	// à⁄ìÆä÷òA
	AddAnim("óßÇø", 2, 0, 1, 4);
	AddAnim("ï‡Ç≠", 1, 0, 2, 4);
	AddAnim("â∫ç~", 1, 1, 2, 4);
	AddAnim("è„è∏", 1, 2, 2, 4);

	// å@ÇËä÷òA
	AddAnim("â°ñx", 3, 0, 2, 4);
	AddAnim("è„å@", 3, 2, 2, 4);
	AddAnim("â∫ñx", 3, 1, 2, 4);

	// Ç‚ÇÁÇÍ±∆“
	AddAnim("Ç‚ÇÁÇÍ", 1, 3, 6, 6);

	// çUåÇ±∆“
	AddAnim("É|ÉìÉvâ°", 5, 0, 3, 4);
	AddAnim("É|ÉìÉvâ∫", 5, 1, 2, 4);
	AddAnim("É|ÉìÉvè„", 5, 2, 2, 4);
	return true;
}

void Player::AnimDirChange(void)
{
	switch (PLDirOld)
	{
	case DIR_RIGHT:
		if(GetAnim() != "â°ñx")
		SetAnim("â°ñx");
		break;
	case DIR_LEFT:
		if (GetAnim() != "â°ñx")
		SetAnim("â°ñx");

		break;
	case DIR_UP:
		if (GetAnim() != "è„ñx")
		SetAnim("è„å@");
		break;
	case DIR_DOWN:
		if (GetAnim() != "â∫ñx")
		SetAnim("â∫ñx");
		break;
	default:
		break;
	}

	//Obj::animCnt--;
}

void Player::Dig(void)
{

	// å@ÇÈèàóù
	switch (PLDirOld)
	{
	case DIR_RIGHT:
		// ñxåáÇØÇÕè¡Ç∑
		if (DigCheck(cpTypeLR))
		{
			lpMapCtl.SetMapData(pos + VECTOR2(5, (lpMapCtl.GetChipSize() / 2)), LDR_CHIP_BLANK);
		}

		if (cpTypeC == EDIT_CHIP_WBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL05);
		}
		if (cpTypeC == EDIT_CHIP_BBL05 || cpTypeC == EDIT_CHIP_BBL05_L)
		{
			if (lpMapCtl.GetMapDig(pos))
			{
				lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
			}
		}

		if (posX_BL <= 8 && (cpTypeR == EDIT_CHIP_BBL))		// 75%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() + 1, lpMapCtl.GetChipSize() -7), EDIT_CHIP_BBL75);
		}
		if ((posX_BL > 8 && posX_BL <= 16) && cpTypeR == EDIT_CHIP_BBL75)	// 50%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize(), lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL50);
		}
		if ((posX_BL > 16 && posX_BL <= 28) && cpTypeR == EDIT_CHIP_BBL50)	// 25%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize(), lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL25);
		}
		if ((posX_BL > 28 && posX_BL <= 31) && cpTypeR == EDIT_CHIP_BBL25)
		{
			//lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize(), lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL05);
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize(), lpMapCtl.GetChipSize() / 2), EDIT_CHIP_UBL);
		}

		if (cpTypeLR == EDIT_CHIP_BBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize(), lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
		}

		break;
	case DIR_LEFT:
		// ñxåáÇØÇÕè¡Ç∑
		if (DigCheck(cpTypeRL))
		{
			lpMapCtl.SetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() - 5), (lpMapCtl.GetChipSize() / 2)), LDR_CHIP_BLANK);
		}

		if (cpTypeC == EDIT_CHIP_WBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL05_L);
		}
		if (cpTypeC == EDIT_CHIP_BBL05 || cpTypeC == EDIT_CHIP_BBL05_L)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
		}

		if (posX_BL >= 23 && cpTypeL == EDIT_CHIP_BBL)		// 75%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(-1, lpMapCtl.GetChipSize() -7), EDIT_CHIP_BBL75_L);
		}
		if ((posX_BL > 15 && posX_BL <= 23) && cpTypeL == EDIT_CHIP_BBL75_L)	// 50%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(0, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL50_L);
		}
		if ((posX_BL > 3 && posX_BL <= 15) && cpTypeL == EDIT_CHIP_BBL50_L)	// 25%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(0, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL25_L);
		}
		if ((posX_BL > 0 && posX_BL <= 3) && cpTypeL == EDIT_CHIP_BBL25_L)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(0, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_UBL);
		}

		if (cpTypeRL == EDIT_CHIP_BBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(1, lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
		}

		break;
	case DIR_UP:
		// ñxåáÇØÇÕè¡Ç∑
		if (DigCheck(cpTypeDU))
		{
			lpMapCtl.SetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), lpMapCtl.GetChipSize() - 5), LDR_CHIP_BLANK);
		}

		if (cpTypeC == EDIT_CHIP_UBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL05_U);
		}
		if (cpTypeC == EDIT_CHIP_BBL05_U || cpTypeC == EDIT_CHIP_BBL05_D)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
		}

		if (posY_BL >= 20 && cpTypeU == EDIT_CHIP_BBL)		// 75%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() - 7, -1), EDIT_CHIP_BBL75_U);
		}
		if ((posY_BL > 10 && posY_BL <= 20) && cpTypeU == EDIT_CHIP_BBL75_U)	// 50%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, 0), EDIT_CHIP_BBL50_U);
		}
		if ((posY_BL > 3 && posY_BL <= 10) && cpTypeU == EDIT_CHIP_BBL50_U)	// 25%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, 0), EDIT_CHIP_BBL25_U);
		}
		if ((posY_BL > 0 && posY_BL <= 3) && cpTypeU == EDIT_CHIP_BBL25_U)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, 0), EDIT_CHIP_WBL);
		}

		break;
	case DIR_DOWN:
		// ñxåáÇØÇÕè¡Ç∑
		if (DigCheck(cpTypeUD))
		{
			lpMapCtl.SetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), 5), LDR_CHIP_BLANK);
		}

		if (cpTypeC == EDIT_CHIP_UBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), EDIT_CHIP_BBL05_D);
		}
		if (cpTypeC == EDIT_CHIP_BBL05_U || cpTypeC == EDIT_CHIP_BBL05_D)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize() / 2), LDR_CHIP_BLANK);
		}

		if (posY_BL <= 8 && cpTypeD == EDIT_CHIP_BBL)		// 75%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() - 7, lpMapCtl.GetChipSize() + 1), EDIT_CHIP_BBL75_D);
		}
		if ((posY_BL > 8 && posY_BL <= 16) && cpTypeD == EDIT_CHIP_BBL75_D)	// 50%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize()), EDIT_CHIP_BBL50_D);
		}
		if ((posY_BL > 16 && posY_BL <= 28) && cpTypeD == EDIT_CHIP_BBL50_D)	// 25%
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize()), EDIT_CHIP_BBL25_D);
		}
		if ((posY_BL > 28 && posY_BL <= 31) && cpTypeD == EDIT_CHIP_BBL25_D)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize()), EDIT_CHIP_WBL);
		}
		if (cpTypeUD == EDIT_CHIP_BBL)
		{
			lpMapCtl.SetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() / 2, lpMapCtl.GetChipSize()), LDR_CHIP_BLANK);
		}
		break;
	default:
		break;
	}

}

bool Player::DigCheck(CHIP_TYPE type)
{
	if (PLDirOld == DIR_UP || PLDirOld == DIR_DOWN)
	{
		// âEë§ÇÃñxåáÇØ
		if ((type == EDIT_CHIP_BBL75) || (type == EDIT_CHIP_BBL50) || (type == EDIT_CHIP_BBL25) || (type == EDIT_CHIP_BBL05))
		{
			return true;
		}

		// ç∂ë§ÇÃñxåáÇØ
		if ((type == EDIT_CHIP_BBL75_L) || (type == EDIT_CHIP_BBL50_L) || (type == EDIT_CHIP_BBL25_L) || (type == EDIT_CHIP_BBL05_L))
		{
			return true;
		}
	}

	if (PLDirOld == DIR_RIGHT || PLDirOld == DIR_LEFT)
	{
		// è„ë§ÇÃñxåáÇØ
		if ((type == EDIT_CHIP_BBL75_U) || (type == EDIT_CHIP_BBL50_U) || (type == EDIT_CHIP_BBL25_U) || (type == EDIT_CHIP_BBL05_U))
		{
			return true;
		}

		// â∫ë§ÇÃñxåáÇØ
		if ((type == EDIT_CHIP_BBL75_D) || (type == EDIT_CHIP_BBL50_D) || (type == EDIT_CHIP_BBL25_D) || (type == EDIT_CHIP_BBL05_D))
		{
			return true;
		}
	}
	return false;
}

void Player::SetAttack(void)
{
	VECTOR2 AtacckPos((PLDirOld == DIR_RIGHT ? CHIP_SIZE : (PLDirOld == DIR_LEFT ? 0 : CHIP_SIZE / 2)), (PLDirOld == DIR_DOWN ? CHIP_SIZE : (PLDirOld == DIR_UP ? 0 : CHIP_SIZE / 2)));
	VECTOR2 tmpPos((pos + AtacckPos + IKARICnt) / CHIP_SIZE);
	VECTOR2 IKARIPos(((pos + AtacckPos + IKARICnt) / CHIP_SIZE) - (pos / CHIP_SIZE));
	if (GetAttackPos() == VECTOR2(999, 999))
	{
		IKARIflag = false;
	}


	if (keyData[KEY_INPUT_Z] && !keyDataOld[KEY_INPUT_Z])
	{
		if (PLDirOld == DIR_RIGHT || PLDirOld == DIR_LEFT)
		{
			SetAnim("É|ÉìÉvâ°");
		}
		else if (PLDirOld == DIR_UP)
		{
			SetAnim("É|ÉìÉvè„");
		}
		else if (PLDirOld == DIR_DOWN)
		{
			SetAnim("É|ÉìÉvâ∫");
		}
		IKARIflag = true;
		SetAttackPos(pos / CHIP_SIZE);
	}
	if ((tmpPos.x > 0 && tmpPos.x < GAME_AREA_CNT_X) && (tmpPos.y > 0 && tmpPos.y < GAME_AREA_CNT_Y))
	{
		if (IKARIflag)
		{
			animCnt++;
			if (animCnt < 12)
			{
				Obj::animCnt++;
			}
			else
			{
				Obj::animCnt = 0;
				if (PLDirOld == DIR_RIGHT || PLDirOld == DIR_LEFT)
				{
					DrawExtendGraph(pos.x + (PLDirOld == DIR_LEFT ? IKARICnt.x : 0), pos.y, pos.x + CHIP_SIZE + (PLDirOld == DIR_RIGHT ? IKARICnt.x : 0), pos.y + CHIP_SIZE, (PLDirOld == DIR_RIGHT ? IMAGE_ID("image/ÉèÉCÉÑÅ[âE.png") : IMAGE_ID("image/ÉèÉCÉÑÅ[ç∂.png"))[0], true);
					IKARICnt.x += (PLDirOld == DIR_LEFT ? -4 : 4);

				}
				else if(PLDirOld == DIR_UP || PLDirOld == DIR_DOWN)
				{
					DrawExtendGraph(pos.x, pos.y + (PLDirOld == DIR_UP ? IKARICnt.y : 0), pos.x + CHIP_SIZE, pos.y + CHIP_SIZE + (PLDirOld == DIR_DOWN ? IKARICnt.y : 0), (PLDirOld == DIR_DOWN ? IMAGE_ID("image/ÉèÉCÉÑÅ[â∫.png") : IMAGE_ID("image/ÉèÉCÉÑÅ[è„.png"))[0], true);
					IKARICnt.y += (PLDirOld == DIR_UP ? -4 : 4);
				}
				SetAttackPos(tmpPos + VECTOR2((PLDirOld == DIR_RIGHT ? -1 : (PLDirOld == DIR_LEFT ? 0 : -1)),(PLDirOld == DIR_DOWN || PLDirOld == DIR_UP ? (PLDirOld == DIR_UP ? -1 : 0) : -1)));

			}


			if (lpMapCtl.GetMapData(pos + AtacckPos + IKARICnt, LDR_GP_BG) == EDIT_CHIP_BBL)
			{
				IKARIflag = false;
			}

			if ((PLDirOld == DIR_RIGHT && IKARIPos.x > 3) || (PLDirOld == DIR_LEFT && IKARIPos.x < -3))
			{
				IKARIflag = false;
			}
			else if ((PLDirOld == DIR_DOWN && IKARIPos.y > 3) || (PLDirOld == DIR_UP && IKARIPos.y < -3))
			{
				IKARIflag = false;
			}
		}
		else
		{
			SetAttackPos(VECTOR2(0,0));
			animCnt = 0;
			IKARICnt = { 0,0 };
		}
	}
	else
	{
		IKARIflag = false;
		animCnt = 0;
		IKARICnt = { 0,0 };

	}
}


