#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f			// エネミーの視野角(初期:120)

namespace
{
	const float		MOVE_SPEED = 3.0f;						// 移動速度
	const float		CHANGING_DISTANCE = 20.0f;				// 目的地を変更する距離
	const float		CALCULATIONNAVI_TIMER = 1.0f;			// ナビメッシュを再度計算するタイマー
	const float		CANMOVE_TIMER = 10.0f;					// 再度行動できるまでのタイマー
	const float		WAITING_TIMER = 3.0f;					// パス移動時の待機時間
	const float		AI_RADIUS = 50.0f;						// AIエージェントの半径
	const float		AI_HIGH = 200.0f;						// AIエージェントの高さ
	const float		CATCH_DECISION = 20.0f;					// プレイヤーを確保したことになる範囲
	const float		ACCESS_DECISION = 40.0f;				// プレイヤーに近づく範囲
	const float		SCALESIZE = 1.3f;						// SetScaleのサイズ
	const Vector3	BOXSIZE = { 75.0f, 90.0f,60.0f };		// CharacterControllerのサイズ
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	// キャラクターコントローラーを初期化する
	m_characterController.Init(BOXSIZE, m_position);

	// ナビメッシュを構築
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	return true;
}

bool Enemy::SeachPlayer()
{
	// 視野角の処理
	// trueのときプレイヤーを発見している

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;

	// プレイヤーにある程度近いとき
	if (diff.LengthSq() <= 700.0 * 700.0f) {
		// エネミーからプレイヤーへ向かうベクトルを正規化
		diff.Normalize();
		// エネミーの正面ベクトル、エネミーからプレイヤーへ向かうベクトルの内積を計算
		float cos = m_forward.Dot(diff);
		// 内積から角度を計算
		float angle = acosf(cos);
		// 角度が視野角より狭いとき
		if (angle <= (FIELDOF_VIEW) {
			// プレイヤーを発見
			m_rotation.SetRotationY(angle);
			return true;
		}
	}

	return false;
}

bool Enemy::CatchPlayer()
{
	// プレイヤーを確保する処理
	// trueのときプレイヤーを確保している
	bool flag = false;

	// エネミーからプレイヤーへ向かうベクトルを計算する
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// ベクトルの長さを求める
	float length = diff.Length();

	// ベクトルが一定以下のとき
	if (length <= CATCH_DECISION) {
		// 捕まえる処理を行う
		// 攻撃アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Attack;

		return true;
	}

	return false;
}

void Enemy::HitFlashBullet()
{
	// 閃光弾が当たったとき
	// trueなら当たった
	if (HitFlashBulletFlag == true) {
		// 被弾アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Damege;

		// タイマーがtrueのとき
		if (Act_Stop(CANMOVE_TIMER) == true) {
			HitFlashBulletFlag = false;		// フラグを降ろす
			addTimer = 0.0f;				// 加算用タイマーをリセット

		}
		// そうでないとき
		else {
			// 待機アニメーションを再生
			m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
		}
	}
}

void Enemy::Act_Craw()
{
	// パス移動
	
	// 目標とするポイントの座標から、現在の座標を引いたベクトル
	Vector3 diff = m_point->s_position - m_position;

	// 距離が一定以内なら目的地とするポイントを変更する
	if (diff.Length() <= CHANGING_DISTANCE) {

		// 現在の目的地のポイントが配列の最後のとき
		if (m_point->s_number == m_pointList.size()) {
			// 一番最初のポイントを目的地とする
			m_point = &m_pointList[0];
		}
		// そうでないとき
		else {
			m_point = &m_pointList[m_point->s_number];
		}

		addTimer = 0.0f;	// 加算用タイマーをリセット
	}

	// 目標とするポイントの座標から、現在の座標を引いたベクトル
	Vector3 moveSpeed = m_point->s_position - m_position;
	// 正規化
	moveSpeed.Normalize();
	// ベクトルにスカラーを乗算
	moveSpeed *= MOVE_SPEED;

	// タイマーがtrueのとき
	if (Act_Stop(WAITING_TIMER) == true) {
		// 待機アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
		// 座標に加算する
		m_position += moveSpeed;
	}
	// そうでないとき
	else {
		// 歩きアニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
	}
}

void Enemy::Act_Tracking()
{
	// 経過時間を加算
	NaviTimer += g_gameTime->GetFrameDeltaTime();

	// 一定時間以下のときreturn
	if (CALCULATIONNAVI_TIMER >= NaviTimer) {
		return;
	}

	// ナビメッシュでの移動
	// プレイヤーの座標を獲得する
	m_playerPos = m_playerManagement->GetPosition();

	bool isEnd;							// パス移動が終了した際のフラグ

	// パス検索
	m_pathFiding.Execute(
		m_path,							// 構築されたパスの格納先
		m_nvmMesh,						// ナビメッシュ
		m_position,						// 開始座標
		m_playerPos,					// 目標地点
		PhysicsWorld::GetInstance(),	// 物理エンジン
		AI_RADIUS,						// AIエージェントの半径
		AI_HIGH							// AIエージェントの高さ
	);

	// パス上を移動する
	m_position = m_path.Move(
		m_position,						// 移動させる座標
		MOVE_SPEED,						// 移動速度
		isEnd							// 終了時にtrueを格納するフラグ
	);

	// 捕まえたとき
	if (CatchPlayer() == true) {
		// 待機アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

		m_fontRender.SetText(L"捕まえた");
		m_fontRender.SetPosition({ 500.0f, 200.0f, 0.0f });
	}
}

void Enemy::Act_Access()
{
	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// ベクトルの長さ
	float length = diff.Length();

	// ベクトルが一定以下のとき
	if (length <= ACCESS_DECISION) {
		// ベクトルを正規化
		diff.Normalize();
		// エネミーの座標に加算
		m_position += diff * MOVE_SPEED;
		// 歩きアニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
	}

	// 捕まえたとき
	if (CatchPlayer() == true) {
		// 待機アニメーションを再生
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

		m_fontRender.SetText(L"捕まえた");
		m_fontRender.SetPosition({ 500.0f, 200.0f, 0.0f });
	}
}

bool Enemy::Act_Stop(float time)
{
	// 経過時間を加算
	addTimer += g_gameTime->GetFrameDeltaTime();

	// 加算された時間が一定以上になったとき
	if (time <= addTimer) {
		return true;
	}

	return false;
}