#include "stdafx.h"
#include "Enemy.h"

#include "Player2D.h"
#include "Player3D.h"

#define FIELDOFVIEW Math::PI / 180.0f) * 120.0f // エネミーの視野角(初期値120)

namespace
{
	const float		 LINEARCOMPLETION = 0.5f;						// 線形補完のフレーム数
	const float		 CATCHDECISION = 10.0f;					// プレイヤーを確保したことになる範囲
	const float		 SCALESIZE = 1.3f;						// SetScaleのサイズ
	const Vector3	 BOXSIZE = { 75.0f, 90.0f,60.0f };	// CharacterControllerのサイズ
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	// モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm");
	m_modelRender.SetScale({ 1.3f, 1.3f, 1.3f });

	// アニメーションの読み込み
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Attack].Load("Assets/animData/enemy/attack1.tka");
	m_animationClips[m_enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	m_animationClips[m_enAnimationClip_Damege].SetLoopFlag(false);

	// キャラクターコントローラーを初期化する
	m_characterController.Init(BOXSIZE, m_position);

	// プレイヤーのインスタンスを探す
	m_player2D = FindGO<Player2D>("player2d");
	m_player3D = FindGO<Player3D>("player3d");

	return true;
}

void Enemy::Update()
{
	// 更新
	//HeadToDestination();
	m_modelRender.Update();
}

void Enemy::HeadToDestination()
{
	// プレイヤーを見失ったとき
	if (MissigPlayerFlag) {
		// 巡回に移行する
		m_enEnemyActState = m_enEnemyActState_Craw;

		// 直近のパスを探す
		// パスに戻ったときにfalseにする
	}

	// プレイヤーを発見したとき
	if (FoundPlayerFlag) {
		// 追跡に移行する
		m_enEnemyActState = m_enEnemyActState_Tracking;

		// プレイヤーに向かう経路を作成
	}

	// 目的地に向かう処理
	Act();
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

bool Enemy::SeachPlayer()
{
	// 視野角の処理
	// trueのときプレイヤーを発見している
	bool flag = false;

	// エネミーからプレイヤーへ向かうベクトル
	Vector3 diff =  /*playerの座標*/ -m_position;

	// プレイヤーにある程度近いとき
	if (diff.LengthSq() <= 700.0 * 700.0f) {
		// エネミーからプレイヤーへ向かうベクトルを正規化
		diff.Normalize();
		// エネミーの正面ベクトル、エネミーからプレイヤーへ向かうベクトルの内積を計算
		float cos = m_forward.Dot(diff);
		// 内積から角度を計算
		float angle = acosf(cos);
		// 角度が視野角より狭いとき
		if (angle <= (FIELDOFVIEW) {
			// プレイヤーを発見
			flag = true;
		}
	}

	return flag;
}

bool Enemy::CatchPlayer()
{
	// プレイヤーを確保する処理
	// エネミーからプレイヤーへ向かうベクトルを計算する
	//float diff = player->position - m_position;

	//// ベクトルが一定以下のとき
	//if (diff <= CATCHDECISION) {
	//	// 捕まえたフラグをtrueにする
	//	CatchPlayerFlag = true;

	//	// 捕まえたことを伝える
	//	m_fontRender.SetText(L"捕まえた");
	//	m_fontRender.SetPosition({ 0.0f, 0.0f, 0.0f });
	//}
}

void Enemy::HitFlashBullet()
{
	// 閃光弾が当たったときの処理
}

void Enemy::Animation()
{
	// アニメーションのステート
	switch (m_enEnemyAnimationState) {
		// 待機
	case Enemy::m_enEnemyAnimationState_Idle:
		m_modelRender.PlayAnimation(m_enAnimationClip_Idle, LINEARCOMPLETION);
		break;
		// 歩く
	case Enemy::m_enEnemyAnimationState_Walk:
		m_modelRender.PlayAnimation(m_enAnimationClip_Walk, LINEARCOMPLETION);
		break;
		// 走る
	case Enemy::m_enEnemyAnimationState_Run:
		m_modelRender.PlayAnimation(m_enAnimationClip_Run, LINEARCOMPLETION);
		break;
		// 攻撃
	case Enemy::m_enEnemyAnimationState_Attack:
		m_modelRender.PlayAnimation(m_enAnimationClip_Attack, LINEARCOMPLETION);
		break;
		// 被弾
	case Enemy::m_enEnemyAnimationState_Damege:
		m_modelRender.PlayAnimation(m_enAnimationClip_Damege, LINEARCOMPLETION);
		break;
	}
}

void Enemy::Render(RenderContext& rc)
{
	// 描画
	m_modelRender.Draw(rc);
}