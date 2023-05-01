#include "stdafx.h"
#include "Enemy_Charge.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
	const float		STOP_TIMER = 3.0f;				// 溜め時間
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
	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true);
	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	Enemy::Start();

	// パス移動
	m_point = &m_pointList[0];

	//// 視野を作成
	//Enemy::SpotLight_New(m_position, 1);

	return true;
}

void Enemy_Charge::Update()
{
	Enemy::SearchPass(CRAW);

	switch (m_ActState) {
		// 巡回
	case CRAW:
		Update_OnCraw();
		break;
		// 突進
	case CHARGE:
		Update_OnCharge();
		break;
		// 呼ばれたとき
	case CALLED:
		Update_OnCalled();
		break;
		// 巡回状態に戻る
	case BACKBASEDON:
		Update_OnBackBasedOn();
		break;
		// 錯乱
	case CONFUSION:
		Update_OnConfusion();
		break;
		// 何もしない
	case NOOP:
		return;
		break;
	}

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

void Enemy_Charge::Update_OnCraw()
{
	// 巡回

	Enemy::Act_Craw();					// 巡回行動

	// 視野角にプレイヤーがいるとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = CHARGE;
	}

	// 閃光弾が当たったとき
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
}

void Enemy_Charge::Update_OnCharge()
{
	// 突進

	Enemy::Act_Charge(STOP_TIMER);		// 突進攻撃
										// 関数内で巡回状態に戻る処理を記述

	// 閃光弾が当たったとき
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	// 突進⇒巡回への切り替え
	Enemy::Act_Loss();					// 追跡行動からの切り替え
	m_ActState = CRAW;
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Act_Called();

	// 視野角にプレイヤーがいるとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = CHARGE;
	}
}

void Enemy_Charge::Update_OnConfusion()
{
	// 錯乱

	Enemy::Act_HitFlashBullet();		// 閃光弾に当たったときの処理

	// 閃光弾に当たっていないとき
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
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
	m_enemyRender.Draw(rc);
}