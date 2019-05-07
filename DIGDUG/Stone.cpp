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

void Stone::Draw(void)
{
	Obj::Draw(Angle);
}


void Stone::SetMove(void)
{
	VECTOR2 tmpPos(pos.x / CHIP_SIZE, (pos.y + CHIP_SIZE - 1) / CHIP_SIZE);

	if (checkPlayer)
	{
		DownTime++;

		if (DownTime > 180 && cpTypeD != EDIT_CHIP_BBL)
		{
			SetAnim("ê√é~");
			Angle = 0;
			pos.y += speed;
			if (DownTime > 210)
			{
				lpMapCtl.SetMapData(pos + VECTOR2(CHIP_SIZE / 2, (CHIP_SIZE / 2) + (CHIP_SIZE / 3)), LDR_CHIP_BLANK);
			}
			else
			{
				lpMapCtl.SetMapData(pos + VECTOR2(CHIP_SIZE / 2, (CHIP_SIZE / 2)), ITEM_CHIP_BLANK);
			}
		}
		else
		{
			if (DownTime < 180)
			{
				if ((DownTime % 32) >= 16)
				{
					Angle = -0.25;
				}
				else
				{
					Angle = 0;
				}
			}
			else
			{
				Angle = 0;
				if (DownTime > 210)
				{
					DownTime = 0;
					checkPlayer = false;
				}
			}
			//Obj::animCnt++;
		}
	}
	else
	{
		SetAnim("ê√é~");
		Angle = 0;
	}
	if (tmpPos == GameTask::GetInstance().GetCheckStone())
	{
		SetAnim("óéâ∫");
		// â∫Ç…Ãﬂ⁄≤‘∞Ç™óàÇΩèÍçá
		checkPlayer = true;

	}
}

bool Stone::InitAnim(void)
{
	AddAnim("ê√é~", 1, 0, 1, 4);
	AddAnim("óéâ∫", 0, 0, 2, 8);
	return false;
}
