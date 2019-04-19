#include "GameTask.h"
#include "MapCtl.h"
#include "EditCursor.h"
#include "ImageMng.h"
#include "Player.h"
#include "Enemy.h"
#include "Dxlib.h"	// DxLib×²ÌŞ×Ø‚ğg—p‚·‚é

GameTask *GameTask::s_Instance = nullptr;

void GameTask::Create()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new GameTask();
	}
}

void GameTask::Destroy()
{
	if (s_Instance)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;

}

GameTask::GameTask()
{
	GtskPtr = &GameTask::SysInit;
	SetOffset(VECTOR2(32, 32));
}

GameTask::~GameTask()
{
	GameTask::SysDestroy();
}

int GameTask::SysInit(void)
{

	// ¼½ÃÑˆ—
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536FÓ°ÄŞ‚Éİ’è
	ChangeWindowMode(true);								// true:window@false:ÌÙ½¸Ø°İ
	SetWindowText("1701313_š O‘ñŠC");
	if (DxLib_Init() == -1)	return false;				// DX×²ÌŞ×Ø‰Šú‰»ˆ—
	SetDrawScreen(DX_SCREEN_BACK);						// ‚Ğ‚Æ‚Ü‚¸ÊŞ¯¸ÊŞ¯Ì§‚É•`‰æ
	PlayerLife = 2;										// ‰Šú‘Ì—Í(2)
	ImageMng::Create();
	MapCtl::Create();

	GtskPtr = &GameTask::Title;
	return true;
}

int GameTask::Update(void)
{
	int rtn_id;
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	rtn_id = (this->*GtskPtr)();
	return rtn_id;
}

void GameTask::SetOffset(VECTOR2 pos)
{
	drawOffset = pos;
}

const VECTOR2 & GameTask::GetOffset(void)
{
	return drawOffset;
}

void GameTask::SetStartFlag(bool AnimFlag)
{
	GameStartFlag = AnimFlag;
}

bool GameTask::CheckEditMode(void)
{
	if ((GtskPtr == &GameTask::EditInit) || (GtskPtr == &GameTask::EditMain))
	{
		return true;
	}

	return false;
}

void GameTask::AddScore(int score)
{
	One_UP += score;
}

const int & GameTask::GetClearCnt(void)
{
	return ClearCnt;
}

int GameTask::Title(void)
{
	ClsDrawScreen();
	if (TitleCallMove > 0)
	{
		TitleCallMove -= 2;
	}
	else
	{
		if (keyData[KEY_INPUT_RETURN] && !keyDataOld[KEY_INPUT_RETURN])
		{
			MapCtl::GetInstance().MapCreate();
			objList.clear();

			lpMapCtl.MapLoad();

			GtskPtr = &GameTask::EditInit;

		}
	}

	SetFontSize(20);		// Ì«İÄ‚Ì»²½Ş
	SetFontThickness(10);	// Ì«İÄ‚Ì‘¾‚³
	ChangeFont("ƒƒCƒŠƒI");

	DrawFormatString(150, 60 + TitleCallMove, GetColor(200, 200, 200), "1UP \n %d", One_UP);

	DrawFormatString(300, 60 + TitleCallMove, GetColor(255, 50, 50), "HIGH-SCORE \n %d", HighScore);
	DrawExtendGraph(TitleCallsetX, TitleCallsetY + TitleCallMove, (TitleCallsetX + TitleCallSize.x), (TitleCallsetY + TitleCallSize.y) + TitleCallMove, IMAGE_ID("image/ƒ^ƒCƒgƒ‹‰æ‘œ.png")[0], true);



	SetFontSize(40);		// Ì«İÄ‚Ì»²½Ş
	SetFontThickness(20);	// Ì«İÄ‚Ì‘¾‚³
	ChangeFont("HGS‘n‰pŠpÎß¯Ìß‘Ì");

	DrawString(200, 220 + TitleCallMove, "`ASO EDITION`", GetColor(255, 80, 160));

	SetFontSize(20);		// Ì«İÄ‚Ì»²½Ş
	SetFontThickness(8);	// Ì«İÄ‚Ì‘¾‚³
	ChangeFont("ƒƒCƒŠƒI");
	//ChangeFont("MSƒSƒVƒbƒN");

	//DrawString(0, 0, "Title", GetColor(255, 255, 255));

	ScreenFlip();
	return true;
}

int GameTask::GameInit(void)
{
	objList.clear();

	ChipSize = VECTOR2(MapCtl::GetInstance().GetChipSize(), MapCtl::GetInstance().GetChipSize());
	SetOffset(VECTOR2(32, 32));
	for (CHIP_GRP gp = LDR_GP_BG; gp < LDR_GP_MAX; gp = (CHIP_GRP)(gp + 1))
	{
		for (int y = 0; y < GAME_AREA_CNT_Y; y++)
		{
			for (int x = 0; x < GAME_AREA_CNT_X; x++)
			{
				CHIP_TYPE tmp = GET_MAP_ID2(x, y, gp);
				if (!CheckEditMode())
				{
					if (tmp == EDIT_CHIP_PL)
					{
						player = AddObjlist(std::make_shared<Player>(keyData, keyDataOld, GetOffset()));
						(*player)->init("image/Player.png", VECTOR2(128 / 8, 64 / 4), VECTOR2(8, 4), VECTOR2(1, 0), 1);
						(*player)->SetPos(VECTOR2(x * CHIP_SIZE - 1, y * CHIP_SIZE - 1));
					}
					else if (tmp == EDIT_CHIP_ENEMY)
					{
						enemy1 = AddObjlist(std::make_shared<Enemy>((*player), GetOffset(), 1));
						(*enemy1)->init("image/Enemy.png", VECTOR2(80 / 5, 32 / 2), VECTOR2(5, 2), VECTOR2(1, 0), 1);
						(*enemy1)->SetPos(VECTOR2(x * CHIP_SIZE - 1, y * CHIP_SIZE - 1));
						(*enemy1)->AddEnemyCnt(1);
					}
					else if (tmp == EDIT_CHIP_ENEMY2)
					{
						enemy2 = AddObjlist(std::make_shared<Enemy>((*player), GetOffset(), 2));
						(*enemy2)->init("image/Enemy.png", VECTOR2(80 / 5, 32 / 2), VECTOR2(5, 2), VECTOR2(1, 0), 1);
						(*enemy2)->SetPos(VECTOR2(x * CHIP_SIZE - 1, y * CHIP_SIZE - 1));
						(*enemy2)->AddEnemyCnt(1);
					}
				}
			}
		}
	}

	for (int y = 0; y <= GAME_AREA_CNT_Y; y++)
	{
		for (int x = 0; x <= GAME_AREA_CNT_X; x++)
		{
			lpMapCtl.SetMapDig(x, y);
		}
	}
	GtskPtr = &GameTask::GameMain;
	return true;
}

int GameTask::GameMain(void)
{
	ClsDrawScreen();
	int enemyCnt = 0;

	if (keyData[KEY_INPUT_F1] && !keyDataOld[KEY_INPUT_F1])
	{
		GtskPtr = &GameTask::EditInit;

	}
	
	lpMapCtl.MapDraw();

	// Ê²½º±
	DrawFormatString(CHIP_SIZE * GAME_AREA_CNT_X, CHIP_SIZE * 2, GetColor(255, 255, 255), "HIGH-\n SCORE \n@ %d ", HighScore);

	// Œ»İ‚Ì“¾“_
	DrawFormatString(CHIP_SIZE * GAME_AREA_CNT_X, CHIP_SIZE * 5, GetColor(255, 255, 255), "@1UP\n@%d ", One_UP);

	/*DrawFormatString(50, 40, GetColor(255, 255, 255), "UŒ‚À•WX %d ", (*enemy2)->GetAttackPos().x);
	DrawFormatString(50, 60, GetColor(255, 255, 255), "UŒ‚À•WY %d ", (*enemy2)->GetAttackPos().y);*/

	/*DrawFormatString(150, 40, GetColor(255, 255, 255), "“GÀ•WX@%d", ((*enemy2)->GetPos().x + 2) / CHIP_SIZE);
	DrawFormatString(150, 60, GetColor(255, 255, 255), "“GÀ•WY@%d", ((*enemy2)->GetPos().y  + 2)/ CHIP_SIZE);

	DrawFormatString(10, 40, GetColor(255, 255, 255), "UŒ‚À•WX@%d", (*player)->GetAttackPos().x);
	DrawFormatString(10, 60, GetColor(255, 255, 255), "UŒ‚À•WY@%d", (*player)->GetAttackPos().y);*/

	for (auto itr : objList)
	{
		if (itr->GetEnemyCnt() == 1)
		{
			enemyCnt += itr->GetEnemyCnt();
		}
		if (!((*player)->GetBeatenFlag()))
		{
			itr->Update();
		}
		if (itr != (*player))
		{

			// ‹ò‚ç‚¢”»’è
			if (((*player)->GetPos().y < (itr->GetPos().y + itr->GetSize().y)) && ((*player)->GetPos().y + (*player)->GetSize().y) > itr->GetPos().y)
			{
				if (((*player)->GetPos().x < (itr->GetPos().x + itr->GetSize().x + itr->GetAttackPos().x)) && ((*player)->GetPos().x + (*player)->GetSize().x) > itr->GetPos().x + itr->GetAttackPos().x)
				{
					if(itr->GetBeatenCnt() == 0)
					(*player)->SetBeatenFlag(true);
				}
			}

			// UŒ‚”»’è
			// Y²
			/*if ((itr->GetPos().y < ((*player)->GetPos().y + (*player)->GetSize().y)) && ((*player)->GetPos().y + (*player)->GetSize().y) > itr->GetPos().y)
			{*/
			if ((itr->GetPos().y / CHIP_SIZE) == (*player)->GetAttackPos().y)
			{
				// X²
				if ((itr->GetPos().x / CHIP_SIZE) == (*player)->GetAttackPos().x)
				{
					itr->Beaten(1);
					(*player)->SetAttackPos(VECTOR2(999,999));
				}
			}

		}
	}
	if((*player)->GetBeatenFlag())
	{
		(*player)->Update();
	}

	for (auto itr : objList)
	{
		itr->Draw();
		
	}

	if (!GameStartFlag)
	{
		DrawString((SCREEN_SIZE_X / 2) / 2, 200, "‚o‚k‚`‚x‚d‚q‚P", GetColor(255, 255, 255));
		DrawString((SCREEN_SIZE_X / 2) / 2, 300, "‚q‚`‚c‚x", GetColor(255, 255, 255));
	}
	else
	{
		OPCnt++;
		if (OPCnt <= 60)
		{
			DrawString((SCREEN_SIZE_X / 2) / 2, 200, "‚o‚k‚`‚x‚d‚q‚P", GetColor(255, 255, 255));
			DrawString((SCREEN_SIZE_X / 2) / 2, 300, "‚q‚`‚c‚x", GetColor(255, 255, 255));
		}
	}

	//DrawString(0, 0, "Main", GetColor(255, 255, 255));
	DrawFormatString(CHIP_SIZE * GAME_AREA_CNT_X, CHIP_SIZE * GAME_AREA_CNT_Y - (CHIP_SIZE * 2), GetColor(255, 255, 255), "ROUND \n@ %d ", StageCnt);
	

	if ((*player)->GetBeatenFlag() == true)
	{
		if ((*player)->Beaten(0) == true)
		{
			PlayerLife--;
			if (PlayerLife >= 0)
			{
				//MapCtl::GetInstance().MapSave();
			}
			else
			{
				if (One_UP > HighScore)
				{
					HighScore = One_UP;
					One_UP = 0;
				}
			}
			//lpMapCtl.MapLoad();
			OPCnt = 0;
			GtskPtr = ((PlayerLife < 0) ? &GameTask::Result : &GameTask::GameInit);
		}
	}
	if (enemyCnt == 0)
	{
		ClearCnt++;
		if (ClearCnt > 120)
		{
			StageCnt++;
			lpMapCtl.MapLoad();
			OPCnt = 0;
			ClearCnt = 0;
			GtskPtr = &GameTask::EditInit;
		}
	}
	

	/*DrawFormatString(150, 10, GetColor(255, 255, 255), "“G”@%d", enemyCnt);
	DrawFormatString(220, 10, GetColor(255, 255, 255), "ŠK‘w@%d", StageCnt);*/
	ScreenFlip();
	return true;
}

int GameTask::EditInit(void)
{

	objList.clear();

	//lpMapCtl.MapLoad();

	//ChipSize = VECTOR2(MapCtl::GetInstance().GetChipSize(), MapCtl::GetInstance().GetChipSize());

	SetOffset(VECTOR2(32, 32));

	objList.push_back(std::make_shared<EditCursor>(keyData, keyDataOld, GetOffset()));
	auto itr = objList.end();
	itr--;
	(*itr)->init("image/Edit_map_Chip2.png", VECTOR2(164 / 10, 80 / 5), VECTOR2(10, 5), VECTOR2(0, 0), 3);// 16,16
	//GtskPtr = &GameTask::GameInit;
	GtskPtr = &GameTask::EditMain;
	return true;
}

int GameTask::EditMain(void)
{
	ClsDrawScreen();
	if (keyData[KEY_INPUT_F1] && !keyDataOld[KEY_INPUT_F1])
	{
		GtskPtr = &GameTask::GameInit;

	}

	if (keyData[KEY_INPUT_RETURN] && !keyDataOld[KEY_INPUT_RETURN])
	{
		GtskPtr = &GameTask::Result;

	}

	if (keyData[KEY_INPUT_F5] && !keyDataOld[KEY_INPUT_F5])
	{
		if (MessageBox(GetMainWindowHandle(), "Edit“à—e‚ğƒZ[ƒu‚µ‚Ü‚·‚©H", "Šm”Fƒ_ƒCƒAƒƒO", MB_OKCANCEL) == IDOK)
		{
			MapCtl::GetInstance().MapSave();
		}

	}

	if (keyData[KEY_INPUT_F6] && !keyDataOld[KEY_INPUT_F6])
	{
		/*if (MessageBox(GetMainWindowHandle(), "ƒZ[ƒu“à—e‚ğƒ[ƒh‚µ‚Ü‚·‚©H", "Šm”Fƒ_ƒCƒAƒƒO", MB_OKCANCEL) == IDOK)
		{*/
			MapCtl::GetInstance().MapLoad();
		//}
	}

	if (keyData[KEY_INPUT_F7] && !keyDataOld[KEY_INPUT_F7])
	{
		if (MessageBox(GetMainWindowHandle(), "ƒ}ƒbƒvó‘Ô‚ğ”jŠü‚µ‚Ü‚·‚©H", "Šm”Fƒ_ƒCƒAƒƒO", MB_OKCANCEL) == IDOK)
		{
			MapCtl::GetInstance().MapCreate();
		}
	}

	lpMapCtl.MapDraw();

	for (auto itr : objList)
	{
		itr->Update();
	}
	for (auto itr : objList)
	{
		itr->Draw();
	}

	//DrawString(0, 0, "Edit",GetColor(255, 255, 255));

	ScreenFlip();
	return true;
}

int GameTask::Result(void)
{
	if (keyData[KEY_INPUT_RETURN] && !keyDataOld[KEY_INPUT_RETURN])
	{
		GameStartFlag = false;
		GtskPtr = &GameTask::SysInit;

	}

	DrawString((SCREEN_SIZE_X / 2) / 2, 300, "‚o‚k‚`‚x‚d‚q‚P", GetColor(255, 255, 255));

	DrawString((SCREEN_SIZE_X / 2) / 2, 350, "‚f‚`‚l‚d‚n‚u‚d‚q", GetColor(255, 255, 255));


	//DrawString(0, 0, "Result", GetColor(255, 255, 255));

	ScreenFlip();
	return true;
}


int GameTask::GameDestroy(void)
{
	return true;
}

int GameTask::SysDestroy(void)
{
	ImageMng::Destroy();
	return 0;
}

std::list<obj_ptr>::iterator GameTask::AddObjlist(obj_ptr &&objPtr)
{
	objList.push_back(objPtr);
	auto itr = objList.end();
	itr--;
	return itr;
}

int DrawLine(VECTOR2 vec1, VECTOR2 vec2, unsigned int Color,int Thickness)
{
	DrawLine(vec1.x, vec1.y, vec2.x, vec2.y, Color, Thickness);
	return 0;
}