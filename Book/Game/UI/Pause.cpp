#include "stdafx.h"
#include "Pause.h"

#include "GameManager.h"
#include "Game.h"
#include "Title.h"

namespace
{
	const Vector3	CURSOR_POS_MENU[3] = { Vector3(-250.0f, 125.0f, 0.0f),
										Vector3(-250.0f, -110.0f, 0.0f),
										Vector3(-250.0f, -345.0f, 0.0f) };	//メニュー画面のカーソル座標
	const int		CURSOR_MIN = 0;											//カーソルの最小値。
	const int		CURSOR_MAX = 2;											//カーソルの最大値。
	const float		CURSOR_MOVESPEED = -235.0f;								//カーソル速度。
	const float		CURSOR_TIMER_SPEED = 4.0f;								//カーソルタイマーの速度。
	const float		CURSOR_TIMER_MAX = 1.0f;								//カーソルタイマーの最大値。
	const float		FADEOUT_TIME = 0.25f;									//フェードアウトの時間。
	const float		FADE_SPEED = 4.0f;										//フェード速度。
}

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
	m_cursorSpriteRender.SetPosition(Vector3(-250.0f, 125.0f + (m_cursor * CURSOR_MOVESPEED), 0.0f));
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
			m_game->StopWallEffect();
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
	//カーソルの移動中は、入力を受け付けない
	if (m_cursorTimer < CURSOR_TIMER_MAX) {
		CursorMove();
		return;
	}

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

		m_pauseState = enState_FadeIn;
	}

	//上ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;

		int cursor = m_cursor;
		m_cursor = min(max(m_cursor, CURSOR_MIN), CURSOR_MAX);

		if (m_cursor == cursor) {
			//選択音を出す
			PlaySE(2);
			m_cursorTimer = 0.0f;
		}
		m_nextCursor = 1;
	}
	//下ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;

		int cursor = m_cursor;
		m_cursor = min(max(m_cursor, CURSOR_MIN), CURSOR_MAX);

		if (m_cursor == cursor) {
			//選択音を出す
			PlaySE(2);
			m_cursorTimer = 0.0f;
		}
		m_nextCursor = -1;
	}
}

void Pause::CursorMove()
{
	m_cursorTimer += g_gameTime->GetFrameDeltaTime() * CURSOR_TIMER_SPEED;

	// -t^2 + 2t
	float rate = ((pow(m_cursorTimer, 2.0f) * -1.0f) + (2.0f * m_cursorTimer));
	rate = min(rate, CURSOR_TIMER_MAX);

	//カーソルを移動
	m_cursorPos.Lerp(rate, CURSOR_POS_MENU[m_cursor + m_nextCursor], CURSOR_POS_MENU[m_cursor]);
	m_cursorSpriteRender.SetPosition(m_cursorPos);
	m_cursorSpriteRender.Update();
}

void Pause::FadeIn()
{
	//タイマーを減少させる
	m_timer -= g_gameTime->GetFrameDeltaTime();

	SetSprite();

	if (m_timer < 0.0f) {
		//カーソルの初期化
		m_cursor = 0;
		m_cursorSpriteRender.SetPosition(Vector3(-250.0f, 125.0f + (m_cursor * CURSOR_MOVESPEED), 0.0f));
		m_cursorSpriteRender.Update();

		m_pauseState = enState_Game;
		m_isPause = false;

		//隙間エフェクトの再開
		m_game->PlayWallEffect();
	}
}

void Pause::FadeOut()
{
	//タイマーを増加させる
	m_timer += g_gameTime->GetFrameDeltaTime();

	SetSprite();

	if (m_timer > FADEOUT_TIME) {
		m_pauseState = enState_Pause;
	}
}

void Pause::SetSprite()
{
	//透明度を設定
	m_backGroundSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * (FADE_SPEED / 2.0f)));
	m_backGroundSpriteRender.Update();
	m_pauseSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * FADE_SPEED));
	m_pauseSpriteRender.Update();
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_timer * FADE_SPEED));
	m_cursorSpriteRender.Update();
}

void Pause::Render(RenderContext& rc)
{
	PauseUpdate();

	//ポーズ画面の描画
	if (m_isPause) {
		m_backGroundSpriteRender.Draw(rc);
		m_pauseSpriteRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);
	}
}