#include "stdafx.h"
#include "Enemy_Search.h"

#include "GameManager.h"
namespace
{
	const float KEEPTIME = 2.0f;
	
	const float ANGLE = 45.0f;
	const float RODADD = 0.01f;

	const float SEARCHPLAYER_TIMER = 7.0f;
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

	// モデルの読み込み
	m_enemyRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);

	return true;
}
void Enemy_Search::Update()
{
	// 行動できるか調べる
	if (GetActiveFlag() == true)
	{
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// イベント後の処理
	if (GetNotDrawFlag() == true) {
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// 閃光弾に当たったとき
	if (GetHitFlashBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Dizzy);
	}

	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
	case m_ActionState_Search:
		// 索敵
		Update_OnSearch();
		break;
	case m_ActionState_Call_AroundEnemy:
		// 周りのエネミーを呼ぶ
		Update_OnCallAroundEnemy();
		break;
	case m_ActionState_Search_MissingPlayer:
		// 見失ったプレイヤーを探す
		Update_OnMissingPlayer();
		break;
	case m_ActionState_Dizzy:
		// 閃光弾を使用されたとき
		Update_OnDizzy();
		break;
	}

	Enemy::PlayAnimation();							// アニメーション

	Enemy::SpotLight_Serch(m_rotation, m_position);	// スポットライト
	Enemy::Action_SeachPlayer();					// 索敵

	m_characterController.SetPosition(m_position);

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	Rotaition();

	if (GetTrackingPlayerFlag() == true) {
		SetEffectDrawFlag(false, 2);
		SetEnemyActionState(m_ActionState_Call_AroundEnemy);
	}
}

void Enemy_Search::Update_OnCallAroundEnemy()
{
	// 周りにいるエネミーを呼ぶ
	Enemy::Action_CallAroundEnemy();
}

void Enemy_Search::Update_OnDizzy()
{
	// 閃光弾に当たったとき
	Enemy::Action_HitFlashBullet();
}

void Enemy_Search::Update_OnMissingPlayer()
{
	// プレイヤーを見失った時
	Enemy::Action_SearchMissingPlayer();
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
		m_rotation.AddRotationY(RODADD);
		break;
	case false:
		m_rotation.AddRotationY(-RODADD);
		break;
	default:
		break;
	}
	
	SetEnemyAnimationState(m_enAnimationState_Idle);
}
void Enemy_Search::Render(RenderContext& rc)
{
	if (GetNotDrawFlag() == false &&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}