#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include "Title.h"
#include "Debug.h"
#include "Fade.h"
#include "Result.h"


/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Book"));

	NewGO<Title>(0, "title");
	//NewGO<Game>(0, "game");
	//NewGO<Debug>(0, "debug");
	//NewGO<Result>(0, "result");
	NewGO<Fade>(2, "fade");

	RenderingEngine::GetInstance()->Init();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		BookEngine::GetInstance()->Execute();
	}

	BookEngine::DeleteInstance();

	return 0;
}

