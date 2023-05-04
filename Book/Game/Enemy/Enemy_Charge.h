#pragma once
#include "Enemy.h"
class Enemy_Charge : public Enemy
{
public:
	Enemy_Charge();
	~Enemy_Charge();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Animation();					// アニメーション
	void Update_OnCraw();				// 巡回
	void Update_OnCharge();				// 突進
	void Update_OnCalled();				// CALL時に実行
	void Update_OnBackBasedOn();		// 巡回状態に戻る
	void Update_OnConfusion();			// 閃光弾に当たったとき
	void UpDate_OnListen();				// 音爆弾を使用したとき
	void Update_OnCatch();				// 捕獲
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