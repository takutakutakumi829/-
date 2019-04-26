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
			SetAnim("�Î~");
			pos.y += speed;
		}
		else
		{
			Obj::animCnt++;
		}
	}
	else
	{
		SetAnim("�Î~");
	}
	if (tmpPos == lpGameTask.GetCheckStone())
	{
		SetAnim("����");
		// ������ڲ԰�������ꍇ
		checkPlayer = true;

	}
}

bool Stone::InitAnim(void)
{
	AddAnim("�Î~", 1, 0, 1, 4);
	AddAnim("����", 0, 0, 2, 8);
	return false;
}
