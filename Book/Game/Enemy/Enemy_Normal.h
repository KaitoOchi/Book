#pragma once
#include "Enemy.h"
class Enemy_Normal:public Enemy
{
public:
	Enemy_Normal();
	~Enemy_Normal()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// 巡回
	/// </summary>
	void Update_OnCraw();		
	/// <summary>
	/// 追跡
	/// </summary>
	void Update_OnTracking();
	/// <summary>
	/// Searchに呼ばれた時の処理s
	/// </summary>
	void Update_OnCalled();
	/// <summary>
	/// 見失った位置まで移動する
	/// </summary>
	void Update_OnMoveMissingPosition();
	/// <summary>
	/// 見失ったプレイヤーを探す
	/// </summary>
	void Update_OnSearchMissingPlayer();
	/// <summary>
	/// 巡回状態に戻る
	/// </summary>
	void Update_OnBackBasedOn();
	/// <summary>
	/// 閃光弾に当たったとき
	/// </summary>
	void Update_OnDizzy();
	/// <summary>
	/// 音爆弾を使用したとき
	/// </summary>
	void UpDate_OnListen();
};