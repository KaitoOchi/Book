#include "stdafx.h"
#include "Player.h"


Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	m_spriteRender.Init("Assets/sprite/playerLeft.dds", 128.0f, 100.0f);
	m_spriteRender.SetPosition(Vector3(500.0f, 0.0f, 0.0f));
	m_spriteRender.Update();

	return true;
}

void Player::Update()
{
	if (g_pad[0]->GetKeyUp(enButtonA)) {
		m_position.x *= -1;
		m_spriteRender.SetPosition(m_position);
		m_spriteRender.Update();
	}
}

void Player::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}