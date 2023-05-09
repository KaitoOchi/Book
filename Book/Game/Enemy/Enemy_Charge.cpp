#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"

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
	// 閃光弾に当たった
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
	// 音爆弾を使用した
	if (m_HitSoundBulletFlag == true) {
		m_ActState = LISTEN;
	}

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
		// 閃光弾に当たった
	case CONFUSION:
		Update_OnConfusion();
		break;
		// 音爆弾を使用したとき
	case LISTEN:
		UpDate_OnListen();
		break;
		// 捕獲
	case CATCH:
		Update_OnCatch();
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
	// 巡回

	if (m_ChachPlayerFlag == true) {
		m_enAnimationState = IDLE;
		return;
	}

	Enemy::Act_Craw();					// 巡回行動

	// 視野角にプレイヤーがいるとき
	if (m_TrakingPlayerFlag == true) {
		m_ActState = CHARGE;
	}
}

void Enemy_Charge::Update_OnCharge()
{
	// 突進
	Enemy::Act_Charge(STOP_TIMER);		// 突進攻撃
										// 関数内で巡回状態に戻る処理を記述

	// プレイヤーを捕まえたとき
	if (Enemy::Act_CatchPlayer() == true) {
		m_ActState = CATCH;
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
	if (m_TrakingPlayerFlag == true) {
		m_ActState = CHARGE;
	}
}

void Enemy_Charge::Update_OnConfusion()
{
	// 閃光弾に当たったとき

	Enemy::Act_HitFlashBullet();

	// 閃光弾に当たっていないとき
	if (Enemy::GetHitFlushBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Charge::UpDate_OnListen()
{
	// 音爆弾を使ったとき

	// 効果が終了したとき
	if (Enemy::Act_HitSoundBullet() == true) {
		m_ActState = BACKBASEDON;
	}
	// 効果が無効だったとき
	else {
		m_ActState = CRAW;
	}
}

void Enemy_Charge::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();

	m_ActState = CRAW;
}

void Enemy_Charge::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);
}