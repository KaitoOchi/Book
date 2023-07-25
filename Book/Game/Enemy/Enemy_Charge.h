#pragma once
#include "Enemy.h"
class Enemy_Charge : public Enemy
{
public:
	Enemy_Charge();
	~Enemy_Charge()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 突進行動
	/// </summary>
	/// <param name="time">突進するまでのチャージ時間</param>
	void Action_ChargeStart(float time);
	/// <summary>
	/// 突進した後の行動処理
	/// </summary>
	void Action_ChargeEnd();
	/// <summary>
	/// 壁との衝突判定
	/// </summary>
	void Action_ChargeHitWall();

	/// <summary>
	/// 巡回
	/// </summary>
	void Update_OnCraw();
	/// <summary>
	/// 突進
	/// </summary>
	void Update_OnCharge();
	/// <summary>
	/// 突進終了の処理
	/// </summary>
	void Update_OnChargeEnd();
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
	/// 捕獲処理
	/// </summary>
	void Update_OnCatch();

private:
	bool						m_CalculatedFlag = false;			// 突進用フラグ。一度だけ参照を行う
	float						m_Chargemove = 1.0f;				// 突進ステート時に乗算しているスカラー
};