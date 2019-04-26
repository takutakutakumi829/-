#pragma once
#include <array>
#include "Obj.h"
class Stone :
	public Obj
{
public:
	Stone(VECTOR2 chipOffSet);
	~Stone();

	bool Update(void);

private:
	void SetMove(void);

	bool InitAnim(void);

	bool checkPlayer = false;

	int DownTime = 0;
	int stone[2] = { 0 };

	char KeyData[256] = { 0 };
	char KeyDataOld[256] = { 0 };


};

