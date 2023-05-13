#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
#include "PlayerManagement.h"
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

	// モデルの読み込み
	m_enemyRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 3);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// パス移動
	m_point = &m_pointList[0];

	return true;
}
void Enemy_Clear::Update()
{
	// 描画しないフラグがtrueのとき
	if (m_NotDrawFlag == true) {
		return;
	}
	// デフォルトに戻すフラグがtrueのとき
	if (m_ChangeDefaultFlag == true) {
		m_ActState = CRAW;
		m_ChangeDefaultFlag = false;
	}

	// 閃光弾に当たった　且　音爆弾を聞いたとき
	if (m_HitSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		// 閃光弾を優先する
		m_HitSoundBulletFlag = false;
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
	case MISSING_MOVEPOSITON:
		Update_OnMoveMissingPosition();
		break;
	case MISSING_SEARCHPLAYER:
		Update_OnSearchMissingPlayer();
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
	}

	Enemy::PlayAnimation();		// アニメーション
	ClearChange();
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

void Enemy_Clear::Update_OnCraw()
{
	if (m_ActState == CATCH) {
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

void Enemy_Clear::Update_OnTracking()
{
	// プレイヤーを捕まえたとき
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
	// 
	Enemy::Act_Tracking();			// 追跡行動

	// 視野角にプレイヤーがいないとき
	if (m_TrakingPlayerFlag == false) {
		// プレイヤーを見失ったのので、見失った時の座標を記憶する。
		m_playerMissionPosition = m_playerManagement->GetPosition();
		m_ActState = MISSING_MOVEPOSITON;
	}
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Act_Called();

	// 視野角にプレイヤーがいるとき
	if (m_TrakingPlayerFlag == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnMoveMissingPosition()
{
	// プレイヤーを見失った位置まで移動する
	Enemy::Act_MoveMissingPosition();
}

void Enemy_Clear::Update_OnSearchMissingPlayer()
{
	// プレイヤーを探す
	Enemy::Act_SearchMissingPlayer();
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();
	m_ActState = CRAW;
}

void Enemy_Clear::Update_OnConfusion()
{
	Enemy::Act_HitFlashBullet();		// 閃光弾に当たったときの処理

	// 硬直が解けているとき
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}


void Enemy_Clear::UpDate_OnListen()
{
	// 音爆弾を使ったとき

	// 閃光弾を食らっているときは実行しない
	if (m_HitFlashBulletFlag == true) {
		m_HitFlashBulletFlag = false;
		return;
	}

	Enemy::Act_HitSoundBullet();

	// 効果が終了したとき
	if (m_HitSoundBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
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

void Enemy_Clear::Render(RenderContext& rc)
{
	//透明化解除
	if (m_clearFlag == false)
	{
		if (m_NotDrawFlag == false) {
			m_enemyRender.Draw(rc);
		}
	}
	
}