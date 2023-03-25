#include "stdafx.h"
#include "Enemy_Serch.h"
namespace
{
	const float KEEPTIME = 2.0f;
	
	const float ANGLE = 45.0f;
	const float RODADD = 0.01f;
}
Enemy_Serch::Enemy_Serch()
{

}
Enemy_Serch::~Enemy_Serch()
{

}
bool Enemy_Serch::Start()
{
	Enemy::Start();
	m_enemyRender.Init("Assets/modelData/unityChan.tkm");
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);
	Enemy::SpotLight_New(m_position);
	return true;
}
void Enemy_Serch::Update()
{
	Rotaition();
	m_enemyRender.SetRotation(m_rot);
	Enemy::SpotLight_Serch(m_rot,m_position);
	m_enemyRender.Update();
}
void Enemy_Serch::Rotaition()
{
	m_rotTime -= g_gameTime->GetFrameDeltaTime();
	if (m_rotTime < 0.0f)
	{
		m_rotTime = KEEPTIME;
		switch (m_rotState)
		{
		case true:
			m_rotState = false;
			break;
		case false:
			m_rotState = true;
			break;
		default:
			break;
		}
	}
	switch (m_rotState)
	{
	case true:
		m_rot.AddRotationY(RODADD);
		break;
	case false:
		m_rot.AddRotationY(-RODADD);
		break;
	default:
		break;
	}
	
	
}
void Enemy_Serch::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);
}