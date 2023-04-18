#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include "Title.h"
#include "Debug.h"


/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Book"));

	NewGO<Title>(0, "title");
	//NewGO<Game>(0, "game");
	//NewGO<Debug>(0, "debug");

	RenderingEngine::GetInstance()->Init();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		BookEngine::GetInstance()->Execute();

	}

	BookEngine::DeleteInstance();

	return 0;
}

