#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::Title()
{

}
Title::~Title()
{

}
bool Title::Start()
{
	m_titleRender.Init("Assets/sprite/title.DDS", 1920, 1080);
	return true;
}
void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0,"game");
		DeleteGO(this);
	}
	m_titleRender.Update();
}
void Title::Render(RenderContext &rc)
{
	m_titleRender.Draw(rc);
}