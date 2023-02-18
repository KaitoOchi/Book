#include "stdafx.h"
#include "system/system.h"


/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		nsBookEngine::BookEngine::GetInstance()->Execute();
	}

	nsBookEngine::BookEngine::DeleteInstance();

	return 0;
}

