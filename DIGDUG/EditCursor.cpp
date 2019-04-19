#include "DxLib.h"
#include "GameTask.h"
#include "EditCursor.h"



/*EditCursor::EditCursor()
{
}*/

EditCursor::EditCursor(const char (&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffSet) :Obj(_keyData,_keyDataOld,chipOffSet)
{
	nowID = EDIT_CHIP_BBL;
	pos = VECTOR2(32, 32);
	FlamTime = 0;
	keyGetRng = EDIT_KEY_GET_RNG;
	inputFlam = 0;

}


EditCursor::~EditCursor()
{
}

bool EditCursor::Update(void)
{
	SetMove();
	SetChipID();
	return false;
}

void EditCursor::Draw(void)
{
	if ((FlamTime / 30) % 10 < 5)
	{
		Obj::Draw(nowID);
	}	
	else
	{
		Obj::Draw(LDR_CHIP_EDIT);
	}

	FlamTime += 10;

}

bool EditCursor::SetChipID(void)
{
	if ((keyData[KEY_INPUT_SPACE]))// && !keyDataOld[KEY_INPUT_SPACE]))
	{
		SET_MAP_ID(pos, nowID);
	}
	return true;
}

void EditCursor::SetMove(void)
{
	VECTOR2 tmpPos = pos;

	if (keyData[KEY_INPUT_RIGHT])
	{
		if(pos.x < ((int)CHIP_SIZE * mapCnt.x))
		tmpPos.x += CHIP_SIZE;
	}
	if (keyData[KEY_INPUT_LEFT])
	{
		if(pos.x > CHIP_SIZE)
		tmpPos.x -= CHIP_SIZE;
	}
	if (keyData[KEY_INPUT_UP])
	{
		if(pos.y > CHIP_SIZE)
		tmpPos.y -= CHIP_SIZE;
	}
	if (keyData[KEY_INPUT_DOWN])
	{
		if(pos.y < ((int)CHIP_SIZE * mapCnt.y))
		tmpPos.y += CHIP_SIZE;
	}
	if (tmpPos != pos)
	{
		if (inputFlam >= keyGetRng)
		{
			pos = tmpPos;
			if ((keyGetRng / 2) < 5)
			{
				keyGetRng = 5;
			}
			else 
			{
				keyGetRng = keyGetRng / 2;
			}
			inputFlam = 0;
		}
		else
		{
			inputFlam++;
		}
	}
	else
	{
		inputFlam = EDIT_KEY_GET_RNG;
		keyGetRng = EDIT_KEY_GET_RNG;

		if (keyData[KEY_INPUT_Z] && !keyDataOld[KEY_INPUT_Z])
		{
			if (nowID <= EDIT_CHIP_BBL05)
			{
				nowID = LAST_EDIT_CHIP;				// ¶Ù°Ìß
			}
			else if (nowID <= EDIT_CHIP_PL && nowID >= EDIT_CHIP_BBL_OUT)
			{
				nowID = EDIT_CHIP_BBL_OUT;
			}
			else // (nowID < LDR_CHIP_EDIT && nowID > LDR_CHIP_BLANK)
			{
				nowID = (CHIP_TYPE)(nowID - 1);

			}
		}
		if (keyData[KEY_INPUT_X] && !keyDataOld[KEY_INPUT_X])
		{
			if (nowID >= LAST_EDIT_CHIP)
			{
				nowID = START_EDIT_CHIP;				// ‰EÙ°Ìß
			}
			else if (nowID > EDIT_CHIP_BBL_OUT && nowID < EDIT_CHIP_PL)
			{
				nowID = EDIT_CHIP_PL;
			}
			else //(nowID > LDR_CHIP_BLANK && nowID < LDR_CHIP_EDIT)
			{
				nowID = (CHIP_TYPE)(nowID + 1);
			}
		}

	}

}
