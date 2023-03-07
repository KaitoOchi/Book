#include "stdafx.h"
#include "system/system.h"

#include "Game.h"
#include "Debug.h"


/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//NewGO<Game>(0, "game");
	NewGO<Debug>(0, "debug");

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		BookEngine::GetInstance()->Execute();

	}

	BookEngine::DeleteInstance();

	return 0;
}

