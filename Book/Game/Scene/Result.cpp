#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "Game.h"

namespace
{
	const float CAN_INPUT_GAMECLEAR = 5.0f;		//ゲームクリア時の入力可能時間
	const float CAN_INPUT_GAMEOVER = 2.0f;		//ゲームオーバー時の入力可能時間
}

Result::Result()
{

}

Result::~Result()
{

}

bool Result::Start()
{
	switch (m_resultState)
	{
	case enState_GameClear:
		//背景画像の設定
		m_backGroundSpriteRender.Init("Assets/sprite/UI/result/base.DDS", 1920.0f, 1080.0f);
		//タイトル画像の設定
		m_stateSpriteRender.Init("Assets/sprite/UI/result/missionComplete!.DDS", 1153.0f, 168.0f);
		m_stateSpriteRender.SetPosition(Vector3(-300.0f, 400.0f, 0.0f));
		//詳細画像の設定
		m_explainSpriteRender[0].Init("Assets/sprite/UI/result/light.DDS", 879.0f, 1024.0f);
		m_explainSpriteRender[0].SetPosition(Vector3(300.0f, 0.0f, 0.0f));
		m_explainSpriteRender[1].Init("Assets/sprite/UI/result/rank.DDS", 879.0f, 1024.0f);
		m_explainSpriteRender[1].SetPosition(Vector3(300.0f, 0.0f, 0.0f));
		//待機時間の設定
		m_canInputTime = CAN_INPUT_GAMECLEAR;
		break;

	case enState_GameOver:
		//背景画像の設定
		m_backGroundSpriteRender.Init("Assets/sprite/UI/gameOver/base.DDS", 1920.0f, 1080.0f);
		//タイトル画像の設定
		m_stateSpriteRender.Init("Assets/sprite/UI/gameOver/gameOver.DDS", 989.0f, 202.0f);
		m_stateSpriteRender.SetPosition(Vector3(-350.0f, 380.0f, 0.0f));
		//詳細画像の設定
		m_explainSpriteRender[0].Init("Assets/sprite/UI/gameOver/retry.DDS", 317.0f, 149.0f);
		m_explainSpriteRender[0].SetPosition(Vector3(-600.0f, 90.0f, 0.0f));
		m_explainSpriteRender[0].SetPivot(Vector2(0.0f, 0.5f));
		m_explainSpriteRender[1].Init("Assets/sprite/UI/gameOver/giveup.DDS", 481.0f, 152.0f);
		m_explainSpriteRender[1].SetPosition(Vector3(-600.0f, -150.0f, 0.0f));
		m_explainSpriteRender[1].SetPivot(Vector2(0.0f, 0.5f));
		//待機時間の設定
		m_canInputTime = CAN_INPUT_GAMEOVER;
		break;
	}

	m_stateSpriteRender.SetScale(Vector3(0.9f, 0.9f, 0.0f));
	m_stateSpriteRender.Update();

	for (int i = 0; i < 2; i++) {
		m_explainSpriteRender[i].SetScale(Vector3(0.9f, 0.9f, 0.0f));
		m_explainSpriteRender[i].Update();
	}

	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::Update()
{
	if (!IsEndAnimation()) {
		return;
	}

	//フェードの待機時間
	if (m_isWaitFadeOut) {

		//フェードをし終えたら
		if (!m_fade->IsFade()) {
			//シーン遷移
			if (m_resultState == enState_GameClear) {
				NewGO<Title>(0, "title");
			}
			else if (m_cursor == 0) {
				NewGO<Game>(0, "game");
			}
			else {
				NewGO<Title>(0, "title");
			}
			DeleteGO(this);
		}

		return;
	}

	Input();

	switch (m_resultState)
	{
		//ゲームクリアなら
	case enState_GameClear:
		GameClear();
		break;

		//ゲームオーバーなら
	case enState_GameOver:
		GameOver();
		break;
	}
}

void Result::Input()
{
	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//フェードを始める
		m_isWaitFadeOut = true;
		m_fade->StartFadeOut();
	}

	//十字ボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;
	}
	else if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;
	}
	m_cursor = min(max(m_cursor, 0), 1);
}

void Result::GameClear()
{

}

void Result::GameOver()
{
	//時間の処理
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);
	m_alpha *= 3.0f;
	m_alpha = max(m_alpha, 1.0f);

	m_cursorSpriteRender.SetPosition(Vector3(-650.0f, 90.0f + (m_cursor * -240.0f), 0.0f));
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_cursorSpriteRender.Update();
}

void Result::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
	m_stateSpriteRender.Draw(rc);

	if (!IsEndAnimation()) {
		return;
	}

	//ゲームクリアなら
	if (m_resultState == enState_GameClear) {

	}
	//ゲームオーバーなら
	else {
		m_cursorSpriteRender.Draw(rc);
	}

	m_explainSpriteRender[0].Draw(rc);
	m_explainSpriteRender[1].Draw(rc);
}
