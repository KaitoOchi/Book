#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

namespace 
{
	const Vector3	ADD_LENGTH = { 50.0f, 0.0f, 50.0f };	// 突進時に追加する長さ
	const float		STOP_TIMER = 1.5f;						// 溜め時間
	const float		MOVING_DISTANCE = 600.0f;				// 突進する移動距離
	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		ADD_SPEED = 1.3f;						// 乗算速度
}

Enemy_Charge::Enemy_Charge()
{
}

Enemy_Charge::~Enemy_Charge()
{
}

bool Enemy_Charge::Start()
{
	Enemy::LoadAnimation();

	// モデルの読み込み
	m_enemyRender.Init("Assets/modelData/enemy/enemy_charge.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);

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

	Enemy::PlayAnimation();

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// キャラクターコントローラーをモデルの位置と同期
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_enemyRender.Update();
}

void Enemy_Charge::Action_ChargeStart(float time)
{
	// 移動距離の長さが一定以上のとき
	if (m_sumPosition.Length() >= MOVING_DISTANCE) {
		// 突進を終了する
		SetEnemyActionState(m_ActionState_ChargeEnd);
		m_sumPosition = Vector3::Zero;
		return;
	}

	// タイマーがtrueのとき
	if (Action_StopMove(time, m_TimerState_UntilTheCharge) == true) {

		// 一度だけ実行する
		if (m_canCharge == false) {
			// 座標を参照
			m_chargeTergetPosition = m_playerManagement->GetPosition();

			// 何度も実行しないようにtrueにする
			m_canCharge = true;

			// エネミーからプレイヤーへ向かうベクトル
			m_chargeDiff = m_chargeTergetPosition - m_position;
			m_chargeDiff.y = 0.0f;
			m_chargeDiff.Normalize();
		}

		// 移動速度に加算
		Vector3 moveSpeed = m_chargeDiff * (MOVE_SPEED * ADD_SPEED);
		m_position += moveSpeed * m_chargeMove;

		// 総移動距離を計算
		m_sumPosition += moveSpeed;
		SetEnemyAnimationState(m_enAnimationState_Run);

		// 衝突判定
		Action_ChargeHitWall();
	}
	else {
		// 回転のみプレイヤーを追尾させる
		m_chargeDiff = m_playerManagement->GetPosition() - m_position;
		m_chargeDiff.Normalize();
		SetEnemyAnimationState(m_enAnimationState_Idle);
	}

	// 回転を教える
	Rotation(m_chargeDiff);
}

void Enemy_Charge::Action_ChargeEnd()
{
	m_position = m_position;								// 座標を固定
	
	ReSetAddTimer(m_TimerState_UntilTheCharge);				// タイマーをリセット
	m_sumPosition = Vector3::Zero;							// 移動距離をリセット
	m_canCharge = false;								// フラグを降ろす

	SetEffectDrawFlag(m_EffectState_QuestionMark,false);	// エフェクトのフラグを降ろす
	SetEffectDrawFlag(m_EffectState_ExclamationPoint, false);

	// プレイヤーが視野角内にいるとき
	if (GetTrackingPlayerFlag() == true) {
		Efect_FindPlayer();
		SetEnemyActionState(m_ActionState_Charge);
		return;
	}
	else {
		// いないときは巡回状態に戻る
		SetEnemyActionState(m_ActionState_Search_MissingPlayer);
	}
}

void Enemy_Charge::Action_ChargeHitWall()
{
	// 壁に衝突する判定

	// 壁に衝突したかどうか
	// プレイヤーの方向へ向かう単位ベクトルにスカラーを乗算したものを加算して渡す
	if (Enemy::WallAndHit(m_position + (m_chargeDiff * ADD_LENGTH.x)) == false) {
		// 衝突したとき
		m_chargeMove = 0.0f;									// 乗算している値を0にして動かないようにする

		ReSetAddTimer(m_TimerState_UntilTheCharge);				// タイマーをリセット
		m_sumPosition = Vector3::Zero;							// 移動距離をリセット
		m_canCharge = false;								// フラグを降ろす

		SetEffectDrawFlag(m_EffectState_QuestionMark,false);	// !のエフェクトのフラグを降ろす
		SetEnemyActionState(m_ActionState_Dizzy);
		return;
	}

	// 衝突していないときは続行する
	m_chargeMove = 1.0f;
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

	Action_ChargeStart(STOP_TIMER);
}

void Enemy_Charge::Update_OnChargeEnd()
{
	Action_ChargeEnd();
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
	Enemy::Action_GoLocationListenSound(m_gatherPosition);
}

void Enemy_Charge::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Charge::UpDate_OnListen()
{
	Enemy::Action_GoLocationListenSound(m_itemPosition);
}

void Enemy_Charge::Update_OnCatch()
{
	Enemy::Action_CatchPlayer();
}

void Enemy_Charge::Render(RenderContext& rc)
{
	// 描画
	if (GetNotDrawFlag() == false&&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}