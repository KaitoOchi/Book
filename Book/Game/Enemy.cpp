#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f				// エネミーの視野角(初期値120)

namespace
{
	const float		MOVE_SPEED = 8.0f;						// 移動速度
	const float		CANMOVE_TIMER = 10.0f;					// 再度行動できるまでのタイマー
	const float		LINEAR_COMPLETION = 0.5f;				// 線形補完のフレーム数
	const float		CATCH_DECISION = 20.0f;					// プレイヤーを確保したことになる範囲
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

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
}

bool Enemy::SeachPlayer()
{
	// 視野角の処理
	// trueのときプレイヤーを発見している
	bool flag = false;

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
			flag = true;
		}
	}

	return flag;
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
		// 捕まえたフラグをtrueにする
		flag = true;
	}

	return flag;
}

bool Enemy::HitFlashBullet()
{
	// 閃光弾が当たったときの処理
	// 閃光弾がヒットしたときtrue
	bool flag = false;

	// 閃光弾が当たったとき
	if (HitFlashBulletFlag == true) {
		// 移動を硬直
		m_position = m_position;

		HitAfterFlashBullet();
	}

	return flag;
}

void Enemy::HitAfterFlashBullet()
{
	// 閃光弾が当たった後の硬直処理
	// 経過時間を加算
	float time = +g_gameTime->GetFrameDeltaTime();

	// 加算された時間が一定以上になったとき
	if (CANMOVE_TIMER <= time) {
		// 加算する経過時間をリセット
		time = 0;
		return;
	}
}

void Enemy::Act()
{
	// 行動パターン
	switch (m_enEnemyActState) {
		// 巡回
	case Enemy::m_enEnemyActState_Craw:
		break;
		// 追跡
	case Enemy::m_enEnemyActState_Tracking:
		break;
	}
}

void Enemy::Animation()
{
	// アニメーションのステート
	switch (m_enEnemyAnimationState) {
		// 待機
	case Enemy::m_enEnemyAnimationState_Idle:
		m_modelRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// 歩く
	case Enemy::m_enEnemyAnimationState_Walk:
		m_modelRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// 走る
	case Enemy::m_enEnemyAnimationState_Run:
		m_modelRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// 攻撃
	case Enemy::m_enEnemyAnimationState_Attack:
		m_modelRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// 被弾
	case Enemy::m_enEnemyAnimationState_Damege:
		m_modelRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}