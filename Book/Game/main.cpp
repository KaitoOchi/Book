#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include "Title.h"
#include "Fade.h"
#include "Logo.h"


/// <summary>
/// メイン関数。
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Carta Gatto"));

	//NewGO<Logo>(10, "logo");
	
	NewGO<Title>(0, "title");
	//NewGO<Game>(0, "game");

	NewGO<Fade>(9, "fade");

	RenderingEngine::GetInstance()->Init();

	//ゲームループ。
	while (DispatchWindowMessage())
	{
		BookEngine::GetInstance()->Execute();
	}

	BookEngine::DeleteInstance();

	return 0;
}

