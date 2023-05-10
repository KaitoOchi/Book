#include "stdafx.h"
#include "Enemy_Normal.h"

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
	Animation();

	// モデルの読み込み
	m_enemyRender.Init("Assets/modelData/enemy/enemy_normal.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	// 描画しないフラグがtrueのとき
	if (m_NotDrawFlag == true) {
		return;
	}
	// デフォルトに戻すフラグがtrueのとき
	if (m_ChangeDefaultFlag == true) {
		m_ActState = CRAW;
	}

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
		break;
		// 捕獲
	case CATCH:
		Update_OnCatch();
		break;
	}

	Enemy::PlayAnimation();		// アニメーション

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);

	// 視野角
	Enemy::Act_SeachPlayer();

	m_enemyRender.Update();	// 更新
}

void Enemy_Normal::Update_OnCraw()
{
	if (m_ChachPlayerFlag == true) {
		m_enAnimationState = IDLE;
		return;
	}

	Enemy::Act_Craw();				// 巡回行動

	// 視野角にプレイヤーがいるとき
	if (m_TrakingPlayerFlag == true) {
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
	if (m_TrakingPlayerFlag == false) {
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
	if (m_TrakingPlayerFlag == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Act_MissingPlayer();
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


void Enemy_Normal::UpDate_OnListen()
{
	// 音爆弾を使ったとき

	Enemy::Act_HitSoundBullet();

	// 効果が終了したとき
	if (Enemy::GetHitSoundBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Normal::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();

	m_ActState = CRAW;
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// 描画
	if (m_NotDrawFlag == false) {
		m_enemyRender.Draw(rc);
	}

	if (Enemy::Act_CatchPlayer() == true) {
		m_fontRender.Draw(rc);
	}
}