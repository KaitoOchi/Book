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

	void Update_OnCraw();					// 巡回
	void Update_OnTracking();				// 追跡
	void Update_OnMoveMissingPosition();	// 見失った位置まで移動する
	void Update_OnSearchMissingPlayer();	// 見失った位置まで移動する
	void Update_OnCalled();					// CALL時に実行
	void Update_OnBackBasedOn();			// 巡回状態に戻る
	void Update_OnDizzy();					// 閃光弾に当たったとき
	void UpDate_OnListen();					// 音爆弾を使用したとき

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

	ModelRender			m_clearModelRender;							//半透明時のモデル

	//透明化を切り替える
	bool				m_clearFlag = true;							//trueなら透明化falseなら解除
	EnEnemyActionState	m_SetActionState = GetEnemyActionState();	//どちらの状態化を記憶する

	float				m_clearChangeTime = 0.0f;					//切り替わるまでの時間
};

