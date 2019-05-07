//#include "GameTask.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "EditCursor.h"
#include "Dxlib.h"	// DxLib×²ÌÞ×Ø‚ðŽg—p‚·‚é
#include "GameTask.h"

MapCtl *MapCtl::s_Instance = nullptr;


void MapCtl::Create()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new MapCtl();
	}
}

void MapCtl::Destroy()
{
	if (s_Instance)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;
}

bool MapCtl::SetMapData(VECTOR2 pos, CHIP_TYPE id)
{
	VECTOR2 tmpPos(pos / CHIP_SIZE);
	if ((tmpPos.y < (int)mapID.size()) && (tmpPos.x < (int)mapID[tmpPos.y].size()))
	{
		mapID[tmpPos.y][tmpPos.x] = (mapID[tmpPos.y][tmpPos.x] & (~(0x000000ff << chipGrp[id] * DATA_BIT_LEN))) | (id << (chipGrp[id] * DATA_BIT_LEN));
		return true;
	}
	return false;
}

CHIP_TYPE MapCtl::GetMapData(VECTOR2 pos, CHIP_GRP grpID)
{
	VECTOR2 tmpPos(pos / CHIP_SIZE);
	if ((tmpPos.y < (int)mapID.size()) && (tmpPos.x < (int)mapID[tmpPos.y].size()))
	{
		return (CHIP_TYPE)((mapID[tmpPos.y][tmpPos.x] >> (grpID * DATA_BIT_LEN)) & 0xff);
	}
	return LDR_CHIP_BLANK;
}

CHIP_TYPE MapCtl::GetMapData(int x, int y, CHIP_GRP grpID)
{
	if ((y < (int)mapID.size()) && (x < (int)mapID[y].size()))
	{
		return (CHIP_TYPE)((mapID[y][x] >> (grpID * DATA_BIT_LEN)) & 0xff);
	}
	return LDR_CHIP_BLANK;
}

bool MapCtl::SetMapDig(int x,int y)
{
	if ((y < (int)mapID.size()) && (x < (int)mapID[y].size()))
	{
		if (x == 1 && (y > BlockStart && y < 14))
		{
			SetMapData(VECTOR2(x*CHIP_SIZE, y*CHIP_SIZE), EDIT_CHIP_BGD_L);
		}
		else if(x == 12 && (y > BlockStart && y < 14))
		{
			SetMapData(VECTOR2(x*CHIP_SIZE, y*CHIP_SIZE), EDIT_CHIP_BGD_R);
		}
		else if (y == 14 && (x > 1 && x < 12))
		{
			SetMapData(VECTOR2(x*CHIP_SIZE, y*CHIP_SIZE), EDIT_CHIP_BGD_D);
		}
		else if (y == 14 && x == 1)
		{
			SetMapData(VECTOR2(x*CHIP_SIZE, y*CHIP_SIZE), EDIT_BGD_CURVE_L);
		}
		else if (y == 14 && x == 12)
		{
			SetMapData(VECTOR2(x*CHIP_SIZE, y*CHIP_SIZE), EDIT_BGD_CURVE_J);
		}
		return true;
	}
	return false;
}

bool MapCtl::GetMapDig(VECTOR2 pos)
{
	VECTOR2 tmpPos(pos / CHIP_SIZE);
	if ((tmpPos.y < (int)mapID.size()) && (tmpPos.x < (int)mapID[tmpPos.y].size()))
	{
		if (tmpPos.x == 0 || tmpPos.x == GAME_AREA_CNT_X || tmpPos.y == GAME_AREA_CNT_Y)
		{
			return false;
		}
	}
	return true;
}

bool MapCtl::CheckMapData(VECTOR2 pos, CHIP_GRP grpID, CHIP_TYPE type)
{
	VECTOR2 tmpPos(pos / CHIP_SIZE);
	if ((tmpPos.y < (int)mapID.size()) && (tmpPos.x < (int)mapID[tmpPos.y].size()))
	{
		SaveMap = (CHIP_TYPE)((mapID[tmpPos.y][tmpPos.x] >> (grpID * DATA_BIT_LEN)) & 0xff);

		if (SaveMap == LDR_CHIP_BLANK)
		{
			return false;
		}
	}
	return true;
}


void MapCtl::SetDir(int Dir)
{
	this->Dir = Dir;
}

int MapCtl::GetChipSize(void)
{
	return CHIP_SIZE;
}


bool MapCtl::MapSave(void)
{
	DataHeader expData = {
		BBM_ID_NAME,
		BBM_VER_ID,
		{ 0,0 },
		GAME_AREA_CNT_X,
		GAME_AREA_CNT_Y,
		{ 0,0,0 },
		0x00
	};

	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		for (unsigned int x = 0; x < mapID[y].size(); x++)
		{
			expData.sum += mapID[y][x];
		}
	}
	FILE *file;
	fopen_s(&file, "data/mapData.map", "wb");
	fwrite(&expData, sizeof(DataHeader), 1, file);
	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		fwrite(&mapID[y][0], sizeof(CHIP_TYPE) * mapID[y].size(), 1, file);
	}
	fclose(file);
	return true;
}

bool MapCtl::MapLoadStart(void)
{
	bool rtnFlag = true;
	DataHeader expData;
	FILE *file;
	fopen_s(&file, "data/mapData_start.map", "rb");
	fread(&expData, sizeof(DataHeader), 1, file);
	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		fread(&mapID[y][0], sizeof(CHIP_TYPE) * mapID[y].size(), 1, file);
	}
	fclose(file);

	return rtnFlag;
}

bool MapCtl::MapLoad(void)
{
	bool rtnFlag = true;
	DataHeader expData;
	FILE *file;
	fopen_s(&file, "data/mapData.map", "rb");
	fread(&expData, sizeof(DataHeader), 1, file);
	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		fread(&mapID[y][0], sizeof(CHIP_TYPE) * mapID[y].size(), 1, file);
	}
	fclose(file);

	return rtnFlag;
}

bool MapCtl::MapCreate(void)
{
	bool rtnFlag = true;
	DataHeader expData;
	FILE *file;
	fopen_s(&file, "data/noData.map", "rb");
	fread(&expData, sizeof(DataHeader), 1, file);
	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		fread(&mapID[y][0], sizeof(CHIP_TYPE) * mapID[y].size(), 1, file);
	}
	fclose(file);

	return rtnFlag;
}

void MapCtl::MapDraw(void)
{
	for (CHIP_GRP gp = LDR_GP_BG; gp < LDR_GP_MAX; gp = (CHIP_GRP)(gp + 1))
	{
		for (int y = 0; y < GAME_AREA_CNT_Y; y++)
		{
			for (int x = 0; x < GAME_AREA_CNT_X; x++)
			{
				if (GET_MAP_ID2(x, y, gp) != LDR_CHIP_BLANK)
				{
					CHIP_TYPE tmp = GET_MAP_ID2(x, y, gp);

					if (tmp >= EDIT_CHIP_BBL && tmp < EDIT_CHIP_PL && tmp != EDIT_CHIP_BBL_OUT)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);
					}
					if (tmp == EDIT_CHIP_BBL_OUT || (tmp >= EDIT_CHIP_PL && tmp <= EDIT_CHIP_ENEMY2) || tmp == ITEM_CHIP_STONE)
					{
						if (GameTask::GetInstance().CheckEditMode())
						{
							DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);
						}
					}
					if (tmp == ITEM_CHIP_FLOWER)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);
					}
					if (tmp == EDIT_CHIP_BGD_L)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);
					}
					if (tmp == EDIT_CHIP_BGD_R)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);
					}
					if (tmp == EDIT_CHIP_BGD_D)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);
					}
					if (tmp == EDIT_BGD_CURVE_L)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);

					}
					if (tmp == EDIT_BGD_CURVE_J)
					{
						DrawExtendGraph(x * GET_MAP_SIZE(), y * GET_MAP_SIZE(), x * GET_MAP_SIZE() + CHIP_SIZE, y * GET_MAP_SIZE() + CHIP_SIZE, IMAGE_ID("image/Edit_map_Chip2.png")[GET_MAP_ID2(x, y, gp)], true);

					}
				}
			}

		}
	}

	if (GameTask::GetInstance().CheckEditMode())
	{
		VECTOR2 tmp1 = GameTask::GetInstance().GetOffset();
		VECTOR2 tmp2(0, 0);
		for (; tmp1.x <= (CHIP_SIZE * 13); tmp1.x += CHIP_SIZE)
		{
			tmp2 = VECTOR2(tmp1.x, (CHIP_SIZE * 15));
			DrawLine(tmp1, tmp2, 0xffffff);
		}

		tmp1 = GameTask::GetInstance().GetOffset();
		for (; tmp1.y <= (CHIP_SIZE * 15); tmp1.y += CHIP_SIZE)
		{
			tmp2 = VECTOR2((CHIP_SIZE * 13), tmp1.y);
			DrawLine(tmp1, tmp2, 0xffffff);
		}
	}
}

MapCtl::MapCtl()
{
	mapID.resize(GAME_AREA_CNT_Y);			//YŽ²
	for (unsigned int j = 0; j < GAME_AREA_CNT_Y; j++)
	{
		mapID[j].resize(GAME_AREA_CNT_X);	//XŽ²
	}

	// ¸ÞÙ°Ìß•ª‚¯

	// ÌÞÛ¯¸Œn“
	// LDR_GP_BG...ÌÞÛ¯¸Œn“
	chipGrp[EDIT_CHIP_BBL] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_WBL] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_UBL] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL75] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL50] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL25] = LDR_GP_BG;

	chipGrp[EDIT_CHIP_BBL75_L] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL50_L] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL25_L] = LDR_GP_BG;

	chipGrp[EDIT_CHIP_BBL75_D] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL50_D] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL25_D] = LDR_GP_BG;

	chipGrp[EDIT_CHIP_BBL75_U] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL50_U] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL25_U] = LDR_GP_BG;

	chipGrp[EDIT_CHIP_BBL05] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL05_L] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL05_U] = LDR_GP_BG;
	chipGrp[EDIT_CHIP_BBL05_D] = LDR_GP_BG;

	// LDR_GP_SD... ”wŒi

	chipGrp[EDIT_CHIP_BBL_BLACK_R] = LDR_GP_SD;
	chipGrp[EDIT_CHIP_BBL_BLACK_RM] = LDR_GP_SD;

	chipGrp[EDIT_CHIP_BBL_BLACK_L] = LDR_GP_SD;
	chipGrp[EDIT_CHIP_BBL_BLACK_LM] = LDR_GP_SD;

	chipGrp[EDIT_CHIP_BBL_BLACK_U] = LDR_GP_SD;
	chipGrp[EDIT_CHIP_BBL_BLACK_UM] = LDR_GP_SD;

	chipGrp[EDIT_CHIP_BBL_BLACK_D] = LDR_GP_SD;
	chipGrp[EDIT_CHIP_BBL_BLACK_DM] = LDR_GP_SD;

	chipGrp[EDIT_CHIP_BGD_L] = LDR_GP_SD;
	chipGrp[EDIT_CHIP_BGD_R] = LDR_GP_SD;
	chipGrp[EDIT_CHIP_BGD_D] = LDR_GP_SD;

	chipGrp[EDIT_BGD_CURVE_L] = LDR_GP_SD;
	chipGrp[EDIT_BGD_CURVE_J] = LDR_GP_SD;

	chipGrp[EDIT_BGD_BLANK] = LDR_GP_SD;

	// ±²ÃÑŒn“
	chipGrp[ITEM_CHIP_FLOWER] = LDR_GP_ITEM;
	chipGrp[ITEM_CHIP_STONE] = LDR_GP_ITEM;
	chipGrp[ITEM_CHIP_BLANK] = LDR_GP_ITEM;

	// ·¬×¸À°Œn“
	chipGrp[EDIT_CHIP_PL] = LDR_GP_CHAR;
	chipGrp[EDIT_CHIP_ENEMY] = LDR_GP_CHAR;
	chipGrp[EDIT_CHIP_ENEMY2] = LDR_GP_CHAR;

	// ˆÚ“®‹ÖŽ~êŠ
	chipGrp[EDIT_CHIP_BBL_OUT] = LDR_GP_BG;

}


MapCtl::~MapCtl()
{
}
