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
	//ƒ^ƒCƒ€‚Ìİ’è
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
	//ŠÔ‚ğŒv‘ª‚·‚é
	m_timer -= g_gameTime->GetFrameDeltaTime();

	//•ª‚ğŒvZ
	int m = m_timer / 60;
	//•b‚ğŒvZ
	float s = (int)m_timer % 60;
	//ƒ~ƒŠ•b‚ğŒvZ
	s += m_timer - (int)m_timer;

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"Time %d:%05.02f", m, s);
	m_fontRender.SetText(debugText);
}

void GameUI::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
}