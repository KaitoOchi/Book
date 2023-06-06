#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"
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
	Animation();

	// モデルを読み込む
	m_enemyRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);
	m_clearModelRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 3);

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
	if (m_activeFlag == true)
	{
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
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

	// 行動パターン
	switch (m_ActState) {
	case m_ActState_Craw:
		// 指定された範囲の巡回
		Update_OnCraw();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActState_Tracking:
		// プレイヤーを追跡する
		Update_OnTracking();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	case m_ActState_Move_MissingPositon:
		// プレイヤーを最後に見た座標まで移動する
		Update_OnMoveMissingPosition();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActState_Search_MissingPlayer:
		// 見失ったプレイヤーを探す
		Update_OnSearchMissingPlayer();
		m_clearModelRender.PlayAnimation(m_enAnimation_Loss, 1.0f);
		break;
	case m_ActState_Called:
		// Searchの座標近くまで移動する
		Update_OnCalled();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	case m_ActState_BackBasedOn:
		// 元のパスに戻る
		Update_OnBackBasedOn();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActState_Dizzy:
		// 混乱
		Update_OnDizzy();
		m_clearModelRender.PlayAnimation(m_enAnimation_Dizzy, 1.0f);
		break;
	case m_ActState_Listen:
		// 音が聞こえた場所に向かう
		UpDate_OnListen();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	}

	Enemy::PlayAnimation();			// アニメーション
	ClearChange();
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーを自身の座標と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);	// スポットライト
	Enemy::Act_SeachPlayer();						// 索敵

	m_enemyRender.Update();

	m_clearModelRender.SetPosition(m_position);
	m_clearModelRender.SetRotation(m_rotation);
	m_clearModelRender.Update();
}

void Enemy_Clear::Update_OnCraw()
{
	if (Act_CatchPlayer() == true) {
		m_ActState = m_ActState_CatchPlayer;
	}

	Enemy::Act_Craw();
}

void Enemy_Clear::Update_OnTracking()
{
	if (Act_CatchPlayer() == true) {
		m_ActState = m_ActState_CatchPlayer;
	}

	Enemy::Act_Tracking();
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Act_GoLocationListenSound(m_setPos);
}

void Enemy_Clear::Update_OnMoveMissingPosition()
{
	Enemy::Act_MoveMissingPosition();
}

void Enemy_Clear::Update_OnSearchMissingPlayer()
{
	Enemy::Act_SearchMissingPlayer();
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();
}

void Enemy_Clear::Update_OnDizzy()
{
	Enemy::Act_HitFlashBullet();
}

void Enemy_Clear::UpDate_OnListen()
{
	Enemy::Act_GoLocationListenSound(m_itemPos);
}

void Enemy_Clear::ClearChange()
{
	if (m_SetActState != m_ActState&&m_clearChangeTime>=0.0f)
	{
		m_clearChangeTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	if (m_ActState == m_ActState_Craw)
	{
		// 透明化
		m_clearFlag = true;
		m_SetActState = m_ActState_Craw;
		m_clearChangeTime = 0.0f;
	}
	else if (m_ActState == m_ActState_Tracking)
	{
		// 透明化解除
		m_clearFlag = false;
		m_SetActState = m_ActState_Tracking;
		m_clearChangeTime = 1.0f;
	
	}
}

void Enemy_Clear::Render(RenderContext& rc)
{
	if (m_activeFlag == true)
	{
		Vector3 move{ 0.0f,-300.0f,0.0f };
		m_characterController.Execute(move, 1.0f);
		return;
	}
	if (m_NotDrawFlag == true) {
		return;
	}

	if (m_clearFlag) {
		m_clearModelRender.Draw(rc);
	}
	else {
		m_enemyRender.Draw(rc);
	}

	
}