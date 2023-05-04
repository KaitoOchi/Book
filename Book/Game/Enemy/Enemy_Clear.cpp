#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
}

Enemy_Clear::Enemy_Clear()
{

}
Enemy_Clear::~Enemy_Clear()
{
	m_clearFlag = true;
}
bool Enemy_Clear::Start()
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

	// パス移動
	m_point = &m_pointList[0];

	return true;
}
void Enemy_Clear::Update()
{
	// 閃光弾に当たった
	if (Enemy::GetHitFlushBullet() == true) {
		m_ActState = CONFUSION;
	}
	// 音爆弾を使用した
	if (Enemy::GetHitSoundBullet() == true) {
		m_ActState = LISTEN;
	}

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
		// 閃光弾に当たった
	case CONFUSION:
		Update_OnConfusion();
		break;
		// 音爆弾を使用したとき
	case LISTEN:
		UpDate_OnListen();
		// 捕獲
	case CATCH:
		Update_OnCatch();

		break;
	}
	Animation();
	ClearChange();
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);

	m_enemyRender.Update();	// 更新
}
void Enemy_Clear::Update_OnCraw()
{
	Enemy::Act_Craw();				// 巡回行動

	// 視野角にプレイヤーがいるとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnTracking()
{
	Enemy::Act_Tracking();			// 追跡行動

	// 視野角にプレイヤーがいないとき
	if (Enemy::Act_SeachPlayer() == false) {
		Enemy::Act_MissingPlayer();
		m_ActState = BACKBASEDON;
	}

	// プレイヤーを捕まえたとき
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Act_Called();

	// 視野角にプレイヤーがいるとき
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();					// 追跡行動からの切り替え
	m_ActState = CRAW;
}

void Enemy_Clear::Update_OnConfusion()
{
	// 閃光弾に当たったとき

	Enemy::Act_HitFlashBullet();

	// 硬直が解けているとき
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Clear::UpDate_OnListen()
{
	// 音爆弾を使ったとき

	Enemy::Act_HitSoundBullet();

	// 効果が終了したとき
	if (Enemy::GetHitSoundBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Clear::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();
	m_ActState = CRAW;
}

void Enemy_Clear::ClearChange()

{
	if (m_SetActState != m_ActState&&m_clearChangeTime>=0.0f)
	{
		m_clearChangeTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//巡回中なら透明化にする
	if (m_ActState == CRAW)
	{
		m_clearFlag = true;
		m_SetActState = CRAW;
		m_clearChangeTime = 0.0f;
	}
	//追跡中なら透明化を解除
	else if (m_ActState == TRACKING)
	{
		m_clearFlag = false;
		m_SetActState = TRACKING;
		m_clearChangeTime = 1.0f;
	
	}
}


void Enemy_Clear::Animation()
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
void Enemy_Clear::Render(RenderContext& rc)
{
	//透明化解除
	if (m_clearFlag == false)
	{
		m_enemyRender.Draw(rc);
	}
	
}