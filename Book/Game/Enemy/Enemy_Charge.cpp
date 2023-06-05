#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// 線形補完のフレーム数
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
	Animation();

	// モデルの読み込み
	m_enemyRender.Init("Assets/modelData/enemy/enemy_charge.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// パス移動
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Charge::Update()
{
	//行動できるか調べる
	if (m_activeFlag == true)
	{
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// 描画しないフラグがtrueのとき
	if (m_NotDrawFlag == true) {
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// プレイヤーを捕まえたとき
	if (m_ActState == m_ActState_CatchPlayer) {
		m_enAnimationState = m_enAnimationState_Idle;
		return;
	}

	if (m_HearedSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		m_HearedSoundBulletFlag = false;
	}

	if (m_ActState == m_ActState_Tracking && m_HearedSoundBulletFlag == true) {
		m_HearedSoundBulletFlag = false;
	}

	// 閃光弾に当たった
	if (m_HitFlashBulletFlag == true) {
		m_ActState = m_ActState_Dizzy;
	}
	// 音爆弾を使用した
	if (m_HearedSoundBulletFlag == true) {
		m_ActState = m_ActState_Listen;
	}

	switch (m_ActState) {
		// 巡回
	case m_ActState_Craw:
		Update_OnCraw();
		break;
		// 突進
	case m_ActState_Charge:
		Update_OnCharge();
		break;
		// 突進終了
	case m_ActState_ChargeEnd:
		Update_OnChargeEnd();
		break;
		// プレイヤーを探す
	case m_ActState_SEarch_MissingPlayer:
		Update_OnSearchMissingPlayer();
		break;
		// 呼ばれたとき
	case m_ActState_Called:
		Update_OnCalled();
		break;
		// 巡回状態に戻る
	case m_ActState_BackBasedOn:
		Update_OnBackBasedOn();
		break;
		// 閃光弾に当たった
	case m_ActState_Dizzy:
		Update_OnConfusion();
		break;
		// 音爆弾を使用したとき
	case m_ActState_Listen:
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();		// アニメーション

	// 更新
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	// スポットライト
	Enemy::SpotLight_Serch(m_rotation, m_position);
	// 視野角
	Enemy::Act_SeachPlayer();

	m_enemyRender.Update();
}

void Enemy_Charge::Update_OnCraw()
{
	// プレイヤーを捕まえたとき
	if (Act_CatchPlayer() == true) {
		m_ActState = m_ActState_CatchPlayer;
	}

	Enemy::Act_Craw();					// 巡回行動
}

void Enemy_Charge::Update_OnCharge()
{
	// プレイヤーを捕まえたとき
	if (Enemy::Act_CatchPlayer() == true) {
		m_ActState = m_ActState_CatchPlayer;
	}

	Enemy::Act_Charge(STOP_TIMER);		// 突進攻撃
										// 関数内で巡回状態に戻る処理を記述
}

void Enemy_Charge::Update_OnChargeEnd()
{
	Enemy::Act_ChargeEnd();		// 突進をやめる
}

void Enemy_Charge::Update_OnSearchMissingPlayer()
{
	// プレイヤーを探す
	Enemy::Act_SearchMissingPlayer();
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	// 突進⇒巡回への切り替え
	Enemy::Act_Loss();					// 追跡行動からの切り替え
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Act_GoLocationListenSound(m_setPos);
}

void Enemy_Charge::Update_OnConfusion()
{
	Enemy::Act_HitFlashBullet();
}

void Enemy_Charge::UpDate_OnListen()
{
	// 音爆弾を使ったとき
	Enemy::Act_GoLocationListenSound(m_itemPos);
}

void Enemy_Charge::Update_OnCatch()
{
	Enemy::Act_CatchPlayer();
}

void Enemy_Charge::Render(RenderContext& rc)
{
	if (m_NotDrawFlag == false&&
		m_activeFlag == false) {
		m_enemyRender.Draw(rc);
	}
}