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
	//タイムの設定
	m_fontRender.SetPosition(Vector3(0.0f, 500.0f, 0.0f));
	m_fontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_fontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_fontRender.SetShadowParam(true, 2.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

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
	m_fontRender.SetText(debugText);
}

void GameUI::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
}