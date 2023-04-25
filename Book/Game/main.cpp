#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include "Title.h"
#include "Debug.h"
#include "Fade.h"
#include "Result.h"


/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Book"));

<<<<<<< HEAD
	//NewGO<Title>(0, "title");
	NewGO<Game>(0, "game");
	//NewGO<Debug>(0, "debug");
	//NewGO<Result>(0, "result");
	//NewGO<Fade>(2, "fade");
=======
	NewGO<Title>(0, "title");
	//NewGO<Game>(0, "game");
	//NewGO<Debug>(0, "debug");
	//NewGO<Result>(0, "result");
	NewGO<Fade>(2, "fade");
>>>>>>> 3f57ebccb9b1317a9dc34e9b55ac6665b9f837d2

	RenderingEngine::GetInstance()->Init();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		BookEngine::GetInstance()->Execute();
	}

	BookEngine::DeleteInstance();

	return 0;
}

