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

	/// <summary>
	/// エネミーの巡回パターンを指定
	/// </summary>
	/// <param name="Line">縦に移動</param>
	/// <param name="Horizontal">横に移動</param>
	/// <param name="RightRotation">右回りに移動</param>
	/// <param name="LeftRotation">左回りに移動</param>
	/// <param name="RightAngle">直角に移動</param>
	void Pass(int PassState);

private:

	Enemy* enemy;

	ModelRender m_NormalModelRender;	// モデルレンダー

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

	// 指定できるパス移動
	enum EnEnemyPassState
	{
		Line,			// 縦
		Horizontal,		// 横
		RightRotation,	// 右回り
		LeftRotation,	// 左回り
		RightAngle,		// 直角
	};

	EnEnemyPassState PassState;
};