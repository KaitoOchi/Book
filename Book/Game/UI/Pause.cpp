#include "stdafx.h"
#include "Pause.h"

#include "GameManager.h"
#include "Game.h"
#include "Title.h"


Pause::Pause()
{

}

Pause::~Pause()
{

}

bool Pause::Start()
{
	m_game = FindGO<Game>("game");

	//背景画像の設定
	m_backGroundSpriteRender.Init("Assets/sprite/black.DDS", 1920.0f, 1080.0f);
	m_backGroundSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
	m_backGroundSpriteRender.Update();

	//ポーズメニュー画像の設定
	m_pauseSpriteRender.Init("Assets/sprite/UI/pause/pause_text.DDS", 517.0f, 845.0f);

	//カーソル画像の設定
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_cursorSpriteRender.SetPosition(Vector3(-250.0f, 125.0f + (m_cursor * -235.0f), 0.0f));
	m_cursorSpriteRender.Update();

	return true;
}

void Pause::PauseUpdate()
{
	switch (m_pauseState)
	{
	//ポーズ中なら
	case enState_Pause:
		PauseScreen();
		break;
	//ゲーム中なら
	case enState_Game:
		//ポーズボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonStart)) {
			m_isPause = true;
			m_pauseState = enState_FadeOut;
			GameObjectManager::GetInstance()->SetStop(true);
		}
		break;
	//フェードイン中なら
	case enState_FadeIn:
		FadeIn();
		break;
	//フェードアウト中なら
	case enState_FadeOut:
		FadeOut();
		break;
	}
}

void Pause::PauseScreen()
{
	//スタートボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonStart)) {
		GameObjectManager::GetInstance()->SetStop(false);
		m_pauseState = enState_FadeIn;

		//キャンセル音を出す
		PlaySE(0);
	}

	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)) {

		GameObjectManager::GetInstance()->SetStop(false);

		switch (m_cursor)
		{
		case 0:
			m_pauseState = enState_FadeIn;
			break;
		case 1:
			m_game->GameDelete(1);
			m_game->NotifyGameBack();
			break;
		case 2:
			m_game->GameDelete(2);
			m_game->NotifyGameBack();
			break;
		}

		//決定音を出す
		PlaySE(1);
	}

	//上ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;

		int cursor = m_cursor;
		m_cursor = min(max(m_cursor, 0), 2);

		if (m_cursor == cursor) {
			//選択音を出す
			PlaySE(2);
		}
	}
	//下ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;

		int cursor = m_cursor;
		m_cursor = min(max(m_cursor, 0), 2);

		if (m_cursor == cursor) {
			//選択音を出す
			PlaySE(2);
		}
	}

	m_cursorSpriteRender.SetPosition(Vector3(-250.0f, 125.0f + (m_cursor * -235.0f), 0.0f));
	m_cursorSpriteRender.Update();
}

void Pause::FadeIn()
{
	//タイマーを減少させる
	m_timer -= g_gameTime->GetFrameDeltaTime();

	SetSprite();

	if (m_timer < 0.0f) {
		m_pauseState = enState_Game;
		m_isPause = false;
	}
}

void Pause::FadeOut()
{
	//タイマーを増加させる
	m_timer += g_gameTime->GetFrameDeltaTime();

	SetSprite();

	if (m_timer > 0.25f) {
		m_pauseState = enState_Pause;
	}
}

void Pause::SetSprite()
{
	//透明度を設定
	m_backGroundSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * 2.0f));
	m_backGroundSpriteRender.Update();
	m_pauseSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * 4.0f));
	m_pauseSpriteRender.Update();
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * 4.0f));
	m_cursorSpriteRender.Update();
}

void Pause::Render(RenderContext& rc)
{
	PauseUpdate();

	if (m_isPause) {
		m_backGroundSpriteRender.Draw(rc);
		m_pauseSpriteRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);
	}
}