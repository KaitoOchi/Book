#include "stdafx.h"
#include "Enemy_Search.h"

#include "GameManager.h"
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
	// アニメーションの読み込み
	Animation();

	m_enemyRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);

	m_ActState = SEARCH;	// 行動パターンを設定。基本がCRAWのため。

	return true;
}
void Enemy_Search::Update()
{
	Enemy::SearchPass(SEARCH);

	// 閃光弾に当たった
	if (Enemy::GetHitFlushBullet() == true) {
		m_ActState = CONFUSION;
	}
	// 音爆弾を使用した
	if (Enemy::GetHitSoundBullet() == true) {
		m_ActState = LISTEN;
	}

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

	m_enAnimationState = IDLE;

	// 更新
	m_enemyRender.SetRotation(m_rot);
	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	// 索敵
	Rotaition();

	// 視野角内にプレイヤーが存在するとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = CALL;
	}
}

void Enemy_Search::Update_OnCall()
{
	// 周りの敵を呼ぶ
	Enemy::Act_Call();

	// 視野角内にプレイヤーが存在しないとき
	if (Enemy::Act_SeachPlayer() == false) {
		m_ActState = CALLEND;
	}
}

void Enemy_Search::Update_OnCallEnd()
{
	Rotaition();
	Enemy::Act_CallEnd();

	// エネミーを元に戻す
	if (Enemy::Act_CallEnd() == true) {
		m_ActState = SEARCH;
	}
}

void Enemy_Search::Update_OnConfusion()
{
	// 錯乱
	Enemy::Act_HitFlashBullet();

	// 硬直が解けているとき
	if (Enemy::GetHitFlushBullet() == false) {
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