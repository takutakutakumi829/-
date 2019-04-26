#include <math.h>
#include "DxLib.h"
#include "Stone.h"
#include "GameTask.h"
#include "MapCtl.h"

Stone::Stone(VECTOR2 chipOffSet):Obj(KeyData,KeyDataOld, chipOffSet)
{
}


Stone::~Stone()
{
}

bool Stone::Update(void)
{
	SetMapCheck();
	SetMove();
	return false;
}


void Stone::SetMove(void)
{
	VECTOR2 tmpPos(pos.x / CHIP_SIZE, (pos.y + CHIP_SIZE - 1) / CHIP_SIZE);


	if (checkPlayer)
	{
		DownTime++;

		if (DownTime > 180 && cpTypeD != EDIT_CHIP_BBL)
		{
			SetAnim("Ã~");
			pos.y += speed;
		}
		else
		{
			Obj::animCnt++;
		}
	}
	else
	{
		SetAnim("Ã~");
	}
	if (tmpPos == lpGameTask.GetCheckStone())
	{
		SetAnim("—‰º");
		// ‰º‚ÉÌßÚ²Ô°‚ª—ˆ‚½ê‡
		checkPlayer = true;

	}
}

bool Stone::InitAnim(void)
{
	AddAnim("Ã~", 1, 0, 1, 4);
	AddAnim("—‰º", 0, 0, 2, 8);
	return false;
}
