#include "stdafx.h"
#include "Game.h"


Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_modelRender.SetPosition(Vector3::Zero);
	m_modelRender.SetRotation(Quaternion::Identity);
	m_modelRender.SetScale(Vector3::One);
	m_modelRender.Update();

	m_spriteRender.Init("Assets/sprite/playerLeft.dds", 128.0f, 100.0f);
	m_spriteRender.SetPosition(Vector3(500.0f, 0.0f, 0.0f));
	m_spriteRender.Update();

	m_fontRender.SetText(L"‚ ‚¢‚¤‚¦‚¨");
	m_fontRender.SetPosition(Vector3(-500.0f, 0.0f, 0.0f));

	return true;
}

void Game::Update()
{
	if (g_pad[0]->GetKeyUp(enButtonA)) {
		m_position.x *= -1;
		m_spriteRender.SetPosition(m_position);
		m_spriteRender.Update();
	}
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_spriteRender.Draw(rc);
	m_fontRender.Draw(rc);
}
