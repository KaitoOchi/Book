#include "stdafx.h"
#include "Enemy_Search.h"
namespace
{
	const float KEEPTIME = 2.0f;
	
	const float ANGLE = 45.0f;
	const float RODADD = 0.01f;
}
Enemy_Search::Enemy_Search()
{

}
Enemy_Search::~Enemy_Search()
{

}
bool Enemy_Search::Start()
{
	Enemy::Start();
	m_enemyRender.Init("Assets/modelData/unityChan.tkm", 0, 0, enModelUpAxisZ, true, true);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);
	Enemy::SpotLight_New(m_position);

	m_ActState = SEARCH;	// 行動パターンを設定。基本がCRAWのため。

	return true;
}
void Enemy_Search::Update()
{
	switch (m_ActState) {
	case SEARCH:
		Update_OnSearch();
		break;
	case CONFUSION:
		Update_OnConfusion();
		break;
	}

	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	// 索敵

	Rotaition();

	m_enemyRender.SetRotation(m_rot);
	Enemy::SpotLight_Serch(m_rot, m_position);

	// 閃光弾が当たったとき
	if (HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
}

void Enemy_Search::Update_OnConfusion()
{
	// 錯乱
	Enemy::Act_HitFlashBullet();

	// 硬直が解けているとき
	if (HitFlashBulletFlag == false) {
		m_ActState = SEARCH;
	}
}

void Enemy_Search::Rotaition()
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
void Enemy_Search::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);
}