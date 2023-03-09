#include "stdafx.h"
#include "Enemy.h"

#include "Player.h"

#define CATCHDECISION 10.0f	// 確保した判定になる範囲

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
	m_modelRender.SetPosition({ 0.0f,0.0f,0.0f });

	// アニメーションの読み込み
	//m_animationClips[enEnemyAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	//m_animationClips[enEnemyAnimationClip_Idle].SetLoopFlag(true);
	//m_animationClips[enEnemyAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	//m_animationClips[enEnemyAnimationClip_Walk].SetLoopFlag(true);
	//m_animationClips[enEnemyAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	//m_animationClips[enEnemyAnimationClip_Run].SetLoopFlag(true);
	//m_animationClips[enEnemyAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	//m_animationClips[enEnemyAnimationClip_Attack].SetLoopFlag(false);
	//m_animationClips[enEnemyAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	//m_animationClips[enEnemyAnimationClip_Damege].SetLoopFlag(false);

	return true;
}

void Enemy::Update()
{
	// 更新
	//m_modelRender.PlayAnimation(enEnemyAnimationClip_Idle);
	m_modelRender.Update();
}

void Enemy::MoveState()
{
	// エネミーの行動処理
}

void Enemy::HeadToDestination()
{
	// 目的地に向かう処理
	// プレイヤーを見失ったとき
	if (MissigPlayerFlag) {
		// 直近のパスを探す
		// パスに戻ったときにfalseにする
	}
	// プレイヤーを発見したとき
	if (FoundPlayerFlag) {
		// プレイヤーに向かう経路を作成
	}	
}

void Enemy::Seach()
{
	// 視野角の処理
}

void Enemy::CatchPlayer()
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

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}