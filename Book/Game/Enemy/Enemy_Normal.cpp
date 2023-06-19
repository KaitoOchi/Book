#include "stdafx.h"
#include "Enemy_Normal.h"
#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

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
	if (m_ActionState == m_ActionState_CatchPlayer) {
		m_enAnimationState = m_enAnimationState_Idle;
		return;
	}

	if (m_HearedSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		// 閃光弾を優先する
		m_HearedSoundBulletFlag = false;
	}

	if (m_ActionState == m_ActionState_Tracking && m_HearedSoundBulletFlag == true) {
		// 追跡を優先する
		m_HearedSoundBulletFlag = false;
	}

	// 閃光弾に当たったとき
	if (m_HitFlashBulletFlag == true) {
		m_ActionState = m_ActionState_Dizzy;
	}
	// 音爆弾が使用されたとき
	if (m_HearedSoundBulletFlag == true) {
		m_ActionState = m_ActionState_Listen;
	}

	switch (m_ActionState) {
	case m_ActionState_Craw:
		// 指定された範囲の巡回
		Update_OnCraw();
		break;
	case m_ActionState_Tracking:
		// プレイヤーを追跡する
		Update_OnTracking();
		break;
	case m_ActionState_Move_MissingPositon:
		// プレイヤーを最後に見た座標まで移動する
		Update_OnMoveMissingPosition();
		break;
	case m_ActionState_Search_MissingPlayer:
		// 見失ったプレイヤーを探す
		Update_OnSearchMissingPlayer();
		break;
	case m_ActionState_Called:
		// Searchの座標近くまで移動する
		Update_OnCalled();
		break;
	case m_ActionState_BackBasedOn:
		// 元のパスに戻る
		Update_OnBackBasedOn();
		break;
	case m_ActionState_Dizzy:
		// 混乱
		Update_OnDizzy();
		break;
	case m_ActionState_Listen:
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
	Enemy::Action_SeachPlayer();						// 索敵

	m_enemyRender.Update();
}

void Enemy_Normal::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		m_ActionState = m_ActionState_CatchPlayer;
	}

	Enemy::Action_CrawPath();
}

void Enemy_Normal::Update_OnTracking()
{
	if (Action_CatchPlayer() == true) {
		m_ActionState = m_ActionState_CatchPlayer;
	}

	Enemy::Action_TrackingPlayer();
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_setPos);
}

void Enemy_Normal::Update_OnMoveMissingPosition()
{
	Enemy::Action_MoveMissingPosition();
}

void Enemy_Normal::Update_OnSearchMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Action_MissingPlayer();
}

void Enemy_Normal::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}


void Enemy_Normal::UpDate_OnListen()
{
	// 音爆弾を使ったとき
	Enemy::Action_GoLocationListenSound(m_itemPos);
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// 描画
	if (m_NotDrawFlag == false&&
		m_activeFlag == false) {
		m_enemyRender.Draw(rc);
	}
}