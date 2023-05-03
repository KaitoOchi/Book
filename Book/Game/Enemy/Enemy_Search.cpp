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
	m_enemyRender.Init("Assets/modelData/unityChan.tkm", 0, 0, enModelUpAxisZ, true, true, 2);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);

	m_ActState = SEARCH;	// s“®ƒpƒ^[ƒ“‚ðÝ’èBŠî–{‚ªCRAW‚Ì‚½‚ßB

	return true;
}
void Enemy_Search::Update()
{
	Enemy::SearchPass(SEARCH);

	switch (m_ActState) {
	case SEARCH:
		Update_OnSearch();
		break;
	case CALL:
		Update_OnCall();
		break;
	case CALLEND:
		Update_OnCallEnd();
		break;
	case CONFUSION:
		Update_OnConfusion();
		break;
	}

	Enemy::SpotLight_Serch(m_rot, m_position);
	// XV
	m_enemyRender.SetRotation(m_rot);
	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	// õ“G
	Rotaition();

	// ‘MŒõ’e‚ª“–‚½‚Á‚½‚Æ‚«
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}

	// Ž‹–ìŠp“à‚ÉƒvƒŒƒCƒ„[‚ª‘¶Ý‚·‚é‚Æ‚«
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = CALL;
	}
}

void Enemy_Search::Update_OnCall()
{
	// Žü‚è‚Ì“G‚ðŒÄ‚Ô
	Enemy::Act_Call();

	// ‘MŒõ’e‚ª“–‚½‚Á‚½‚Æ‚«
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}

	// Ž‹–ìŠp“à‚ÉƒvƒŒƒCƒ„[‚ª‘¶Ý‚µ‚È‚¢‚Æ‚«
	if (Enemy::Act_SeachPlayer() == false) {
		m_ActState = CALLEND;
	}
}

void Enemy_Search::Update_OnCallEnd()
{
	Rotaition();
	Enemy::Act_CallEnd();

	// ƒGƒlƒ~[‚ðŒ³‚É–ß‚·
	if (Enemy::Act_CallEnd() == true) {
		m_ActState = SEARCH;
	}
}

void Enemy_Search::Update_OnConfusion()
{
	// ö—
	Enemy::Act_HitFlashBullet();

	// d’¼‚ª‰ð‚¯‚Ä‚¢‚é‚Æ‚«
	if (m_HitFlashBulletFlag == false) {
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
	//m_fontRender.Draw(rc);
}