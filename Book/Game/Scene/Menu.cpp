#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{

}

Menu::~Menu()
{

}

bool Menu::Start()
{
	m_fontRender.SetText(L"Game Start\n\nSousa\n\nSetting");
	m_cursorFontRender.SetText(L">");

	return true;
}

void Menu::Update()
{
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor--;
	}
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor++;
	}

	//”ÍˆÍŠO‚É‚Í‚İo‚³‚È‚¢‚æ‚¤‚É‚·‚é
	m_cursor = min(max(m_cursor, 0), 2);

	m_cursorFontRender.SetPosition(Vector3(-50.0f, 50.0f * -m_cursor, 0.0f));
}

void Menu::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
	m_cursorFontRender.Draw(rc);
}