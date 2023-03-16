#include "stdafx.h"
#include "GameUI.h"


namespace
{
	const Vector3	GAGE_SPRITE_POSITION = { -700.0f, 300.0f, 0.0f };	//ゲージ画像の位置
	const Vector3	TIME_FONT_POSITION = { -100.0f, 500.0f, 0.0f };		//タイムの位置
	const float		GAGE_MAX = 245.0f;									//ゲージの最大値
	const float		TIME_MAX = 180.0f;									//最大残り時間
}

GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_gage = GAGE_MAX;
	m_timer = TIME_MAX;

	//ゲージの枠画像の設定
	m_gageFrameSpriteRender.Init("Assets/sprite/UI/gageFrame_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, false);
	m_gageFrameSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageFrameSpriteRender.Update();

	//ゲージ画像の設定
	m_gageSpriteRender.Init("Assets/sprite/UI/gage_player.dds", 256.0f, 256.0f, AlphaBlendMode_Trans, true);
	m_gageSpriteRender.SetPosition(GAGE_SPRITE_POSITION);
	m_gageSpriteRender.Update();

	//タイムの設定
	m_timeFontRender.SetPosition(TIME_FONT_POSITION);
	m_timeFontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_timeFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_timeFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	return true;
}

void GameUI::Update()
{
	Time();

	//Debug
	// 実装が完了したら消してね
	//ゲージの量を変更する処理
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//減らす
		m_gage -= 5.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize = GAGE_MAX - m_gage;
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {
		//増やす
		m_gage += 5.0f;
		RenderingEngine::GetInstance()->GetSpriteCB().clipSize = GAGE_MAX - m_gage;
	}
}

void GameUI::Time()
{
	//時間を計測する
	m_timer -= g_gameTime->GetFrameDeltaTime();

	//分を計算
	int m = m_timer / 60;
	//秒を計算
	float s = (int)m_timer % 60;
	//ミリ秒を計算
	s += m_timer - (int)m_timer;

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"Time %d:%05.02f", m, s);
	m_timeFontRender.SetText(debugText);
}

void GameUI::Render(RenderContext& rc)
{
	m_gageFrameSpriteRender.Draw(rc);
	m_gageSpriteRender.Draw(rc);
	m_timeFontRender.Draw(rc);
}