#include "stdafx.h"
#include "Enemy_Normal.h"

#include "GameManager.h"
namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
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
	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true, 2);
	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	Enemy::Start();

	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	switch (m_ActState) {
		// 巡回
	case CRAW:
		Update_OnCraw();
		break;
		// 追跡
	case TRACKING:
		Update_OnTracking();
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
		// 捕獲
	case CATCH:
		Update_OnCatch();
		break;
	}

	Animation();					// アニメーション

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);

	m_enemyRender.Update();	// 更新
}

void Enemy_Normal::Update_OnCraw()
{
	Enemy::Act_Craw();				// 巡回行動

	// 視野角にプレイヤーがいるとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = TRACKING;
	}

	// プレイヤーを捕まえたとき
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Normal::Update_OnTracking()
{
	Enemy::Act_Tracking();			// 追跡行動

	// 視野角にプレイヤーがいないとき
	if (Enemy::Act_SeachPlayer() == false) {
		m_ActState = BACKBASEDON;
	}

	// プレイヤーを捕まえたとき
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Act_Called();

	// 視野角にプレイヤーがいるとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();					// 追跡行動からの切り替え
	m_ActState = CRAW;
}

void Enemy_Normal::Update_OnConfusion()
{
	Enemy::Act_HitFlashBullet();		// 閃光弾に当たったときの処理

	// 硬直が解けているとき
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Normal::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();

	m_ActState = CRAW;
}

void Enemy_Normal::Animation()
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

void Enemy_Normal::Render(RenderContext& rc)
{
	// 描画
	m_enemyRender.Draw(rc);

	if (Enemy::Act_CatchPlayer() == true) {
		m_fontRender.Draw(rc);
	}
}