#pragma once

#include <vector>
#include "VECTOR2.h"

#pragma pack(1)
struct DataHeader {
	char fileID[13];				// ﾌｧｲﾙのID情報
	unsigned char  verID;			// ﾌｧｲﾙのﾊﾞｰｼﾞｮﾝ番号
	unsigned char rez1[2];			// ｱﾗｲﾒﾝﾄ分予約領域とする
	int sizeX;						// ﾏｯﾌﾟのｻｲｽﾞX
	int sizeY;;						// ﾏｯﾌﾟのｻｲｽﾞY
	unsigned char rez2[3];			// ｱﾗｲﾒﾝﾄ分予約領域とするその２
	unsigned char sum;				// SUM値
};
#pragma pack()

#define BBM_ID_NAME "BBM_MAP_DATA"	// ﾌｧｲﾙﾈｰﾑ
#define BBM_VER_ID	0x01			// ﾌｧｲﾙﾊﾞｰｼﾞｮﾝ
#define lpMapCtl MapCtl::GetInstance()

enum CHIP_GRP {
	LDR_GP_BG,			// ﾌﾞﾛｯｸ
	LDR_GP_SD,
	LDR_GP_ITEM,		// ｱｲﾃﾑ
	LDR_GP_CHAR,		// ｷｬﾗｸﾀｰ
	LDR_GP_MAX,
};

enum CHIP_TYPE {
	LDR_CHIP_BLANK,		// 空白
	//LDR_CHIP_BLANK_NEW = 11,

	EDIT_CHIP_BBL = 1,			// 掘れるﾌﾞﾛｯｸ

	//右
	EDIT_CHIP_BBL75,		// 75%
	EDIT_CHIP_BBL50,		// 50%
	EDIT_CHIP_BBL25,		// 25%
	EDIT_CHIP_BBL05,			// 5%

	// 岩が通過するブロック
	EDIT_CHIP_WBL,		// 上側の掘ったﾌﾞﾛｯｸ
	EDIT_CHIP_UBL,		// 横側の掘ったﾌﾞﾛｯｸ

	EDIT_CHIP_BBL_OUT,	// 移動禁止

	EDIT_CHIP_BGD_R = 11,		// 背景用の留め
	EDIT_CHIP_BGD_L = 21,
	EDIT_CHIP_BGD_D = 31,
	EDIT_BGD_CURVE_L = 37,	// 背景用の曲線
	EDIT_BGD_CURVE_J = 38,	// 背景用の曲線
	EDIT_BGD_BLANK,			// 背景用の上書き用

	// 左
	EDIT_CHIP_BBL75_L = 12,		// 75%
	EDIT_CHIP_BBL50_L,		// 50%
	EDIT_CHIP_BBL25_L,		// 25%
	EDIT_CHIP_BBL05_L,		// 5%

	EDIT_CHIP_BBL_BLACK_R,	// 重ね用
	EDIT_CHIP_BBL_BLACK_L,
	EDIT_CHIP_BBL_BLACK_U,
	EDIT_CHIP_BBL_BLACK_D,


	// 下
	EDIT_CHIP_BBL75_D = 22,		// 75%
	EDIT_CHIP_BBL50_D,		// 50%
	EDIT_CHIP_BBL25_D,		// 25%
	EDIT_CHIP_BBL05_D,		// 5%

	EDIT_CHIP_BBL_BLACK_RM,	// 重ね用
	EDIT_CHIP_BBL_BLACK_LM,
	EDIT_CHIP_BBL_BLACK_UM,
	EDIT_CHIP_BBL_BLACK_DM,

	// 上
	EDIT_CHIP_BBL75_U = 32,		// 75%
	EDIT_CHIP_BBL50_U,		// 50%
	EDIT_CHIP_BBL25_U,		// 25%
	EDIT_CHIP_BBL05_U,		// 5%

	EDIT_CHIP_PL = 40,		// ﾌﾟﾚｲﾔｰ

	EDIT_CHIP_ENEMY,		// 敵
	EDIT_CHIP_ENEMY2,	// 敵2

	ITEM_CHIP_FLOWER,	// 花
	ITEM_CHIP_STONE,		// 岩
	ITEM_CHIP_BLANK,


	LDR_CHIP_EDIT,		// ｴﾃﾞｨｯﾄ用ｶｰｿﾙ
	LDR_CHIP_CNT_X,		// 横方向の画像数

};

#define LAST_EDIT_CHIP ITEM_CHIP_STONE		//	ｴﾃﾞｨｯﾄで使う最後のCHIP
#define START_EDIT_CHIP LDR_CHIP_BLANK

#define SET_MAP_ID(X,Y)  MapCtl::GetInstance().SetMapData(X,Y)
#define GET_MAP_ID(X,Z)	 MapCtl::GetInstance().GetMapData(X,Z)
#define GET_MAP_ID2(X,Y,Z) MapCtl::GetInstance().GetMapData(X,Y,Z)
#define GET_MAP_SIZE()	 MapCtl::GetInstance().GetChipSize()


using VEC_CHIP = std::vector<unsigned int>;
using VEC_CHIP2 = std::vector<VEC_CHIP>;

constexpr unsigned int BlockStart = 2;
constexpr unsigned int GAME_AREA_CNT_X = 13;
constexpr unsigned int GAME_AREA_CNT_Y = 15;
constexpr unsigned int CHIP_SIZE = 32;
constexpr unsigned int CHIP_CHAR_SIZE = 16;
constexpr unsigned int DATA_BIT_LEN = 8;
class MapCtl
{
public:
	static void Create();
	static void Destroy();

	static MapCtl &GetInstance()
	{
		Create();
		return *s_Instance;
	}
	bool SetMapData(VECTOR2 pos, CHIP_TYPE id);
	CHIP_TYPE GetMapData(VECTOR2 pos, CHIP_GRP grpID);
	CHIP_TYPE GetMapData(int x,int y, CHIP_GRP grpID);

	bool SetMapDig(int x,int y);
	bool GetMapDig(VECTOR2 pos);

	bool CheckMapData(VECTOR2 pos, CHIP_GRP grpID, CHIP_TYPE type);

	void SetDir(int Dir);

	int GetChipSize(void);

	bool MapSave(void);

	bool MapLoadStart(void);

	bool MapLoad(void);
	bool MapCreate(void);

	void MapDraw(void);


private:

	MapCtl();
	~MapCtl();

	static MapCtl *s_Instance;
	VEC_CHIP2 mapID;
	CHIP_GRP chipGrp[LDR_CHIP_CNT_X];
	int Dir = 1;
	CHIP_TYPE SaveMap = LDR_CHIP_BLANK;
};

