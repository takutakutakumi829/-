#pragma once

#include <vector>
#include "VECTOR2.h"

#pragma pack(1)
struct DataHeader {
	char fileID[13];				// Ãß≤ŸÇÃIDèÓïÒ
	unsigned char  verID;			// Ãß≤ŸÇÃ ﬁ∞ºﬁÆ›î‘çÜ
	unsigned char rez1[2];			// ±◊≤“›ƒï™ó\ñÒóÃàÊÇ∆Ç∑ÇÈ
	int sizeX;						// œØÃﬂÇÃª≤ΩﬁX
	int sizeY;;						// œØÃﬂÇÃª≤ΩﬁY
	unsigned char rez2[3];			// ±◊≤“›ƒï™ó\ñÒóÃàÊÇ∆Ç∑ÇÈÇªÇÃÇQ
	unsigned char sum;				// SUMíl
};
#pragma pack()

#define BBM_ID_NAME "BBM_MAP_DATA"	// Ãß≤Ÿ»∞—
#define BBM_VER_ID	0x01			// Ãß≤Ÿ ﬁ∞ºﬁÆ›
#define lpMapCtl MapCtl::GetInstance()

enum CHIP_GRP {
	LDR_GP_BG,			// Ãﬁ€Ø∏
	LDR_GP_SD,
	LDR_GP_ITEM,		// ±≤√—
	LDR_GP_CHAR,		// ∑¨◊∏¿∞
	LDR_GP_MAX,
};

enum CHIP_TYPE {
	LDR_CHIP_BLANK,		// ãÛîí
	//LDR_CHIP_BLANK_NEW = 11,

	EDIT_CHIP_BBL = 1,			// å@ÇÍÇÈÃﬁ€Ø∏

	//âE
	EDIT_CHIP_BBL75,		// 75%
	EDIT_CHIP_BBL50,		// 50%
	EDIT_CHIP_BBL25,		// 25%
	EDIT_CHIP_BBL05,			// 5%

	// ä‚Ç™í âﬂÇ∑ÇÈÉuÉçÉbÉN
	EDIT_CHIP_WBL,		// è„ë§ÇÃå@Ç¡ÇΩÃﬁ€Ø∏
	EDIT_CHIP_UBL,		// â°ë§ÇÃå@Ç¡ÇΩÃﬁ€Ø∏

	EDIT_CHIP_BBL_OUT,	// à⁄ìÆã÷é~

	EDIT_CHIP_BGD_R = 11,		// îwåiópÇÃóØÇﬂ
	EDIT_CHIP_BGD_L = 21,
	EDIT_CHIP_BGD_D = 31,
	EDIT_BGD_CURVE_L = 37,	// îwåiópÇÃã»ê¸
	EDIT_BGD_CURVE_J = 38,	// îwåiópÇÃã»ê¸
	EDIT_BGD_BLANK,			// îwåiópÇÃè„èëÇ´óp

	// ç∂
	EDIT_CHIP_BBL75_L = 12,		// 75%
	EDIT_CHIP_BBL50_L,		// 50%
	EDIT_CHIP_BBL25_L,		// 25%
	EDIT_CHIP_BBL05_L,		// 5%

	EDIT_CHIP_BBL_BLACK_R,	// èdÇÀóp
	EDIT_CHIP_BBL_BLACK_L,
	EDIT_CHIP_BBL_BLACK_U,
	EDIT_CHIP_BBL_BLACK_D,


	// â∫
	EDIT_CHIP_BBL75_D = 22,		// 75%
	EDIT_CHIP_BBL50_D,		// 50%
	EDIT_CHIP_BBL25_D,		// 25%
	EDIT_CHIP_BBL05_D,		// 5%

	EDIT_CHIP_BBL_BLACK_RM,	// èdÇÀóp
	EDIT_CHIP_BBL_BLACK_LM,
	EDIT_CHIP_BBL_BLACK_UM,
	EDIT_CHIP_BBL_BLACK_DM,

	// è„
	EDIT_CHIP_BBL75_U = 32,		// 75%
	EDIT_CHIP_BBL50_U,		// 50%
	EDIT_CHIP_BBL25_U,		// 25%
	EDIT_CHIP_BBL05_U,		// 5%

	EDIT_CHIP_PL = 40,		// Ãﬂ⁄≤‘∞

	EDIT_CHIP_ENEMY,		// ìG
	EDIT_CHIP_ENEMY2,	// ìG2

	ITEM_CHIP_FLOWER,	// â‘
	ITEM_CHIP_STONE,		// ä‚


	LDR_CHIP_EDIT,		// ¥√ﬁ®Øƒóp∂∞øŸ
	LDR_CHIP_CNT_X,		// â°ï˚å¸ÇÃâÊëúêî

};

#define LAST_EDIT_CHIP ITEM_CHIP_STONE		//	¥√ﬁ®ØƒÇ≈égÇ§ç≈å„ÇÃCHIP
#define START_EDIT_CHIP LDR_CHIP_BLANK
//#define GAME_AREA_CNT_X (13)
//#define GAME_AREA_CNT_Y (15)
//#define CHIP_SIZE		(32)
//#define CHIP_CHAR_SIZE	(16)
#define SET_MAP_ID(X,Y)  MapCtl::GetInstance().SetMapData(X,Y)
#define GET_MAP_ID(X,Z)	 MapCtl::GetInstance().GetMapData(X,Z)
#define GET_MAP_ID2(X,Y,Z) MapCtl::GetInstance().GetMapData(X,Y,Z)
#define GET_MAP_SIZE()	 MapCtl::GetInstance().GetChipSize()
//# define DATA_BIT_LEN 8


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

