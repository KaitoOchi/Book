#pragma once
#include "Enemy.h"
class Enemy_Normal:public Enemy
{
public:
	Enemy_Normal();
	~Enemy_Normal();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Act();							// エネミーの行動パターン
	void Animation();					// アニメーション

private:

	Enemy* enemy;

	// アニメーションクリップ
	enum EnEnemyAnimationClip
	{
		m_enAnimationClip_Idle,			// 待機
		m_enAnimationClip_Walk,			// 歩く
		m_enAnimationClip_Run,			// 走る
		m_enAnimationClip_Attack,		// 攻撃
		m_enAnimationClip_Damege,		// 被弾
		m_enAnimationClip_Num
	};
	// アニメーションクリップ
	AnimationClip m_animationClips[m_enAnimationClip_Num];
};