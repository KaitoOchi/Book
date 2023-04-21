#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"

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
		m_backGroundSpriteRender.Init("Assets/sprite/UI/result/base.DDS", 1920.0f, 1080.0f);
		break;

	case enState_GameOver:
		m_backGroundSpriteRender.Init("Assets/sprite/UI/gameOver/all.DDS", 1920.0f, 1080.0f);
		break;
	}

	m_cursorSpriteRender.Init("Assets/sprite/UI/gameOver/tryangle.DDS", 131.0f, 135.0f);

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Result::Update()
{
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

	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void Result::GameClear()
{

}

void Result::GameOver()
{
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;
	}
	else if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;
	}

	m_cursor = min(max(m_cursor, 0), 1);

	//時間の処理
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);
	m_alpha *= 3.0f;
	m_alpha = max(m_alpha, 1.0f);

	m_cursorSpriteRender.SetPosition(Vector3(-730.0f, 90.0f + (m_cursor * -240.0f), 0.0f));
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_cursorSpriteRender.Update();
}

void Result::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
	m_cursorSpriteRender.Draw(rc);
}
