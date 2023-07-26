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

	/// <summary>
	/// 巡回
	/// </summary>
	void Update_OnCraw();
	/// <summary>
	/// 追跡
	/// </summary>
	void Update_OnTracking();
	/// <summary>
	/// 見失った位置まで移動する
	/// </summary>
	void Update_OnMoveMissingPosition();
	/// <summary>
	/// 見失ったプレイヤーを探す
	/// </summary>
	void Update_OnSearchMissingPlayer();
	/// <summary>
	/// Searchに呼ばれた時の処理
	/// </summary>
	void Update_OnCalled();
	/// <summary>
	/// 巡回状態に戻る
	/// </summary>
	void Update_OnBackBasedOn();
	/// <summary>
	/// 閃光弾に当たったときの処理
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// 音爆弾が使用された時の処理
	/// </summary>
	void UpDate_OnListen();

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
	EnEnemyActionState	m_setActionState = GetEnemyActionState();	//現在の状態を記録する

	bool				m_clearFlag = true;							//trueなら透明化falseなら解除
	float				m_clearChangeTime = 0.0f;					//切り替わるまでの時間
};
