#include "stdafx.h"
#include "Enemy_Charge.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
	const float		STOP_TIMER = 5.0f;				// 溜め時間
}

Enemy_Charge::Enemy_Charge()
{
}

Enemy_Charge::~Enemy_Charge()
{
}

bool Enemy_Charge::Start()
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
	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ);
	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	Enemy::Start();

	// パス移動
	m_point = &m_pointList[0];

	// 視野を作成
	Enemy::SpotLight_New(m_position);

	return true;
}

void Enemy_Charge::Update()
{
	Act();			// 行動パターン
	Animation();	// アニメーション

	// 更新
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	// スポットライト
	Enemy::SpotLight_Serch(m_rotation, m_position);

	m_enemyRender.Update();
}

void Enemy_Charge::Act()
{
	Enemy::Act_HitFlashBullet();		// 閃光弾に当たったときの処理

	// プレイヤーを発見したとき
	if (Enemy::Act_SeachPlayer() == true) {

		Act_Charge(STOP_TIMER);		// 突進攻撃

		if (HitFlashBulletFlag == true || Enemy::Act_SeachPlayer() == false) {
			ChangeFlag = true;
		}
	}
	// プレイヤーを見失う　または　閃光弾がヒットしたとき
	else if (HitFlashBulletFlag == true || Enemy::Act_SeachPlayer() == false) {

		if (ChangeFlag == true) {
			Enemy::Act_Loss();			// 追跡行動からの切り替え
			ChangeFlag = false;			// 毎度初回だけ処理を実行する
		}
		else {
			Enemy::Act_Craw();			// 巡回行動
		}
	}
}

void Enemy_Charge::Animation()
{
	// アニメーションのステート
	switch (m_enEnemyAnimationState) {
		// 待機
	case Enemy::m_enEnemyAnimationState_Idle:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// 歩く
	case Enemy::m_enEnemyAnimationState_Walk:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// 走る
	case Enemy::m_enEnemyAnimationState_Run:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// 攻撃
	case Enemy::m_enEnemyAnimationState_Attack:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// 被弾
	case Enemy::m_enEnemyAnimationState_Damege:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy_Charge::Render(RenderContext& rc)
{
	// 描画
	m_enemyRender.Draw(rc);
}