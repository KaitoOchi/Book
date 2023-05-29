#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include "Title.h"
#include "Fade.h"
#include "Logo.h"

#include "Event.h"
#include "Opening.h"


/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Carta Gatto"));

	//NewGO<Logo>(10, "logo");
	
	NewGO<Title>(0, "title");
	//NewGO<Game>(0, "game");

	//NewGO<Event>(0, "event");
	//NewGO<Opening>(0, "opening");

	NewGO<Fade>(9, "fade");

	RenderingEngine::GetInstance()->Init();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		BookEngine::GetInstance()->Execute();
	}

	BookEngine::DeleteInstance();

	return 0;
}

