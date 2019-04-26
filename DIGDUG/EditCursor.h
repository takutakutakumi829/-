#pragma once
#include "Obj.h"
#include "GameTask.h"
#include "MapCtl.h"



constexpr unsigned int EDIT_KEY_GET_RNG = (30);

class GameTask;

class EditCursor :
	public Obj
{
public:
	//EditCursor();
	EditCursor(const char (&_keyData)[256], const char (&_keyDataOld)[256], VECTOR2 chipOffSet);
	~EditCursor();
	bool Update(void);
	void Draw(void);

	VECTOR2 mapCnt = { 12,14 };

private:
	bool SetChipID(void);
	void SetMove(void);
	CHIP_TYPE nowID;
	int FlamTime;
	unsigned int keyGetRng;				// Ÿ‚Ì·°ˆ—‚Ü‚Å‚ÌŠÔŠu
	unsigned int inputFlam;				// ‘O‰ñ‚ÌˆÚ“®‚©‚ç‚ÌˆÚ“®ÌÚ°Ñ

};

