#include "Obj.h"
#include "Dxlib.h"
#include "MapCtl.h"
#include "ImageMng.h"


/*Obj::Obj()
{
}*/

Obj::Obj(const char (&_keyData)[256], const char (&_keyDataOld)[256],  VECTOR2 chipOffset):keyData(_keyData),keyDataOld(_keyDataOld)
{
	pos = VECTOR2(32, 32);
	animCnt = 0;
	//this->chipOffset = chipOffset;
}


Obj::~Obj()
{
	//delete imageID;
}

void Obj::init(std::string filname, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed)
{
	ImageMng::GetInstance()->GetID(filname, divSize, divCnt, chipOffset);
	imageName = filname;
	this->divSize = divSize;
	this->divCnt = divCnt;
	this->chipOffset = chipOffset;
	this->speed = speed;
	InitAnim();
}

bool Obj::Update(void)
{
	return false;
}

void Obj::Draw(void)
{
	if (animName.length() == 0)
	{
		return;
	}
	int id = chipOffset.x + chipOffset.y*divCnt.x;

	if (animTable.find(animName) != animTable.end())	//存在をチェックする
	{
		id = animTable[animName][ANIM_TBL_START_ID] + (animCnt / animTable[animName][ANIM_TBL_INV]) % animTable[animName][ANIM_TBL_FRAME];	//アニメーションゆっくりさせる為にインターバルで割る
	}

	if (imageName.length() == 0)
	{
		return;
	}
	if (Dir == DIR_LEFT)
	{
		Dir = DIR_LEFT;

		DrawExtendGraph(chipOffset.x + pos.x + CHIP_SIZE, chipOffset.x + pos.y, chipOffset.x + pos.x, chipOffset.x + pos.y + CHIP_SIZE, IMAGE_ID(imageName)[id], true);
	}
	else
	{
		DrawExtendGraph(chipOffset.x + pos.x, chipOffset.x + pos.y, chipOffset.x + pos.x + CHIP_SIZE, chipOffset.x + pos.y + CHIP_SIZE, IMAGE_ID(imageName)[id], true);
	}
}

bool Obj::Beaten(int count)
{
	return false;
}

const bool & Obj::GetBeatenFlag(void)
{
	return BeatenFlag;
}

void Obj::SetBeatenFlag(const bool & BeatenFlag)
{
	this->BeatenFlag = BeatenFlag;
}

const int & Obj::GetEnemyCnt(void)
{
	return EnemyCount;
}

void Obj::AddEnemyCnt(int Cnt)
{
	EnemyCount += Cnt;
}

const bool & Obj::GetBeatenCnt(void)
{
	return BeatenCnt;
}


void Obj::SetAttackPos(VECTOR2 pos)
{
	attackPos = pos;
}

const VECTOR2 & Obj::GetAttackPos(void)
{
	return attackPos;
}

void Obj::Draw(int id)
{

	if (imageName.length() == 0 || IMAGE_ID(imageName).size() <= id)
	{
		return;
	}
	//DrawGraph(pos.x, pos.y, IMAGE_ID(imageName)[id], true);
	DrawExtendGraph(chipOffset.x + pos.x, chipOffset.x + pos.y, chipOffset.x + pos.x + CHIP_SIZE, chipOffset.x + pos.y + CHIP_SIZE, IMAGE_ID(imageName)[id], true);

}

void Obj::Draw(double angle)
{
	int id = chipOffset.x + chipOffset.y*divCnt.x;

	if (imageName.length() == 0 || IMAGE_ID(imageName).size() <= id)
	{
		return;
	}
	DrawRotaGraph(pos.x + CHIP_SIZE / 2, pos.y + CHIP_SIZE / 2, 2, angle, IMAGE_ID(imageName)[id], true);

}

const VECTOR2 & Obj::GetSize(void)
{
	return this->divSize;
}

void Obj::SetSize(const VECTOR2 & divSize)
{
	this->divSize = divSize;
}

bool Obj::SetPos(VECTOR2 pos)
{
	this->pos = pos;
	return true;
}

const VECTOR2 & Obj::GetPos(void)
{
	return this->pos;
}

bool Obj::SetAnim(std::string animName)
{
	if (animTable.find(animName) == animTable.end())
	{
		return false;
	}
	if (Obj::animName != animName)
	{
		animCnt = 0;
		Obj::animName = animName;
	}
	return true;
}

std::string Obj::GetAnim(void)
{
	return Obj::animName;
}

bool Obj::InitAnim(void)
{
	return true;
}

void Obj::SetMove(void)
{

}

void Obj::SetMapCheck(void)
{
	cpTypeC = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2) - 1, (lpMapCtl.GetChipSize() / 2) - 1), LDR_GP_BG);

	// 上下左右
	cpTypeD = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), lpMapCtl.GetChipSize() + 1), LDR_GP_BG);
	cpTypeU = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), -1), LDR_GP_BG);
	cpTypeL = lpMapCtl.GetMapData(pos + VECTOR2(-1, (lpMapCtl.GetChipSize() / 2)), LDR_GP_BG);
	cpTypeR = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() + 1), (lpMapCtl.GetChipSize() / 2)), LDR_GP_BG);

	// 上下左右のチェック
	cpTypeDU = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), lpMapCtl.GetChipSize() - 5), LDR_GP_BG);
	cpTypeUD = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() / 2), 5), LDR_GP_BG);
	cpTypeLR = lpMapCtl.GetMapData(pos + VECTOR2(5, (lpMapCtl.GetChipSize() / 2)), LDR_GP_BG);
	cpTypeRL = lpMapCtl.GetMapData(pos + VECTOR2((lpMapCtl.GetChipSize() - 5), (lpMapCtl.GetChipSize() / 2)), LDR_GP_BG);

	// 左上　右上
	cpTypeUL = lpMapCtl.GetMapData(pos + VECTOR2(1, 1), LDR_GP_BG);
	cpTypeUR = lpMapCtl.GetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() - 1, 1), LDR_GP_BG);

	// 左下　右下
	cpTypeDL = lpMapCtl.GetMapData(pos + VECTOR2(1, lpMapCtl.GetChipSize() - 1), LDR_GP_BG);
	cpTypeDR = lpMapCtl.GetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() - 1, lpMapCtl.GetChipSize() - 1), LDR_GP_BG);

	// 右側の斜め上下
	cpTypeURR = lpMapCtl.GetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() + 1, 1), LDR_GP_BG);
	cpTypeDRR = lpMapCtl.GetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() + 1, lpMapCtl.GetChipSize() - 1), LDR_GP_BG);

	// 左側の斜め上下
	cpTypeULL = lpMapCtl.GetMapData(pos + VECTOR2(-1, 1), LDR_GP_BG);
	cpTypeDLL = lpMapCtl.GetMapData(pos + VECTOR2(-1, lpMapCtl.GetChipSize() - 1), LDR_GP_BG);

	// 上側の斜め左右
	cpTypeULU = lpMapCtl.GetMapData(pos + VECTOR2(1, -1), LDR_GP_BG);
	cpTypeURU = lpMapCtl.GetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() - 1, -1), LDR_GP_BG);

	// 下側の斜め左右
	cpTypeDLD = lpMapCtl.GetMapData(pos + VECTOR2(1, lpMapCtl.GetChipSize() + 1), LDR_GP_BG);
	cpTypeDRD = lpMapCtl.GetMapData(pos + VECTOR2(lpMapCtl.GetChipSize() - 1, lpMapCtl.GetChipSize() + 1), LDR_GP_BG);
}

void Obj::LifeDraw(int life)
{
	DrawExtendGraph(CHIP_SIZE * GAME_AREA_CNT_X + (CHIP_SIZE * life), CHIP_SIZE * 8, CHIP_SIZE * GAME_AREA_CNT_X + CHIP_SIZE + (CHIP_SIZE * life), CHIP_SIZE * 8 + CHIP_SIZE, IMAGE_ID("image/Life.png")[0], true);
}

bool Obj::AddAnim(std::string animName, int id_x, int id_y, int Frame, int inv)
{
	animTable[animName][ANIM_TBL_START_ID] = id_x + id_y * divCnt.x;
	animTable[animName][ANIM_TBL_FRAME] = Frame;
	animTable[animName][ANIM_TBL_INV] = inv;
	return true;
}

