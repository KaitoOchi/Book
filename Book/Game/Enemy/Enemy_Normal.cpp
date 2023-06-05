#include "stdafx.h"
#include "Enemy_Normal.h"
#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"
namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
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

	// パスの初期座標を渡す
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	// 行動できるか調べる
	if (m_activeFlag == true)
	{
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
		m_position = move;
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// イベント後の処理
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
		// 閃光弾を優先する
		m_HearedSoundBulletFlag = false;
	}

	if (m_ActState == m_ActState_Tracking && m_HearedSoundBulletFlag == true) {
		// 追跡を優先する
		m_HearedSoundBulletFlag = false;
	}

	// 閃光弾に当たったとき
	if (m_HitFlashBulletFlag == true) {
		m_ActState = m_ActState_Dizzy;
	}
	// 音爆弾が使用されたとき
	if (m_HearedSoundBulletFlag == true) {
		m_ActState = m_ActState_Listen;
	}

	switch (m_ActState) {
	case m_ActState_Craw:
		// 指定された範囲の巡回
		Update_OnCraw();
		break;
	case m_ActState_Tracking:
		// プレイヤーを追跡する
		Update_OnTracking();
		break;
	case m_ActState_Move_MissingPositon:
		// プレイヤーを最後に見た座標まで移動する
		Update_OnMoveMissingPosition();
		break;
	case m_ActState_SEarch_MissingPlayer:
		// 見失ったプレイヤーを探す
		Update_OnSearchMissingPlayer();
		break;
	case m_ActState_Called:
		// Searchに呼ばれた際の処理
		Update_OnCalled();
		break;
	case m_ActState_BackBasedOn:
		// 元のパスに戻る
		Update_OnBackBasedOn();
		break;
	case m_ActState_Dizzy:
		// 混乱
		Update_OnDizzy();
		break;
	case m_ActState_Listen:
		// 音が聞こえた場所に向かう
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();							// アニメーション

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーを自身の座標と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);	// スポットライト
	Enemy::Act_SeachPlayer();						// 索敵

	m_enemyRender.Update();
}

void Enemy_Normal::Update_OnCraw()
{
	if (Act_CatchPlayer() == true) {
		m_ActState = m_ActState_CatchPlayer;
	}

	Enemy::Act_Craw();
}

void Enemy_Normal::Update_OnTracking()
{
	if (Act_CatchPlayer() == true) {
		m_ActState = m_ActState_CatchPlayer;
	}

	Enemy::Act_Tracking();
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Act_GoLocationListenSound(m_setPos);
}

void Enemy_Normal::Update_OnMoveMissingPosition()
{
	Enemy::Act_MoveMissingPosition();
}

void Enemy_Normal::Update_OnSearchMissingPlayer()
{
	Enemy::Act_SearchMissingPlayer();
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();
}

void Enemy_Normal::Update_OnDizzy()
{
	Enemy::Act_HitFlashBullet();
}


void Enemy_Normal::UpDate_OnListen()
{
	// 音爆弾を使ったとき
	Enemy::Act_GoLocationListenSound(m_itemPos);
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// 描画
	if (m_NotDrawFlag == false&&
		m_activeFlag == false) {
		m_enemyRender.Draw(rc);
	}
}