#include "Dxlib.h"	// DxLibײ���؂��g�p����
//#include "main.h"
#include "GameTask.h"

using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameTask::Create();


	// ٰ��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		GameTask::GetInstance().Update();
	}
	GameTask::GetInstance().Destroy();
	DxLib_End();	// DXײ���؂̏I������
	return 0;
}
