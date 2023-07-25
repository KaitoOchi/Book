#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

Enemy_Clear::Enemy_Clear()
{
}

Enemy_Clear::~Enemy_Clear()
{
}

bool Enemy_Clear::Start()
{
	m_clearFlag = true;

	Enemy::LoadAnimation();

	// 通常のモデルを読み込む
	m_enemyRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);
	// 半透明描画用のモデルを読み込む
	m_clearModelRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_TranslucentEnemy);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	m_clearModelRender.SetScale(m_scale);
	m_clearModelRender.SetPosition(m_position);
	m_clearModelRender.SetRotation(m_rotation);

	// パスの初期座標を渡す
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Clear::Update()
{
	//行動できるか調べる
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

	// プレイヤーを捕まえたとき
	if (GetEnemyActionState() == m_ActionState_CatchPlayer) {
		SetEnemyAnimationState(m_enAnimationState_Idle);
		return;
	}

	if (GetHearedSoundBulletFlag() == true && GetHitFlashBulletFlag() == true) {
		// 閃光弾を優先する
		SetHearedSoundBulletFlag(false);
	}

	if (GetActiveFlag() == m_ActionState_Tracking && GetHearedSoundBulletFlag() == true) {
		// 追跡を優先する
		SetHearedSoundBulletFlag(false);
	}

	// 閃光弾に当たったとき
	if (GetHitFlashBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Dizzy);
	}
	// 音爆弾が使用されたとき
	if (GetHearedSoundBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Listen);
	}

	// 行動パターン
	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
		Update_OnCraw();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActionState_Tracking:
		Update_OnTracking();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	case m_ActionState_Move_MissingPositon:
		Update_OnMoveMissingPosition();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActionState_Search_MissingPlayer:
		Update_OnSearchMissingPlayer();
		m_clearModelRender.PlayAnimation(m_enAnimation_Loss, 1.0f);
		break;
	case m_ActionState_Called:
		Update_OnCalled();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	case m_ActionState_BackBasedOn:
		Update_OnBackBasedOn();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		m_clearModelRender.PlayAnimation(m_enAnimation_Dizzy, 1.0f);
		break;
	case m_ActionState_Listen:
		UpDate_OnListen();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	}

	Enemy::PlayAnimation();

	ClearChange();

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーを自身の座標と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_enemyRender.Update();

	m_clearModelRender.SetPosition(m_position);
	m_clearModelRender.SetRotation(m_rotation);
	m_clearModelRender.Update();
}

void Enemy_Clear::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_CrawPath();
}

void Enemy_Clear::Update_OnTracking()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_TrackingPlayer();
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_gatherPosition);
}

void Enemy_Clear::Update_OnMoveMissingPosition()
{
	Enemy::Action_MoveMissingPosition();
}

void Enemy_Clear::Update_OnSearchMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Action_MissingPlayer();
}

void Enemy_Clear::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Clear::UpDate_OnListen()
{
	Enemy::Action_GoLocationListenSound(m_itemPosition);
}

void Enemy_Clear::ClearChange()
{
	if (m_setActionState != GetEnemyActionState () && m_clearChangeTime >= 0.0f)
	{
		m_clearChangeTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	if (GetEnemyActionState() == m_ActionState_Craw)
	{
		// 透明化
		m_clearFlag = true;
		m_setActionState = m_ActionState_Craw;
		m_clearChangeTime = 0.0f;
	}
	else if (GetEnemyActionState() == m_ActionState_Tracking)
	{
		// 透明化解除
		m_clearFlag = false;
		m_setActionState = m_ActionState_Tracking;
		m_clearChangeTime = 1.0f;
	}
}

void Enemy_Clear::Render(RenderContext& rc)
{
	// 描画
	if (GetActiveFlag() == true)
	{
		Vector3 move{ 0.0f,-300.0f,0.0f };
		m_characterController.Execute(move, 1.0f);
		return;
	}

	if (GetNotDrawFlag() == true) {
		return;
	}

	// モデルの切り替え
	if (m_clearFlag) {
		m_clearModelRender.Draw(rc);
	}
	else {
		m_enemyRender.Draw(rc);
	}
}