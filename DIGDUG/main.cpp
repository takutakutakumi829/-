#include "Dxlib.h"	// DxLib×²ÌŞ×Ø‚ğg—p‚·‚é
//#include "main.h"
#include "GameTask.h"

using namespace std;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameTask::Create();


	// Ù°Ìß
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		GameTask::GetInstance().Update();
	}
	GameTask::GetInstance().Destroy();
	DxLib_End();	// DX×²ÌŞ×Ø‚ÌI—¹ˆ—
	return 0;
}
