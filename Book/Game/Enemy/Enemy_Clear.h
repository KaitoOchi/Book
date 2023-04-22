#pragma once
#include "Enemy.h"
class Enemy_Clear:public Enemy 
{
public:
	Enemy_Clear();
	~Enemy_Clear()override;
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);


	void Animation();					// アニメーション
	void Update_OnCraw();				// 巡回
	void Update_OnTracking();			// 追跡
	void Update_OnCalled();				// CALL時に実行
	void Update_OnBackBasedOn();		// 巡回状態に戻る
	void Update_OnConfusion();			// 錯乱
	void Update_OnCatch();				// 捕獲

	/// <summary>
	/// 透明化するか設定できる
	/// </summary>
	/// <param name="cle">trueなら透明化falseなら解除</param>
	/// <returns></returns>
	const void SetClearFlag(const bool cle)
	{
		m_clearFlag = cle;
	}
	/// <summary>
	/// 透明化しているか情報を得られる
	/// </summary>
	/// <returns></returns>
	const bool GetClearFlag()const
	{
		return m_clearFlag;
	}
private:
	/// <summary>
	/// 透明化を切り替える
	/// </summary>
	void ClearChange();


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

	//透明化を切り替える
	bool m_clearFlag=true;							//trueなら透明化falseなら解除
	EnEnemyActState m_SetActState=m_ActState;		//どちらの状態化を記憶する

	float m_clearChangeTime = 0.0f;					//切り替わるまでの時間

};

