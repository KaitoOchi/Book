#include "stdafx.h"
#include "Enemy_Normal.h"

namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
	const float		MOVING_DISTANCE = 500.0f;		// 移動量
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

	// パスの設定(初期:横移動)
	m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
	m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });

	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	// 更新
	Act();						// 行動パターン
	Animation();				// アニメーション

	m_NormalModelRender.SetPosition(m_position);
	m_NormalModelRender.SetRotation(m_rotation);
	m_characterController.SetPosition(m_position);

	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());
	m_NormalModelRender.Update();
}

void Enemy_Normal::Act()
{
	// プレイヤーを見つけたとき
	if (Enemy::SeachPlayer() == true) {
		Enemy::Act_Tracking();
	}
	// プレイヤーを見つけていないとき
	else {
		Enemy::Act_Craw();		// 巡回行動
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