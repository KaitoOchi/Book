#include "stdafx.h"
#include "Enemy_Serch.h"
namespace
{
	const float KEEPTIME = 2.0f;
	const Vector3 LIGHTCOLOR(100.0f, 1.0f, 1.0f);
	const float ANGLE = 45.0f;
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
	m_spotLight.SetPosition(m_position);
	m_spotLight.SetColor(LIGHTCOLOR);
	m_spotLight.SetRange(500.0f);
	m_spotLight.SetAngle(ANGLE);
	Vector3 direction = Vector3::AxisY;
	//ƒ‰ƒCƒg‚Ì•ûŒüÝ’è
	m_spotLight.SetDirection(direction);
	m_spotLight.Update();
	return true;
}
void Enemy_Serch::Update()
{
	Rotaition();
	LightRotaition();
	m_position.z += 1;
	m_enemyRender.SetRotation(m_rot);
	m_enemyRender.SetPosition(m_position);
	m_spotLight.SetPosition(m_position);
	m_spotLight.Update();
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
		m_rot.AddRotationY(0.01f);
		break;
	case false:
		m_rot.AddRotationY(-0.01f);
		break;
	default:
		break;
	}
	Vector3 forward = Vector3(0.0f, -1.0f, 2.0f);
	m_rot.Apply(forward);
	m_spotLight.SetDirection(forward);
	//m_spotLight.SetDirection(Vector3(0.0f,-1.0f,0.0f));
}
void Enemy_Serch::LightRotaition()
{

}
void Enemy_Serch::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);
}