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
			SetAnim("Γ~");
			pos.y += speed;
		}
		else
		{
			Obj::animCnt++;
		}
	}
	else
	{
		SetAnim("Γ~");
	}
	if (tmpPos == lpGameTask.GetCheckStone())
	{
		SetAnim("Ί");
		// ΊΙΜίΪ²Τ°ͺ½κ
		checkPlayer = true;

	}
}

bool Stone::InitAnim(void)
{
	AddAnim("Γ~", 1, 0, 1, 4);
	AddAnim("Ί", 0, 0, 2, 8);
	return false;
}
