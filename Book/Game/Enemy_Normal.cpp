#include "stdafx.h"
#include "Enemy_Normal.h"

namespace
{
	const float		LINEAR_COMPLETION = 0.5f;				// 線形補完のフレーム数
}

Enemy_Normal::Enemy_Normal()
{
}

Enemy_Normal::~Enemy_Normal()
{
}

bool Enemy_Normal::Start()
{
	// アニメーションの読み込み
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Attack].Load("Assets/animData/enemy/attack1.tka");
	m_animationClips[m_enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	m_animationClips[m_enAnimationClip_Damege].SetLoopFlag(false);

	// モデルの読み込み
	m_NormalModelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ);
	//m_NormalModelRender.Init("Assets/modelData/unityChan.tkm");
	m_NormalModelRender.SetScale(m_scale);
	m_NormalModelRender.SetPosition(m_position);
	m_NormalModelRender.SetRotation(m_rotation);

	Enemy::Start();

	return true;
}

void Enemy_Normal::Update()
{
	// 更新
	//HeadToDestination();
	m_NormalModelRender.Update();
}

void Enemy_Normal::HeadToDestination()
{
	// プレイヤーを発見したとき
	if (Enemy::SeachPlayer() == true) {
		// 追跡に移行する
		m_enEnemyActState = m_enEnemyActState_Tracking;

		// プレイヤーに向かう経路を作成


		// プレイヤーを確保したとき
		if (Enemy::CatchPlayer() == true) {
		}
	}
	else {
		// 巡回に移行する
		m_enEnemyActState = m_enEnemyActState_Craw;

		// 直近のパスを探す
		// パスに戻ったときにfalseにする
	}
}

void Enemy_Normal::Animation()
{
	// アニメーションのステート
	switch (m_enEnemyAnimationState) {
		// 待機
	case Enemy::m_enEnemyAnimationState_Idle:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// 歩く
	case Enemy::m_enEnemyAnimationState_Walk:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// 走る
	case Enemy::m_enEnemyAnimationState_Run:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// 攻撃
	case Enemy::m_enEnemyAnimationState_Attack:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// 被弾
	case Enemy::m_enEnemyAnimationState_Damege:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// 描画
	m_NormalModelRender.Draw(rc);
}