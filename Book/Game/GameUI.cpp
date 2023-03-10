#include "stdafx.h"
#include "GameUI.h"


GameUI::GameUI()
{

}

GameUI::~GameUI()
{

}

bool GameUI::Start()
{
	m_gageSpriteRender.Init("Assets/sprite/UI/gage.dds", 280.0f, 820.0f);
	m_gageSpriteRender.SetPosition(Vector3(-400.0f, 100.0f, 0.0f));
	m_gageSpriteRender.SetScale(Vector3(0.5f, 0.5f, 0.5f));
	m_gageSpriteRender.Update();

	//タイムの設定
	m_timeFontRender.SetPosition(Vector3(0.0f, 500.0f, 0.0f));
	m_timeFontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_timeFontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_timeFontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	return true;
}

void GameUI::Update()
{
	Time();
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
	m_gageSpriteRender.Draw(rc);
	m_timeFontRender.Draw(rc);
}