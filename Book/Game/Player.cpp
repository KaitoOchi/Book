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
	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	return true;
}

void Player::Update()
{

}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}