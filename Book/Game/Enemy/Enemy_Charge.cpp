#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

namespace 
{
	const float		STOP_TIMER = 1.5f;				// 溜め時間
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
	LoadAnimation();

	// モデルの読み込み
	m_enemyRender.Init("Assets/modelData/enemy/enemy_charge.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// パスの初期座標を渡す
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Charge::Update()
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

	// プレイヤーを捕まえたとき
	if (GetEnemyActionState() == m_ActionState_CatchPlayer) {
		// 待機状態にする
		SetEnemyAnimationState(m_enAnimationState_Idle);
		return;
	}

	if (GetHearedSoundBulletFlag() == true && GetHitFlashBulletFlag() == true) {
		// 追跡を優先する
		SetHearedSoundBulletFlag(false);
	}

	if (GetEnemyActionState() == m_ActionState_Tracking && GetHearedSoundBulletFlag() == true) {
		// 閃光弾を優先する
		SetHearedSoundBulletFlag(false);
	}

	// 閃光弾に当たった
	if (GetHitFlashBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Dizzy);
	}
	// 音爆弾を使用した
	if (GetHearedSoundBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Listen);
	}

	switch (GetEnemyActionState()) {
		// 一定の場所を巡回する
	case m_ActionState_Craw:
		Update_OnCraw();
		break;
		// プレイヤーに向かって突進する
	case m_ActionState_Charge:
		Update_OnCharge();
		break;
		// 突進終了する
	case m_ActionState_ChargeEnd:
		Update_OnChargeEnd();
		break;
		// 見失ったプレイヤーを探す
	case m_ActionState_Search_MissingPlayer:
		Update_OnSearchMissingPlayer();
		break;
		// Searchの座標近くに向かう
	case m_ActionState_Called:
		Update_OnCalled();
		break;
		// 元のパスに戻る
	case m_ActionState_BackBasedOn:
		Update_OnBackBasedOn();
		break;
		// 混乱
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		break;
		// 音が聞こえた場所に向かう
	case m_ActionState_Listen:
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();							// アニメーション

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);	// スポットライト
	Enemy::Action_SeachPlayer();						// 索敵

	m_enemyRender.Update();
}

void Enemy_Charge::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_CrawPath();
}

void Enemy_Charge::Update_OnCharge()
{
	if (Enemy::Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_ChargeStart(STOP_TIMER);
}

void Enemy_Charge::Update_OnChargeEnd()
{
	Enemy::Action_ChargeEnd();
}

void Enemy_Charge::Update_OnSearchMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	Enemy::Action_MissingPlayer();
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_setPos);
}

void Enemy_Charge::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Charge::UpDate_OnListen()
{
	Enemy::Action_GoLocationListenSound(m_itemPos);
}

void Enemy_Charge::Update_OnCatch()
{
	Enemy::Action_CatchPlayer();
}

void Enemy_Charge::Render(RenderContext& rc)
{
	if (GetNotDrawFlag() == false&&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}