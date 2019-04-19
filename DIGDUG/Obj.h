#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "VECTOR2.h"
#include "MapCtl.h"

#define GAME_EDIT_SIZE_X 32
#define GAME_EDIT_SIZE_Y 32

// �D�揇��
// RIGHT>DOWN>LEFT>UP
enum DIR {		// ����
	DIR_NON,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP,
	DIR_MAX,
};

enum ANIM_TBL {			// ��Ұ���ð���
	ANIM_TBL_START_ID,
	ANIM_TBL_FRAME,
	ANIM_TBL_INV,
	ANIM_TBL_MAX,
};

class Obj
{
public:
	//Obj();
	Obj(const char (&_keyData)[256], const char (&_keyDataOld)[256], VECTOR2 chipOffset);
	~Obj();
	
	void init(std::string filname, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed);
	virtual bool Update(void);			//	�X�V����
	virtual void Draw(void);			//	�`��p

	virtual bool Beaten(int count);			//�@����


	const bool& GetBeatenFlag(void);
	void SetBeatenFlag(const bool& BeatenFlag);

	const int& GetEnemyCnt(void);
	void AddEnemyCnt(int Cnt);

	const bool& GetBeatenCnt(void);


	// �U���֌W
	void SetAttackPos(VECTOR2 pos);
	const VECTOR2& GetAttackPos(void);


	void Draw(int id);
	const VECTOR2& GetSize(void);
	void SetSize(const VECTOR2& divSize);
	bool SetPos(VECTOR2 pos);
	const VECTOR2& GetPos(void);
	bool SetAnim(std::string animName);
	std::string GetAnim(void);

private:
	virtual bool InitAnim(void);
	std::string imageName;
	VECTOR2 divSize = { 0,0 };			//	�`�����߂̻���
	VECTOR2 divCnt = { 0,0 };			//	�`�����߂̐�
	VECTOR2 chipOffset = { 0,0 };			//	�`�����߂̲Ұ��ID�̊J�n�ʒu
	std::map<std::string, int[ANIM_TBL_MAX]> animTable;
	std::string animName;

protected:
	const char (&keyData)[256];
	const char (&keyDataOld)[256];

	virtual void SetMove(void);

	// �����͗p�������֐�
	void SetMapCheck(void);
	// �̗�
	void LifeDraw(int life);
	int PlayerLife = 2;


	bool AddAnim(std::string animName, int id_x, int id_y, int Frame, int inv);

	VECTOR2 pos = { 0,0 };			//	�`��ʒu
	VECTOR2 drawOffset;
	int speed;						//	��߰��
	DIR Dir = DIR_NON;
	unsigned int animCnt = 0;

	bool BeatenFlag = false;
	int BeatenCnt = 0;

	int EnemyCount = 0;
	int OneUp = 0;


	// �㉺���E
	CHIP_TYPE cpTypeU, cpTypeD, cpTypeR, cpTypeL;

	// �㉺���E�̃`�F�b�N
	CHIP_TYPE cpTypeUD, cpTypeDU, cpTypeRL, cpTypeLR;

	// ����@�E��@�����@�E��
	CHIP_TYPE cpTypeUL, cpTypeUR, cpTypeDL, cpTypeDR;

	// �㉺�̎΂ߍ��E
	CHIP_TYPE cpTypeURU, cpTypeULU, cpTypeDRD, cpTypeDLD;

	// ���E�̎΂ߏ㉺
	CHIP_TYPE cpTypeURR, cpTypeDRR, cpTypeULL, cpTypeDLL;

	// ���S
	CHIP_TYPE cpTypeC;

	VECTOR2 attackPos = { 0,0 };

};

